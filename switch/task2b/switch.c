/*******************************************************
 *
 *  Switch Routines for EECE 494, Assignment 2.
 *
 *  Created by _________, University of British Columbia
 *
 *  This is where you will put your routines to implement
 *  the switch routines.  You should use the headers as
 *  supplied.  All your code for Assignment 2 will go in this
 *  file (and possibly in switch.h)
 *
 ******************************************************/

#include "defs.h"
using namespace std;

//These are the 16 voq buffers that the rr thread reads from. Initialized below.
VOQBUFFER voq_buffer[4][4];

//These are the voq buffer parameters used to get, pop and block the queue.
int voq_size[4][4] = {0};
int voq_front[4][4] = {0};
int voq_rear[4][4] = {0};

// This is used to tell the first iteration of the cell time
int cell_first = 1;

//These are the input and output round robin schedules. It is easier to use arrays for updating the position of the highest priority. Use (granted/accepted port) modulus 4 to point to the next highest priority.

int rr_schedule_input[4] =  {0,1,2,3};

int rr_schedule_output[4] = {0,1,2,3};


//Request queue on output ports. An array of ints. At most 4 requests per port.
vector<int> output_request_queue[4];

//Request queue on input ports. An array of ints. At most 4 grants per port.
vector<int> input_grant_queue[4];


void *switch_thread_routine(void *arg)
{
    //This is the input thread that puts directly on the voqs
    pthread_t in_port_thread; 

    //We may not need an out put thread. The rr thread might be able to put directly on the ports.
    pthread_t out_port_thread;

    //This is the driver thread that keeps track of cell time. Take a look
    //at the pthreads library to figure out how it could suspend and restart
    //the rr_thread.
    pthread_t rr_thread; 
    pthread_create(&rr_thread, NULL, schedule_rr, NULL);

    //initialize voq buffer mutexes. Required when using a struct.
    for (int i=0; i<4; i++) {
	for (int j=0; j<4; j++) {
	    voq_buffer[i][j].mutex = PTHREAD_MUTEX_INITIALIZER;
	}
    }

    for(int i=0; i<4; i++){
	out_port[i].is_written = FALSE;
    }


    //init output_request_queue and input_grant_queue
    for(int i=0; i<4; i++){
	vector<int> rque;
	output_request_queue[i] = rque;
	vector<int> gque;
	input_grant_queue[i] = gque;
    }

    //The driver thread needs to somehow be the parent thread of the rr_thre
    //ad. The input thread can remain busy.	
    pthread_create(&in_port_thread, NULL, read_in_port_packet, NULL);
    pthread_join(in_port_thread, NULL);
    pthread_exit(NULL);
}

void *schedule_rr(void* p){
    while(1){

	while(cell_flag == 0){
	    // Reset all buffers and flags
	    cell_first = 1;
	    for(int i=0; i<4; i++){
		out_port[i].is_written = FALSE;
	    }
	}

	// We only process RR in cell time (nano sleep in harness)
	while(cell_flag == 1){
	    // 1. Loop through voqs and send request for each packet
	    for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
		    // Loop through the head packet in this inport buffer(VOQ)
		    // Send the request from this inport to the outport request que
		    if( voq_buffer[i][j].buffer.size() != 0){// Make sure the VOQ is not empty
			// Write the inport NO. to the correct output request que
			output_request_queue[j].push_back(i);
			//cout<<"VOQ buffer in "<<i<< " and out "<<j<<", size:"<< voq_buffer[i][j].buffer.size()<<"\n";
		    }
		}
	    }

	    // 2. Loop through outputs request placeholder and choose the one the rph-array based on the prio. And send grants to grant-place-holder of the ports
	    for(int i=0; i<4; i++){
		int min_distance = 1000;
		int high_inport = 9; // We find something matches and update it, otherwise it is 9 as a flag.
		int output_port = i;
		for(int j=0; j<output_request_queue[i].size(); j++){
		    // Check the prio.
		    int out_prio = rr_schedule_output[i];
		    int input_port = output_request_queue[i][j];
		    // Compare prio with in_port and choose the nearest one
		    if(input_port < out_prio) input_port += 3;
		    int distance = input_port - out_prio;
		    if(distance < min_distance){
			min_distance = distance;
			if(input_port > 3) input_port -= 3;
			high_inport = input_port;
		    }
		}
		if(high_inport != 9){//We found the next highest inport
		    cout << "Pushing outport "<<output_port<<" to input_grant_queue\n";
		    input_grant_queue[high_inport].push_back(output_port);
		}
		output_request_queue[i].clear();
		//cout << "Highest inport for outport " << i<<": "<< high_inport<<"\n";
	    }

	    // 3. Loop through each input port's grant-place-holder and choose which to accept based on the prio. And finally transfer the packet to output port.
	    for(int i=0; i<4; i++){
		int min_distance = 1000;
		int high_outport = 9; // Same as above
		int input_port = i;
		for(int j=0; j<input_grant_queue[i].size(); j++){
		    int in_prio = rr_schedule_input[i];
		    int output_port = input_grant_queue[i][j];
		    // Compare same as above
		    if(output_port < in_prio) output_port += 3;
		    int distance = output_port - in_prio;
		    if(distance < min_distance){
			min_distance = distance;
			if(output_port > 3) output_port -= 3;
			high_outport = output_port;
		    }
		}
		//cout << "Highest outport for inport " << i<<": " << high_outport<<"\n";
		if(high_outport != 9 && out_port[high_outport].is_written == FALSE){//Found the outport and match
		    // Copy the packet
		    while(1){
			port_lock(&(out_port[high_outport]));
			if(out_port[high_outport].flag){
			    port_unlock(&(out_port[high_outport]));
			}else{
			    // Poping actually
			    packet_t* packet = get_packet_from_voq(input_port, high_outport);
			    forward_packet_to_port(packet, high_outport);
			    out_port[high_outport].is_written = TRUE;
			    port_unlock(&(out_port[high_outport]));
			    break;
			}
		    }
		    if(cell_first == 1){
			// Make sure we only update rr list at the first iteration of each cell time
			cell_first = 0;

			// Update prio for both inport and outport
			rr_schedule_output[high_outport] = high_outport + rr_schedule_output[high_outport]%4;
			if(rr_schedule_output[high_outport] > 3) rr_schedule_output[high_outport] -= 3;
			rr_schedule_input[i] = i + rr_schedule_input[i]%4;
			if(rr_schedule_input[i] > 3) rr_schedule_input[i] -= 3;
		    }
		}
		input_grant_queue[i].clear();
	    }
	}
    }

}

