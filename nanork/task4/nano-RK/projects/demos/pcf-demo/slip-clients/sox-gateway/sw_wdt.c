#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sw_wdt.h>

#define WATCHDOG_SECONDS	10

long            wdt_cnt;

void* watchdog_thread()
{
	long            now;
	while (1) {
		now = time(NULL);
		if (now - wdt_cnt > WATCHDOG_SECONDS) {
			printf("Software Watchdog Expired, time to kill.\n");
			exit(0);
		}
		sleep(WATCHDOG_SECONDS / 3);
	}

}

void sw_watchdog_reset()
{
	wdt_cnt = time(NULL);
}

void sw_watchdog_init()
{
	pthread_t      *wdt_thread;
	pthread_attr_t  pthread_attr;

	wdt_cnt = time(NULL);
	wdt_thread = (pthread_t *) malloc(sizeof(*wdt_thread));
	pthread_attr_init(&pthread_attr);
	pthread_create(&wdt_thread, &pthread_attr, watchdog_thread, NULL);
}
/*
int main(void)
{
	int             i;

	sw_watchdog_init();

	for (i = 0; i < 3; i++) {
		sw_watchdog_reset();
		printf("reset sleeping\n");
		sleep(5);
	}
	printf("Long sleep, no reset\n");
	sleep(30);
	printf("done...\n");
	return 1;
}*/
