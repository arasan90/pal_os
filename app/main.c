#include <stdio.h>

#include "pal_os/thread.h"
int app_main(void)
{
	pal_thread_t *thread;
	pal_thread_create(&thread, PAL_THREAD_PRIORITY_NORMAL, 1024, NULL, "TestThread", NULL);
	printf("Hello, World!\n");
	return 0;
}