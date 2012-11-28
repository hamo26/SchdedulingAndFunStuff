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

#define SIZE 100

pthread_mutex_t region_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t space_available = PTHREAD_COND_INITIALIZER;
pthread_cond_t data_available = PTHREAD_COND_INITIALIZER;

packet_t network_buffer[SIZE];  
int size = 0;  /* number of full elements */
int front,rear=0;  /* queue */

void *switch_thread_routine(void *arg)
{
        pthread_t in_port_thread; 
	pthread_t out_port_thread; 
	
	pthread_create(&in_port_thread,NULL,read_in_port_packet,NULL);
	pthread_create(&out_port_thread,NULL,read_out_port_packet,NULL);
	pthread_join(in_port_thread,NULL);
	pthread_exit(NULL);
}

void forward_packet_to_port(packet_t packet, int destination) {
    std::cout<<"dest_port:"<<destination<<"\n";
    out_port[destination].packet = packet;
    out_port[destination].flag = TRUE;
    printf("Packet delivered\n");
}

/* Gets a port number from a packet*/
int get_port_from_packet(packet_t* packet) {
    return cam_lookup_address(&(packet->address));
}

void add_packet_to_buffer(packet_t packet){
  network_buffer[rear] = packet;
  rear = (rear+1) % SIZE;
  size++;
}

packet_t get_packet_from_buffer(){
  packet_t packet;
  packet = network_buffer[front];
  front= (front+1) % SIZE;
  size--;
  return packet;
}
 
void *read_in_port_packet(void* p)
{
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
			printf("I am here.\n");
			current_input_port = in_port[i];
			pthread_mutex_lock(&current_input_port.mutex);
			if (current_input_port.flag == TRUE) {
				pthread_mutex_lock(&region_mutex);
				if (size == SIZE) {	
					printf("The buffer is full\n");
				       	printf("Thread in locked\n");
					pthread_cond_wait(&space_available,&region_mutex);
					printf("Thread in unlocked\n");
				}
				add_packet_to_buffer(current_input_port.packet);
				pthread_cond_signal(&data_available);
				pthread_mutex_unlock(&region_mutex);
				//Add logic here to read from network port.
				current_input_port.flag = FALSE;
				pthread_mutex_unlock(&current_input_port.mutex);
			}
		}

    }
}

void *read_out_port_packet(void* p)
{
   packet_t packet;
   int dest_port;
   while (1) {
	   pthread_mutex_lock(&out_port[dest_port].mutex);
	   if (out_port[dest_port].flag != TRUE) {
		printf("Attempting to get mutext for out thread\n");
		 pthread_mutex_lock(&region_mutex);
		 if (size == 0) {
                   printf("Thread out locked.\n");
                   pthread_cond_wait(&data_available,&region_mutex);
                   printf("Thread out unlocked.\n");
	         }

		packet = get_packet_from_buffer();
		pthread_cond_signal(&space_available);
		pthread_mutex_unlock(&region_mutex);
		dest_port = get_port_from_packet(&packet);
		forward_packet_to_port(packet, dest_port);
	   } 
	   pthread_mutex_unlock(&out_port[dest_port].mutex);
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
