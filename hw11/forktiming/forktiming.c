#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "timer.h"

/* The simple code that follows gives us some insight into (a) the
   overhead of the fork() function, and (b) the order in which the OS
   schedules the two separate processes after the fork gets to the
   point that two processes exist. */

int main()
{
    double ctime,ptime,mhzest;
    pid_t pid;
    
    /* get estimate of mhz rating of cpu */
    mhzest = mhz();
    start_counter();
    if ((pid = fork()) == 0)
    {
	ctime = dget_counter();
	printf("Child:  %.8d ticks (%.2f ms)\n", (int) ctime, 
	       ctime/(mhzest * 1e3));
	exit(0);
    }
    else
    {
	ptime = dget_counter();
	printf("Parent: %.8d ticks (%.2f ms)\n", (int) ptime,
	       ptime/(mhzest * 1e3));
	waitpid(pid,NULL,0);
    }
}
