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
    packet_copy(&packet, &(out_port[destination].packet));
    printf("Forwarding packet: "); packet_print(&packet);
    out_port[destination].flag = TRUE;
}

/* Gets a port number from a packet*/
int get_port_from_packet(packet_t* packet) {
    return cam_lookup_address(&(packet->address));
}

void add_packet_to_buffer(packet_t* packet){
  packet_copy(packet,&(network_buffer[rear]));
  rear = (rear+1) % SIZE;
  size++;
}

packet_t get_packet_from_buffer(){
  packet_t packet;
  packet_copy(&(network_buffer[front]),&packet);
  front= (front+1) % SIZE;
  size--;
  return packet;
}
 
void *read_in_port_packet(void* p)
{
	while(1){
		int i;
	
		if (die_flag == 0) { 
			std::cout<<"DIE is true now \n";
			pthread_exit(NULL);
		}

		for (i = 0; i < 4; i++) {
			//printf("Reading from port %d\n",i);

			if (in_port[i].flag == TRUE) {
				port_lock(&(in_port[i]));
				printf("Adding data to buffer from port %d\n",i);
				pthread_mutex_lock(&region_mutex);
				if (size == SIZE) {	
					printf("The buffer is full\n");
				       	printf("Thread in locked\n");
					pthread_cond_wait(&space_available,&region_mutex);
					printf("Thread in unlocked\n");
				}
				add_packet_to_buffer(&(in_port[i].packet));
				pthread_cond_signal(&data_available);
				pthread_mutex_unlock(&region_mutex);
				//Add logic here to read from network port.
				in_port[i].flag = FALSE;
				port_unlock(&(in_port[i]));
			}
			
		}

    }
}

void *read_out_port_packet(void* p)
{
   while (1) {
	   
	   packet_t packet;
	   int dest_port;
	   BOOL flag = TRUE;
//	   printf("Attempting to get lock for regional mutex in out thread\n");
	   pthread_mutex_lock(&region_mutex);
//	   printf("Successfuly retrieved lock for regional mutex in out thread\n");
           if (size == 0) {
           	printf("Thread out locked.\n");
                pthread_cond_wait(&data_available,&region_mutex);
                printf("Thread out unlocked.\n");
           }

           packet = get_packet_from_buffer();
           pthread_cond_signal(&space_available);
           pthread_mutex_unlock(&region_mutex);
           
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
