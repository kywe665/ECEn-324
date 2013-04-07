#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "timer.h"

char *shared;
char *string = "This is the original file.";

int main()
{
    pid_t pid;
    int fd;
    struct stat stat;
    double cycles;
    
    /* create shared data file and set permissions */

    fd = open("shared.dat", O_WRONLY|O_CREAT,0600);
    if (fd == -1)
    {
	fprintf(stderr, "open failed\n");
	exit(-1);
    }
    write(fd, string, strlen(string)+1);
    close(fd);
    
    if ((pid = fork()) == 0)
    {   /* child */
	fd = open("shared.dat", O_RDWR);
	fstat(fd,&stat);   /* get file info (including size) */
	start_counter();   /* time mmap call */
	shared = mmap(NULL, stat.st_size, PROT_WRITE, MAP_SHARED, fd, 0);
	cycles = dget_counter();
	if (shared == MAP_FAILED)
	{
	    fprintf(stderr, "mmap failed in child\n");
	    exit(-1);
	}
	printf("Original string seen by child:\t\t%s\n",shared);
	strncpy(shared,"It read",7);
	printf("Modified string seen by child:\t\t%s\n",shared);
	sleep(1);
	printf("Final string seen by child:\t\t%s\n",shared);
	printf("Overhead of mmap: %d cycles (child)\n",(int) cycles);
    }
    else
    {   /* parent */
	fd = open("shared.dat", O_RDWR);
	fstat(fd,&stat);  /* get file info (including size) */
	start_counter();  /* time mmap call */
	shared = mmap(NULL, stat.st_size, PROT_WRITE, MAP_SHARED, fd, 0);
	cycles = dget_counter();
	if (shared == MAP_FAILED)
	{
	    fprintf(stderr, "mmap failed in parent\n");
	    exit(-1);
	}
	printf("Original string seen by parent:\t\t%s\n",shared);
  sleep(1);
	strncpy(shared+21,"data",4);
	printf("Modified string seen by parent:\t\t%s\n",shared);
	sleep(1);
	printf("Final string seen by parent:\t\t%s\n",shared);
	printf("Overhead of mmap: %d cycles (parent)\n",(int) cycles);
    }
}
