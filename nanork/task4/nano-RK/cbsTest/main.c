#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <hal.h>
#include <nrk_error.h>
#include <nrk_timer.h>
#include <nrk_stack_check.h>

/*
   To stay inline (no pun intended) with the nano-rk direction of using guards for 
   optimization and potential performance, we use macros rather than methods.
   We could have created a method, but the call may be expensive and impede the performance of the kernel.
 */
#define TASK(n, taskPeriod, taskExecution)                              \
    void task_##n##_func()                                              \
{                                                                       \
    int task_ID =  nrk_cur_task_TCB->task_ID;				\
    int k=0;								\
    while (1)                                                           \
    {									\
	printf("\n------\nStart running task..%d\n", n);            \
	if(n==2){							\
	    for(long i=0;i<=30000;i++){				\
		if(i%10000==0){					\
		printf("CBS budget is %d\n", nrk_task_TCB[task_ID].cpu_remaining);\
		    for(int j=0;j<9999;j++){k++;}			\
		    printf("Busying with task 2(CBS) %d out of 30000\n",i);\
		}							\
	    }							\
	}else{for(int i=0;i<10;i++){k++;}printf("Some busy tasks\n");}	\
	printf("Finishing the task %d\n------\n\n",n);		\
	nrk_wait_until_next_period();				\
    }                                                      \
}                                                                   \
NRK_STK stack_##n[NRK_APP_STACKSIZE];                                   \
nrk_task_type task_##n;                                                 \
uint32_t task_##n##_period = taskPeriod;                               	\
uint32_t task_##n##_execution = taskExecution;                         	\

/*"Instantiate" the task*/
#define INITIALIZE_TASK(n,task_type)                                    \
task_##n.FirstActivation = TRUE;                                        \
task_##n.prio = n;							\
task_##n.Type = task_type;                                             \
task_##n.SchType = PREEMPTIVE;                                          \
task_##n.period.secs = task_##n##_period;                               \
task_##n.period.nano_secs = 0;                                          \
task_##n.cpu_reserve.secs = task_##n##_execution;                      \
task_##n.cpu_reserve.nano_secs = 0;                                     \
task_##n.offset.secs = 0;                                               \
task_##n.offset.nano_secs = 0;                                          \
nrk_task_set_entry_function(&task_##n, task_##n##_func);            \
nrk_task_set_stk(&task_##n, stack_##n, NRK_APP_STACKSIZE);              \
nrk_activate_task(&task_##n)						

//"Instantiate" tasks.
TASK(1, 6, 5);
TASK(2, 7, 1);
//TASK(3, 7, 1);

int main ()
{
    nrk_setup_ports();
    nrk_setup_uart(UART_BAUDRATE_115K2);

    nrk_init();

    nrk_time_set(0,0);

    //Initialize tasks 
    //Higher value higher priority`
    INITIALIZE_TASK(1, BASIC_TASK);
    INITIALIZE_TASK(2, CBS_TASK);
   // INITIALIZE_TASK(3, BASIC_TASK);

    nrk_start();

    return 0;
}
