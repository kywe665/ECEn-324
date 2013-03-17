/* Created 27 Jan 2011.  This code contains start_counter() and
 * get_counter() functions that use the rdtsc instruction and return
 * the difference as a 32-bit unsigned. This code is therefore useful
 * only if the measurement interval is fairly short -- the target
 * length is much shorter than a timer tick: think 1 ms. If the
 * measurement interval is too long (and the counter value doesn't fit
 * in a 32-bit unsigned), get_counter() issues a fatal error message.
 *
 * Don't compile this file with -O3 or higher. Somehow the
 * optimizations mess up the values you get when reading the
 * counter. Maybe related to optimizations performed on
 * access_counter() which probably looks empty to the compiler.
 */

#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

static unsigned start_hi = 0;
static unsigned start_lo = 0;
static unsigned end_hi = 0;
static unsigned end_lo = 0;

/* the only function that includes the rdtsc instruction */
void access_counter(unsigned *hi, unsigned *lo)
{
    asm("rdtsc; movl %%edx,%0; movl %%eax,%1"   
	: "=r" (*hi), "=r" (*lo) 
	:
	: "%edx", "%eax");
}

/* records the counter value at start of interval */
void start_counter(void)
{
    access_counter(&start_hi, &start_lo);
}

/* return # cycles since start_counter as unsigned */
unsigned get_counter(void)
{
    access_counter(&end_hi, &end_lo);
    if (end_hi == start_hi)
	return end_lo - start_lo;
    else if (end_hi == start_hi+1 && end_lo < start_lo)
	return end_lo - start_lo;
    else
    {
	fprintf(stderr, "Cycle counter overflow: measurement interval too long\n");
	exit(-1);
    }
}

/* return # cycles since start_counter as double */
double dget_counter(void)
{
    unsigned hi, lo, borrow;
    double result;
    
    access_counter(&end_hi, &end_lo);
    lo = end_lo - start_lo;
    borrow = lo > end_lo;
    hi = end_hi - start_hi - borrow;
    result = (double) hi * (double) (1 << 30) * 4.0 + (double) lo;
    if (result < 0.0)
	fprintf(stderr, "Counter returned negative value in dget_counter()\n");
    return result;
}

/* output MHz rating of CPU, # cycles in 10ms tick interval */
void display_CPUspeed(void)
{
    int sleeptime = 1;
    double rate;

    start_counter();
    sleep(sleeptime);
    rate = dget_counter() / (1e9 * sleeptime); 
    
    printf("Estimated CPU clock rate: %.2f GHz\n", rate);
    printf("Estimated clocks between 10ms ticks: %.2f (millions)\n", rate * 10.0);
}

