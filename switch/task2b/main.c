

/*******************************************************
 *
 *  Main routine for EECE 494, Assignment 2
 *
 *  Sathish Gopalakrishnan & Steve Wilton
 *  University of British Columbia
 *
 ******************************************************/

#include "defs.h"

/* Shared variables among all threads */

port_t in_port[4];
port_t out_port[4];
BOOL die;
int cell_flag = 0;
double accum = 0;
double countPackets = 0;
double sleepDelay = 0;

main()
{
   int i,status;
   pthread_t switch_thread_id;
   pthread_t harness_thread_id;

   printf("Initializing ports\n");

   for(i=0;i<4;i++) {
      port_init( &(in_port[i]));
      port_init( &(out_port[i]));
   }

   printf("Initializing switch\n");

   switch_init();

   printf("Initializing harness\n");
  
   harness_init();

   die = FALSE;

   printf("Starting switch thread\n");
   status = pthread_create(&switch_thread_id,
                           NULL,
                           switch_thread_routine,
                           NULL);

   if (status != 0) {
      printf("Error creating switch thread\n");
      exit(0);
   }

   status = pthread_detach(switch_thread_id);
   if (status != 0) {
      printf("Error detaching switch thread\n");
      exit(0);
   }


   printf("Starting harness thread\n");
   status = pthread_create(&harness_thread_id,
                           NULL,
                           harness_thread_routine,
                           NULL);

   if (status != 0) {
      printf("Error creating harness thread\n");
      exit(0);
   }

   status = pthread_detach(harness_thread_id);
   if (status != 0) {
      printf("Error detaching harness thread\n");
      exit(0);
   }

   while (!die) {
      sleep(1);
   }

   printf("Main program is now done\n");
	sleepDelay = countPackets*HARNESS_SPEED*2 / 1000000000.0;
	printf("Total accumulated packet delays: %f seconds\n",accum);
	printf("Total packets: %f packets\n",countPackets);
	printf("Total sleep delay: %f nano seconds \n",sleepDelay);
	printf("Average Delay without sleep: %f seconds per packet \n",(accum - sleepDelay/1000000000) /countPackets);
	

   harness_end();   
   switch_free();   
}
