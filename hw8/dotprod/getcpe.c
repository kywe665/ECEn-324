/* jka: Created 8 Feb 2011. This file contains driver functions that
 * call vector operations (in vector.c) while measuring their
 * execution time (using routines from timer.c) The goal is to redo
 * the sequence of optimizations CS:APP2 Chapter 5 (showing the
 * performance benefits at each point) for a dot product function
 * (rather than the combine code used in the text). 
 */

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "timer.h"

/* CMIN is the minimum measurement interval in cycles. Should be long
 * enough to make measurement overhead negligible, but short enough to
 * reduce likelihood of capturing a portion of a context switch. The
 * display_CPUspeed() function outputs estimated CPU clock frequency
 * (and # clocks in a 10ms tick interval) as a sanity check. 
 */
#define CMIN 1000000   /* 1M cycles */

/* the next two values must be kept consistent; if one is changed, you
 * should change both */
#define VECVALS  10     /* unique vector lengths tested */
#define VECMAX 1024     /* max vector length for loop runs */

#define MEASMAX  20     /* number of runs to make for each vec length */
#define THRESHOLD 0.005 /* max difference we'd like to see between
			 * timings of top 3 runs */

vec_ptr V1,V2;
double result;

/* cycle is array holding list of times for repeated runs for given
 * vector length */
unsigned cycle[MEASMAX];
int cindex;   /* index to cycle[] */

/* master is array of single best time for each vector length */
unsigned master[VECVALS];
int mindex;  /* index to master[] */

unsigned vlen[VECVALS];  /* actual vector length at each index */


/* output list of timings in cycle[] */
void dumpcycle()
{
    int i;
    printf("cycle: ");
    for (i = 0; i < cindex; i++)
	printf("%u ",cycle[i]);
    printf("\n");
}

/* output list of timings in master[] */
void dumpmaster()
{
    int i;
    printf("master: ");
    for (i = 0; i < mindex; i++)
	printf("%u ",master[i]);
    printf("\n");
}

/* output vector lengths in vlen[] */
void dumpvlen()
{
    int i;
    printf("vlen: ");
    for (i = 0; i < VECVALS; i++)
	printf("%u ",vlen[i]);
    printf("\n");
}

void dumpdatapts()
{
    int i;
    printf("data points: \n");
    for (i = 0; i < VECVALS; i++)
	printf("  %5u,%5u\n", vlen[i], master[i]);
}

/* The function lsfit() does a linear least-squares fit on the
 * measurements from each vector length. Equation for a
 *  straight line is given by 
 *	 y = mx + b
 *  where m is the slope of the line and b is the y-intercept.
 *
 *  Given a set of n points {(x1,y1), x2,y2),...,xn,yn)}, let
 *      sumx = x1 + x2 + ... + xn
 *      sumy = y1 + y2 + ... + yn
 *      sumxy = x1*y1 + x2*y2 + ... + xn*yn
 *      sumxx = x1*x1 + x2*x2 + ... + xn*xn
 *
 *  The slope and y-intercept for the least-squares line can be 
 *  calculated using the following equations:
 *        slope (m) = ( sumx*sumy - n*sumxy ) / ( sumx*sumx - n*sumxx ) 
 *  y-intercept (b) = ( sumy - slope*sumx ) / n
 *
 * 
 */
void lsfit(void) 
{
    double sumx, sumy, sumxy, sumxx;
    double slope, y_intercept;
    int i;
    
    sumx = sumy = sumxy = sumxx = 0.0;
    for (i = 0; i < VECVALS; i++)
    {
	double x = (double) vlen[i];
	double y = (double) master[i];
	sumx += x;
	sumy += y;
	sumxy += x * y;
	sumxx += x * x;
    }
    slope = (sumx * sumy - VECVALS * sumxy) / (sumx * sumx - VECVALS * sumxx);
    y_intercept = (sumy - slope * sumx) / VECVALS;

    printf("Measured CPE for current function: %.1f\n", slope);
    
    /* more verbose output for debugging 
    printf ("The linear equation that best fits the given data:\n");
    printf ("       y = %6.2lfx + %6.2lf\n", slope, y_intercept); */
    /* dumpdatapts();  for debugging */
}

