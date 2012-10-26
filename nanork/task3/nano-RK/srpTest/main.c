/******************************************************************************
 *  Nano-RK, a real-time operating system for sensor networks.
 *  Copyright (C) 2007, Real-Time and Multimedia Lab, Carnegie Mellon University
 *  All rights reserved.
 *
 *  This is the Open Source Version of Nano-RK included as part of a Dual
 *  Licensing Model. If you are unsure which license to use please refer to:
 *  http://www.nanork.org/nano-RK/wiki/Licensing
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2.0 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contributing Authors (specific to this file):
 *  Zane Starr
 *******************************************************************************/

#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <hal.h>
#include <nrk_error.h>
#include <nrk_events.h>
#include <nrk_timer.h>


/*
   To stay inline (no pun intended) with the nano-rk direction of using guards for 
   optimization and potential performance, we use macros rather than methods.
   We could have created a method, but the call may be expensive and impede the performance of the kernel.
 */
#define TASK(n, taskPeriod, taskExecution)                                      \
    	NRK_STK stack_##n[NRK_APP_STACKSIZE];                               	\
	nrk_task_type task_##n;                                                 \
	uint32_t task_##n##_period = taskPeriod;                               	\
	uint32_t task_##n##_execution = taskExecution;                         	\
	void task_##n##_activity(void)                                              \
	{									\
	    uint16_t cnt;                                                       \
	    int8_t v;								\
	    printf( "My node's address is %d\r\n",NODE_ADDR );			\
	    printf( "Task%d PID=%d\r\n", n, nrk_get_pid());			\
	    cnt=0;								\
	    while(1) {								\
		printf( "Task%d has held resource %d times\r\n", n, cnt );	\
		printf("Task%d attempting to access semaphore\r\n", n);		\
		v = nrk_sem_pend(my_semaphore);					\
		if(v==NRK_ERROR) printf("T##n error pend\r\n");			\
		printf("Task%d holding semaphore\r\n", n);			\
		nrk_wait_until_next_period();					\
		v = nrk_sem_post(my_semaphore);					\
		if(v==NRK_ERROR) printf("T%d error post\r\n", n);		\
		printf("Task%d released semaphore\r\n\n", n);			\
		nrk_wait_until_next_period();					\
		cnt++;								\
	    }									\
	}									\

/*"Instantiate" the task*/
#define INITIALIZE_TASK(n, total_tasks)						\
    task_##n.FirstActivation = TRUE;                                    \
task_##n.Ptos = (void *) &stack_##n[NRK_APP_STACKSIZE];			\
task_##n.Pbos = (void *) &stack_##n[0];					\
task_##n.Type = BASIC_TASK;                                             \
task_##n.task = task_##n##_activity;					\
task_##n.SchType = PREEMPTIVE;                                          \
task_##n.prio = 1;							\
task_##n.period.secs = task_##n##_period;                               \
task_##n.period.nano_secs = 0;                                          \
task_##n.cpu_reserve.secs = task_##n##_execution;                       \
task_##n.cpu_reserve.nano_secs = 0;                                     \
task_##n.offset.secs = 0;                                               \
task_##n.offset.nano_secs = 0;                                          \
nrk_activate_task(&task_##n)				

//Semaphore shared among resources.
nrk_sem_t *my_semaphore;

//"Instantiate" tasks.
TASK(1, 2, 0.5);
TASK(2, 3, 0.5);
TASK(3, 5, 0.5);

int main ()
{
    nrk_setup_ports();
    nrk_setup_uart(UART_BAUDRATE_115K2);

    printf( "Starting up...\r\n" );

    nrk_init();

    nrk_time_set(0,0);

    //Initialize tasks 
    INITIALIZE_TASK(1, 3);
    INITIALIZE_TASK(2, 3);
    INITIALIZE_TASK(3, 3);
    	
    //instead of passing the ceiling priority, the task with the shortest period that accesses the semaphore is given
    //in this case, task1 which has a period 350*NANOS_PER_MS
    my_semaphore = nrk_sem_create(1, 2);
    if(my_semaphore==NULL) nrk_kprintf( PSTR("Error creating sem\r\n" ));
    nrk_start();

    return 0;
}
