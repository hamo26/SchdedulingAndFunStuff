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
 *******************************************************************************/


#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <hal.h>
#include <nrk_error.h>
#include <nrk_timer.h>
#include <nrk_stack_check.h>


NRK_STK Stack1[NRK_APP_STACKSIZE];
nrk_task_type TaskOne;
void Task1(void);

NRK_STK Stack2[NRK_APP_STACKSIZE];
nrk_task_type TaskTwo;
void Task2 (void);

/*NRK_STK Stack3[NRK_APP_STACKSIZE];
  nrk_task_type TaskThree;
  void Task3 (void);*/


void nrk_create_taskset();

int main ()
{

    nrk_setup_ports();
    nrk_setup_uart(UART_BAUDRATE_115K2);
    nrk_kprintf( PSTR("EDF Test: \n") );

    nrk_init();

    nrk_time_set(0,0);
    nrk_create_taskset ();
    nrk_start();
    return 0;
}

void Task1()
{
    while(1) {
	nrk_kprintf( PSTR("In T1 loop \n") );
	nrk_wait_until_next_period();
    }
}

void Task2()
{
    while(1) {
	nrk_kprintf( PSTR("In T2 loop \n") );
	nrk_wait_until_next_period();
    }
}


void nrk_create_taskset()
{
    TaskOne.task = Task1;
    TaskOne.Ptos = (void *) &Stack1[NRK_APP_STACKSIZE];
    TaskOne.Pbos = (void *) &Stack1[0];
    TaskOne.prio = 1;
    TaskOne.FirstActivation = TRUE;
    TaskOne.Type = BASIC_TASK;
    TaskOne.SchType = PREEMPTIVE;
    TaskOne.period.secs = 6;
    TaskOne.period.nano_secs = 20*NANOS_PER_MS;
    TaskOne.cpu_reserve.secs = 3;
    TaskOne.cpu_reserve.nano_secs =  5*NANOS_PER_MS;
    TaskOne.offset.secs = 0;
    TaskOne.offset.nano_secs= 0;
    nrk_activate_task (&TaskOne);

    TaskTwo.task = Task2;
    TaskTwo.Ptos = (void *) &Stack2[NRK_APP_STACKSIZE];
    TaskTwo.Pbos = (void *) &Stack2[0];
    TaskTwo.prio = 2;
    TaskTwo.FirstActivation = TRUE;
    TaskTwo.Type = BASIC_TASK;
    TaskTwo.SchType = PREEMPTIVE;
    TaskTwo.period.secs = 10;
    TaskTwo.period.nano_secs = 11*NANOS_PER_MS;
    TaskTwo.cpu_reserve.secs = 2;
    TaskTwo.cpu_reserve.nano_secs = 5*NANOS_PER_MS;
    TaskTwo.offset.secs = 0;
    TaskTwo.offset.nano_secs= 0;
    nrk_activate_task (&TaskTwo);

}
