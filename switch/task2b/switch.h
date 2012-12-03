
/*******************************************************
 *
 *  Switch Routines for EECE 494, Assignment 2
 *
 *  Created by _________, University of British Columbia
 *
 *  This file contains the prototypes for the switch 
 *  routines that you will create.  In addition,
 *  you can add any type information or anything else
 *  to this file that you want.  All the material you
 *  add for Assignment 2 should be in this file and switch.c.
 *  You won't need to modify any of the other other files.
 *
 ******************************************************/

#define SIZE 100

typedef struct voq_buffer_struct {
	pthread_mutex_t mutex;
	packet_t buffer[SIZE];
} VOQBUFFER;
	

void switch_init();

void switch_add_entry(ip_address_t *address,
                      int port);

void *switch_thread_routine(void *arg);

void switch_free();      

int get_port_from_packet(packet_t* packet);    

void forward_packet_to_port(packet_t packet);            

void add_packet_to_buffer(packet_t network_buffer[], packet_t packet);

packet_t get_packet_from_buffer(packet_t network_buffer[]);

void add_packet_to_voq(int input_port, int destination_port, packet_t* packet);

packet_t get_packet_from_voq(int input_port, int destination_port);

void *read_in_port_packet(void* p);

void *read_out_port_packet(void* p);
