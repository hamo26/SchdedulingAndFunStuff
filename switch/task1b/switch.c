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
  int i;
  port_t current_input_port;			
  packet_t send_packet;
  while(1) {
  	for (i = 0; i < 4; i++) {
  	printf("I am here\n");
	if (die == TRUE) { return NULL; }
	printf("I am here\n");
	current_input_port = in_port[i];
	if (current_input_port.flag) {
		pthread_mutex_lock(&current_input_port.mutex);
		send_packet = current_input_port.packet;
		printf("port sent to packet  %d\n",get_port_from_packet(&current_input_port.packet));
		current_input_port.flag = false;
		pthread_mutex_unlock(&current_input_port.mutex);
		forward_packet_to_port(current_input_port.packet);
	} 
	}
  }
  sleep(THREAD_SLEEP_TIME);				   
}

void forward_packet_to_port(packet_t packet) {
//This should be thread safe because only one thread deals with an output port.
	int dest_port;	
	dest_port = get_port_from_packet(&packet);
	if (!out_port[dest_port].flag) {
		pthread_mutex_lock(&out_port[dest_port].mutex);
		out_port[dest_port].packet = packet;
		out_port[dest_port].flag = true;
		pthread_mutex_unlock(&out_port[dest_port].mutex);
	} 
	return;
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