/* initialize global vectors V1,V2 for subsequent use */
void setupvectors(int len)
{
    int i;
    V1 = new_vec(len);
    V2 = new_vec(len);
    for (i = 0; i < len; i++)
    {
	set_vec_element(V1, i, i+1);   /* arbitrary values */
	set_vec_element(V2, i, len-i);
    }
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

/* reset the list of timings to empty */
void clearlist(void)
{
    cindex = 0;
}

/* reset the master list to empty */
void clearmaster(void)
{
    mindex = 0;
}



/* sanity check: flag instances where best time and 3rd best time
 * differ by a relative factor greater than THRESHOLD. Hopefully
 * we've done enough runs that best 3 times are very close. This
 * alerts us if this becomes a problem. */
void testbest()
{
    if ((((double) cycle[2] - cycle[0]) / cycle[2]) > THRESHOLD)
    {
	printf("Result difference exceeds threshold: ");
	printf("Best time = %u, third best = %u, diff: %.2f%%\n", 
	       cycle[0], cycle[2], 100.0 * 
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

/* add current best timing in cycle[] to master[] */
void addtomaster(void)
{
    master[mindex++] = cycle[0];
    if (mindex > VECVALS)
    {
	fprintf(stderr,"fatal error: overflowed master array\n");
	exit(-1);
    }
}

/* initialize vlen array to vector lengths used */
void initvlen(void)
{
    int i, j;
    j = 0;
    for (i = 2; i <= VECMAX; i = i << 1)
	vlen[j++] = i;
    /* dumpvlen();  for debugging */
}


/* the basic function(s) we want to measure */
/* Do dot product of two vectors, abstract version */
void dotproduct1(vec_ptr u, vec_ptr v, data_t *dest)
{
    long int i;
    *dest = 1.0;
    for (i = 0; i < vec_length(u); i++) {
	    data_t val1;
	    data_t val2;
	    get_vec_element(u, i, &val1);
	    get_vec_element(v, i, &val2);
	    *dest = *dest + val1 * val2;
    }
}

/* repeatedly calls the function to measure until the total execution
 * time (in cycles) is above specified threshold */
unsigned measure(void)
{
    unsigned cmeas, cycles;
    int cnt = 1;
    do 
    {
	int c = cnt;
	dotproduct1(V1,V2,&result);    /* first call to warm up cache */
	start_counter();
	while (c-- > 0)
	    dotproduct1(V1,V2,&result);
	cmeas = get_counter();
	cycles = cmeas / cnt;
	cnt += cnt;
    } while (cmeas < CMIN);  /* make sure long enough */
    return cycles;
    
    /* printf("Measured function required %u clock cycles (per call)\n", cycles);
       printf("  cnt = %d, cmeas = %u\n", cnt, cmeas); */
}

/* driver for everything: for current vector and operation types,
 * run through entire range of vector lengths, making repeated
 * measurements of each, then determine slope of line that best
 * matches data, and finally output results as CPE. */
main()
{
    int i,j;
    unsigned ccount;
    
    /* The next two lines are just a sanity check -- particularly
       useful when trying on a new platform. Can comment out when
       doing repeated runs thereafter. */
    /* display_CPUspeed();
    printf("Cycle threshold for measurements: %.2f (millions)\n\n", 
    (double) CMIN / 1e6);  */

    clearmaster();
    initvlen();
    for (i = 2; i <= VECMAX; i = i << 1)
    {
	/* printf("Vector length: %d\n",i);  for debugging */
	setupvectors(i);
	clearlist();
	/* do multiple runs for this vector length, recording each */
	for (j = 0; j < MEASMAX; j++)
	{
	    ccount = measure();
	    addtolist(ccount);
	}
	testbest();
	addtomaster();  /* record best time in master list */
    }
    /* dumpmaster();      for debugging */
    lsfit();	       /* do linear fit */
}