// Pass INDEX
void forward_packet_to_port(packet_t* packet, int destination) {
    printf("Forwarding packet: "); packet_print(packet);
    packet_copy(packet, &(out_port[destination].packet));
    out_port[destination].flag = TRUE;
}

// Gets a port number from a packet
int get_port_from_packet(packet_t* packet) {
    return cam_lookup_address(&(packet->address));
}

// Add a packet to a specific voq.
// Pass INDEX
void add_packet_to_voq(int input_port, packet_t* packet) {
    int dest_port = get_port_from_packet(packet);
    pthread_mutex_lock(&(voq_buffer[input_port][dest_port].mutex));
    // FIXME thinkabout vector use size
    if ( voq_buffer[input_port][dest_port].buffer.size() < SIZE) {
	voq_buffer[input_port][dest_port].buffer.push_back(packet);
	//packet_copy(packet, voq_buffer[input_port][dest_port].buffer.back());
    }
    pthread_mutex_unlock(&(voq_buffer[input_port][dest_port].mutex));
}

// Pop a packet from a specific voq.
// Taking INDEX as params
packet_t* get_packet_from_voq(int input_port, int dest_port) {
    pthread_mutex_lock(&(voq_buffer[input_port][dest_port].mutex));
    packet_t* packet;
    if (voq_buffer[input_port][dest_port].buffer.size() != 0) {
	//packet_copy(voq_buffer[input_port][dest_port].buffer.front(), packet);
	// Poping from the front
	packet = voq_buffer[input_port][dest_port].buffer.front();
	voq_buffer[input_port][dest_port].buffer.erase(voq_buffer[input_port][dest_port].buffer.begin());
    } else {
	packet = NULL;
    }
    pthread_mutex_unlock(&(voq_buffer[input_port][dest_port].mutex));
    return packet;
}

// Here we add packet from inport to VOQ directly, this is a busy thread looping all the time.
void *read_in_port_packet(void* p)
{
    while(1){
	int i;

	if (die == TRUE) { 
	    std::cout<<"DIE is true now \n";
	    pthread_exit(NULL);
	}

	for (i = 0; i < 4; i++) {
	    //printf("Reading from port %d\n",i);

	    if (in_port[i].flag == TRUE) {
		port_lock(&(in_port[i]));
		add_packet_to_voq(i,&(in_port[i].packet));
		in_port[i].flag = FALSE;
		port_unlock(&(in_port[i]));
	    }

	}

    }
}

// This needs to be re written completely to use the RR thread. We also need something for the driver thread in another method.
/*
   void *read_out_port_packet(void* p)
   {
   while (1) {

   packet_t packet;
   int dest_port;
   BOOL flag = TRUE;
//	   printf("Attempting to get lock for regional mutex in out thread\n");
pthread_mutex_lock(&main_buffer_region_mutex);
//	   printf("Successfuly retrieved lock for regional mutex in out thread\n");
if (size == 0) {
printf("Thread out locked.\n");
pthread_cond_wait(&main_buffer_data_available,&main_buffer_region_mutex);
printf("Thread out unlocked.\n");
}

packet = get_packet_from_buffer(base_network_buffer);
pthread_cond_signal(&main_buffer_space_available);
pthread_mutex_unlock(&main_buffer_region_mutex);

dest_port = get_port_from_packet(&packet);

while (1) {
port_lock(&(out_port[dest_port]));
if (out_port[dest_port].flag) {
port_unlock(&(out_port[dest_port]));
} else {
forward_packet_to_port(packet, dest_port);
port_unlock(&(out_port[dest_port]));
break;
}
}
}
}
 */

void switch_init()
{
    /* Add your code here.  At the very least, you probably
       want to include a call to cam_init() */
    /* Initalize the routing table by calling your routine */

    cam_init();

}

void switch_add_entry(ip_address_t *address,
	int port)
{
    /* Add your code here.  It might be as simple as adding
       a call to cam_add_entry() */

    cam_add_entry(address,port);
}

void switch_free()
{
    /* add your code (if any) here */
    cam_free();
}
