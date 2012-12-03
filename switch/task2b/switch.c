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


//These are the 16 voq buffers that the rr thread reads from. Initialized below.
VOQBUFFER voq_buffer[4][4];

//These are the voq buffer parameters used to get, pop and block the queue.
Int voq_size[4][4] = {0};
int voq_front[4][4] = {0};
int voq_rear[4][4] = {0};

//These are the input and output round robin schedules. It is easier to use arrays for updating the position of the highest priority. Use (granted/accepted port) modulus 4 to point to the next highest priority.

int rr_schedule_input[4][4] =  {{1, 2, 3, 4},
			       {1, 2, 3, 4},
			       {1, 2, 3, 4},
			       {1, 2, 3, 4}};

int rr_schedule_output[4][4] =    {{1, 2, 3, 4},
                                   {1, 2, 3, 4},
                                   {1, 2, 3, 4},
                                   {1, 2, 3, 4}};


//Request queue on output ports. An array of ints. At most 4 requests per port.
int output_request_queue[4][4];

//Request queue on input ports. An array of ints. At most 4 grants per port.
int input_grant_queue[4][4];


void *switch_thread_routine(void *arg)
{
	//This is the input thread that puts directly on the voqs
        pthread_t in_port_thread; 
	
	//We may not need an out put thread. The rr thread might be able to put 	//directly on the ports.
	pthread_t out_port_thread;
	
	//This is the rr algorithm thread doing most of the work. Replace what
	//is currently the out_thread with the rr thread.
	pthread_t rr_thread; 
	
	//This is the driver thread that keeps track of cell time. Take a look
	//at the pthreads library to figure out how it could suspend and restart
	//the rr_thread.
	pthread_t driver_thread;

	int i,j;
	
	//initialize voq buffer mutexes. Required when using a struct.
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			voq_buffer[i][j].mutex = PTHREAD_MUTEX_INITIALIZER;
		}
	}

	//The driver thread needs to somehow be the parent thread of the rr_thre	//ad. The input thread can remain busy.	
	pthread_create(&in_port_thread,NULL,read_in_port_packet,NULL);
	pthread_create(&out_port_thread,NULL,read_out_port_packet,NULL);
	pthread_join(in_port_thread,NULL);
	pthread_exit(NULL);
}

void forward_packet_to_port(packet_t packet, int destination) {
    std::cout<<"dest_port:"<<destination<<"\n";
    packet_copy(&packet, &(out_port[destination].packet));
    printf("Forwarding packet: "); packet_print(&packet);
    out_port[destination].flag = TRUE;
}

/* Gets a port number from a packet*/
int get_port_from_packet(packet_t* packet) {
    return cam_lookup_address(&(packet->address));
}

//Add a packet to a specific voq.
void add_packet_to_voq(int input_port, packet_t packet) {
	int dest_port = get_port_from_packet(&packet);
	pthread_mutex_lock(&(voq_buffer[input_port][dest_port].mutex));
	if (!voq_size[input_port][dest_port] == SIZE) {
		packet_copy(&packet, &(voq_buffer[input_port][dest_port].buffer));
		voq_rear[input_port][dest_port] = (voq_rear[input_port][dest_port] + 1) % SIZE;
		voq_size[input_port][dest_port]++;
	}
	pthread_mutex_unlock(&(voq_buffer[input_port][dest_port].mutex));
}

//Get a packet from a specific voq.
packet_t* get_packet_from_buffer(int input_port, int dest_port) {
	pthread_mutex_lock(&(voq_buffer[input_port][dest_port].mutex));
	packet_t* packet;
	if (!voq_size[input_port][dest_port] == 0) {
		packet_copy(&(voq_buffer[input_port][dest_port].buffer), packet);
		voq_front[input_port][dest_port] = (voq_front[input_port][dest_port] + 1) % SIZE;
		voq_size[input_port][dest_port]--;
	} else {
		packet = NULL;
	}
	pthread_mutex_unlock(&(voq_buffer[input_port][dest_port].mutex));
	return packet;
}

//THis needs to be re written completely to add packets directly to voq. But use this code as an example.
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
				printf("Adding data to buffer from port %d\n",i);
				pthread_mutex_lock(&main_buffer_region_mutex);
				if (size == SIZE) {	
					printf("The buffer is full\n");
				       	printf("Thread in locked\n");
					pthread_cond_wait(&main_buffer_space_available,&main_buffer_region_mutex);
					printf("Thread in unlocked\n");
				}
				add_packet_to_buffer(base_network_buffer, &(in_port[i].packet));
				pthread_cond_signal(&main_buffer_data_available);
				pthread_mutex_unlock(&main_buffer_region_mutex);
				//Add logic here to read from network port.
				in_port[i].flag = FALSE;
				port_unlock(&(in_port[i]));
			}
			
		}

    }
}

//This needs to be re written completely to use the RR thread. We also need something for the driver thread in another method.
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
