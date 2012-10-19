#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <hal.h>
#include <nrk_error.h>
#include <nrk_timer.h>
#include <nrk_stack_check.h>
#include <nrk_stats.h>

/*
To stay inline (no pun intended) with the nano-rk direction of using guards for 
optimization and potential performance, we use macros rather than methods.
We could have created a method, but the call may be expensive and impede the performance of the kernel.
*/
#define TASK(n, taskPeriod, taskExecution)                                \
    void task_##n##_func()                                                  \
    {                                                                       \
                                                                            \
        while (1)                                                           \
        {									\
			nrk_wait_until_next_period();	    \
			printf("Running task..%d\n", n);                                	\
        }                                                                   \
    }                                                                       \
    NRK_STK stack_##n[NRK_APP_STACKSIZE];                                   \
    nrk_task_type task_##n;                                                 \
    uint32_t task_##n##_period = task_period;                               \
    uint32_t task_##n##_execution = task_execution;                         \
	task_##n.prio = 10 - n;                                                      \
    task_##n.FirstActivation = TRUE;                                        \
    task_##n.Type = BASIC_TASK;                                             \
    task_##n.SchType = PREEMPTIVE;                                          \
    task_##n.period.secs = task_##n##_period;                               \
    task_##n.period.nano_secs = 0;                                          \
    task_##n.cpu_reserve.secs = task_##n##_execution;                       \
    task_##n.cpu_reserve.nano_secs = 0;                                     \
    task_##n.offset.secs = 0;                                               \
    task_##n.offset.nano_secs = 0;                                          \
    
/*Initialize the task*/
#define INITIALIZE_TASK(n)                                                    \
    nrk_task_set_entry_function(&task_##n, task_##n##_func);                \
    nrk_task_set_stk(&task_##n, stack_##n, NRK_APP_STACKSIZE);              \
    nrk_activate_task(&task_##n)

//"Instantiate" tasks.
TASK(1, 4, 2);
TASK(2, 7, 3);


int main ()
{
    nrk_setup_ports();
    nrk_setup_uart(UART_BAUDRATE_115K2);

    nrk_init();

    nrk_time_set(0,0);
	
	//Initialize tasks 
    INITIALIZE_TASK(1);
    INITIALIZE_TASK(2);

    nrk_start();

    return 0;
}
