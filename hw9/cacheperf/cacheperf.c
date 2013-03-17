#include <stdio.h>
#include "timer.h"

/* try different sizes here */
#define SQSIZE 2048

struct point_color 
{
    int c;
    int m;
    int y;
    int k;
};

struct point_color square[SQSIZE][SQSIZE];

int main()
{
    int i,j;
    unsigned time1, time2, time3;
    start_counter();
    for (i = 0; i < SQSIZE; i++)
    {
	for (j = 0; j < SQSIZE; j++)
	{
	    square[i][j].c = 0;
	    square[i][j].m = 0;
	    square[i][j].y = 1;
	    square[i][j].k = 0;
	}
    }
    time1 = get_counter();

    start_counter();
    for (i = 0; i < SQSIZE; i++)
    {
	for (j = 0; j < SQSIZE; j++)
	{
	    square[i][j].c = 0;
	    square[i][j].m = 0;
	    square[i][j].y = 1;
	    square[i][j].k = 0;
	}
    }
    time2 = get_counter();
    
    start_counter();
    for (i = 0; i < SQSIZE; i++)
    {
	for (j = 0; j < SQSIZE; j++)
	{
	    square[j][i].c = 0;
	    square[j][i].m = 0;
	    square[j][i].y = 1;
	    square[j][i].k = 0;
	}
    }
    time3 = get_counter();
    printf("T1 (Cold cache time):    %12u  (%.1fx)\n", time1,(double)time1/time2);
    printf("T2 (Warm cache time):    %12u  (1.0x)\n", time2);
    printf("T3 (Poor locality time): %12u  (%.1fx)\n", time3,(double)time3/time2);
}

