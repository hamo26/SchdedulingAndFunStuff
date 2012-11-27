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

void *switch_thread_routine(void *arg)
{
    /* Add your code here */
    //iterate over all input ports.
    while(1){
	int i;
	port_t current_input_port;			
	
	printf( "DIE:%d\n", die_flag );

	if (die_flag == 0) { 
	    std::cout<<"DIE is true now \n";
	    pthread_exit(NULL);
	}else{
	    std::cout<<"NOT DIE"<<"\n";
	}

	for (i = 0; i < 4; i++) {
	    current_input_port = in_port[i];
	    if (current_input_port.flag) {
		pthread_mutex_lock(&current_input_port.mutex);
		forward_packet_to_port(current_input_port.packet);
		pthread_mutex_unlock(&current_input_port.mutex);
		current_input_port.flag = FALSE;
	    }
	    usleep(1000000);
	}

    }

}

void forward_packet_to_port(packet_t packet) {
    //This should be thread safe because only one thread deals with an output port.
    int dest_port;	
    dest_port = get_port_from_packet(&packet);
    std::cout<<"dest_port:"<<dest_port<<"\n";
    if (!out_port[dest_port].flag) {
	pthread_mutex_lock(&out_port[dest_port].mutex);
	out_port[dest_port].packet = packet;
	pthread_mutex_unlock(&out_port[dest_port].mutex);
	out_port[dest_port].flag = TRUE;
    } 
}

/* Gets a port number from a packet*/
int get_port_from_packet(packet_t* packet) {
    return cam_lookup_address(&(packet->address));
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
