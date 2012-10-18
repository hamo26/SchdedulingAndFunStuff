#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <hal.h>
#include <nrk_error.h>
#include <nrk_timer.h>
#include <nrk_stack_check.h>
#include <nrk_stats.h>

#define CHECK_PERIOD    200
#define LEEWAY_FACTOR   4/5

#define TASK(n, task_period, task_execution)                                \
    void task_##n##_func()                                                  \
    {                                                                       \
                                                                            \
        while (1)                                                           \
        {									\
	nrk_wait_until_next_period();	    \
	printf("Beginning task %d\n", n);                                	\
        }                                                                   \
    }                                                                       \
    NRK_STK stack_##n[NRK_APP_STACKSIZE];                                   \
    nrk_task_type task_##n;                                                 \
    uint32_t task_##n##_period = task_period;                               \
    uint32_t task_##n##_execution = task_execution;                         \

#define ACTIVATE_TASK(n)                                                    \
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
    nrk_task_set_entry_function(&task_##n, task_##n##_func);                \
    nrk_task_set_stk(&task_##n, stack_##n, NRK_APP_STACKSIZE);              \
    nrk_activate_task(&task_##n)

TASK(1, 4, 2);
TASK(2, 7, 3);
//TASK(3, 8, 3);

int main ()
{
    nrk_setup_ports();
    nrk_setup_uart(UART_BAUDRATE_115K2);

    nrk_init();

    nrk_led_clr(ORANGE_LED);
    nrk_led_clr(BLUE_LED);
    nrk_led_clr(GREEN_LED);
    nrk_led_clr(RED_LED);

    nrk_time_set(0,0);

    ACTIVATE_TASK(1);
    ACTIVATE_TASK(2);
 //   ACTIVATE_TASK(3);

    nrk_start();

    return 0;
}
