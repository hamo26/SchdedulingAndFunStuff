#include "/home/cclinus/workplace/494/hw2/nano-RK/src/kernel/include/nrk.h"
#include <include.h>
#include <ulib.h>
#include <stdio.h>

int main(){
    nrk_setup_ports();
    printf("Starting ... \n");
    nrk_init();
    nrk_time_set(0,0);
    nrk_create_taskset();
    nrk_start();
    return 0;
}
