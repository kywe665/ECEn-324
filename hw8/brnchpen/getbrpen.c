/* jka: Created 14 Feb 2011. This file contains code to evaluate the
 * misprediction penalty in modern CPUs. Two large arrays are filled
 * with numerical values -- in one scenario, the values always have a
 * consistent relationship; in the second scenarior, the values are
 * chosen randomly so they have no consistent relationship. Pairwise,
 * the values are passed as parameters to an absdiff() function. In
 * the first scenario, the branch in the absdiff() function will be
 * very predictable; in the second scenario it will NOT be
 * predictable. The difference in timing can be used to compute the
 * misprediction penalty.
 */

#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

/* CMIN is the minimum measurement interval in cycles. Should be long
 * enough to make measurement overhead negligible, but short enough to
 * reduce likelihood of capturing a portion of a context switch. The
 * display_CPUspeed() function outputs estimated CPU clock frequency
 * (and # clocks in a 10ms tick interval) as a sanity check. 
 */
#define CMIN 1000000   /* 1M cycles */

#define MEASMAX  20     /* number of runs to make to get single
			 * trustworthy measurement */
#define THRESHOLD 0.005 /* max difference we'd like to see between
			 * timings of top 3 runs */


/* cycle is array holding list of times for repeated runs for given
 * length */
unsigned cycle[MEASMAX];
int cindex;   /* index to cycle[] */

/* arrays of pairs of numbers to use as parameters*/
int *valA, *valB;

/* output list of timings in cycle[] */
void dumpcycle()
{
    int i;
    printf("cycle: ");
    for (i = 0; i < cindex; i++)
	printf("%u ",cycle[i]);
    printf("\n");
}

/* sorts the cycle array of timings, smallest first */
void sortcycle(void)
{
    int i, done;
    do {
	done = 1;
	for (i = 1; i < cindex; i++)
	{
	    if (cycle[i-1] > cycle[i])
	    {
		unsigned tmp = cycle[i];
		cycle[i] = cycle[i-1];
		cycle[i-1] = tmp;
		done = 0;
	    }
	}
	
    } while (!done) ;
}

/* allocate space for arrays */
void allocarrays(int size)
{
    int bytecount = size * sizeof(int);
    if ((valA = (int *) malloc(bytecount)) == NULL)
    {
	fprintf(stderr,"Malloc failed\n");
	exit(-1);
    }
    if ((valB = (int *) malloc(bytecount)) == NULL)
    {
	fprintf(stderr,"Malloc failed\n");
	exit(-1);
    }
}

/* init arrays for scenario 1: valA[i] < valB[i] */
void initarrays1(int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
	valA[i] = 0;
	valB[i] = 10;
    }
}

/* init arrays for scenario 2: random values all around */
void initarrays2(int size)
{
    int i;
    srand48(0L);
    for (i = 0; i < size; i++)
    {
	valA[i] = (int) mrand48();
	valB[i] = (int) mrand48();
    }
}

/* reset the list of timings to empty */
void clearlist(void)
{
    cindex = 0;
}

/* Output best time, and flag instances where best time and 3rd best
 * time differ by a relative factor greater than THRESHOLD. If we've
 * done enough runs, the best 3 times should be very close. This
 * alerts us if this becomes a problem. */
void testbest()
{
    printf("Best time = %u\n", cycle[0]);
    if ((((double) cycle[2] - cycle[0]) / cycle[2]) > THRESHOLD)
    {
	printf("  Difference exceeds threshold: ");
	printf("third best = %u, diff: %.2f%%\n", cycle[2], 100.0 * 
	       ((double) cycle[2] - cycle[0]) / cycle[2]);
    }
}

/* add another timing to list */
void addtolist(unsigned time)
{
    cycle[cindex++] = time;
    if (cindex > MEASMAX)
    {
	fprintf(stderr,"fatal error: overflowed cycle array\n");
	exit(-1);
    }
    sortcycle();
    /* dumpcycle();  for debugging */
}

/* The function we want to measure */
int absdiff(int x, int y)
{
    if (x > y)
	return x-y;
    else
	return y-x;
}

/* repeatedly calls the function to measure until the total execution
 * time (in cycles) is above specified threshold, returns iteraction count */
int measurecnt(void)
{
    unsigned cmeas, cycles;
    int cnt = 1;
    volatile int res = 0;  /* don't let compiler optimize away fn
			      calls */
    do 
    {
	int c = cnt;
	res += absdiff(0,1);    /* first call to warm up cache */
	start_counter();
	while (c-- > 0)
	    res += absdiff(0,1);
	cmeas = get_counter();
	cycles = cmeas / cnt;
	cnt += cnt;
    } while (cmeas < CMIN);  /* make sure long enough */
    return cnt;
}

/* repeatedly calls the function to measure -- argument is iteration
 * count */ 
unsigned measure(int iter)
{
    int i;
    unsigned cmeas,cycles;
    volatile int res = 0;  /* don't let compiler optimize away fn
			    * calls */
    
    res += absdiff(0,1);  /* warm up cache */
    start_counter();
    for (i = 0; i < iter; i++)
	res += absdiff(valA[i],valB[i]);
    cmeas = get_counter();
    cycles = cmeas / iter;
    return cycles;
}

/* Steps in program: determine the number of iterations of desired
 * function required to get a measurement interval of the desired
 * length, allocate vectors of that size, initialize to appropriate
 * values for both the related and random scenarios, make the runs,
 * output the results. */
main()
{
    int i,itercount;
    unsigned ccount;
    
    /* The next two lines are just a sanity check -- particularly
       useful when trying on a new platform. Can comment out when
       doing repeated runs thereafter. */
    /* display_CPUspeed();
    printf("Cycle threshold for measurements: %.2f (millions)\n\n", 
    (double) CMIN / 1e6);  */

    itercount = measurecnt();
    printf("Measuring using %d iterations\n",itercount);

    /* allocate arrays for both scenarios */
    allocarrays(itercount);
    
    /* initialize arrays for scenario 1: valA[i] < valB[i] */
    initarrays1(itercount);
    
    /* do multiple runs for this scenario, recording each */
    clearlist();
    for (i = 0; i < MEASMAX; i++)
    {
	ccount = measure(itercount);
	addtolist(ccount);
    }

    /* test and output best */
    printf("Cycles per function call, predictable branches\n");
    testbest();

    /* initialize arrays for scenario 2: valA[i] ? valB[i] */
    initarrays2(itercount);
    
    /* do multiple runs for this scenario, recording each */
    clearlist();
    for (i = 0; i < MEASMAX; i++)
    {
	ccount = measure(itercount);
	addtolist(ccount);
    }

    /* test and output best */
    printf("Cycles per function call, unpredictable branches\n");
    testbest();
}
