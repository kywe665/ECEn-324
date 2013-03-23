/* mountain.c - Generate the memory mountain. */

#include <stdlib.h>
#include <stdio.h>
#include "timer.h"

#define MAXELEMS (1 << 26)
#define NUMSIZES   16
#define NUMSTRIDES 18
#define NUMRUNS    10

char fname[] = "mountain.res";

int stride[NUMSTRIDES] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,32,64 };

int size[NUMSIZES] = { 67108864, 33554432, 16777216, 8388608, 4194304,
			2097152, 1048576, 524288, 262144, 131072, 65536,
			32768, 16384, 8192, 4096, 2048 };

double best[NUMSIZES][NUMSTRIDES];  /* best results over several runs */

double data[MAXELEMS];      /* The global array we'll be traversing */

/* init_data initializes the array */
void init_data(double *data, int n)
{
    int i;
    for (i = 0; i < n; i++)
	data[i] = i;
}

/* initialize the array recording best times for each data point; set
 * to 0 initially, later values of read throughput will be higher */
void init_best(void)
{
    int i,j;
    for (i = 0; i < NUMSIZES; i++)
	for (j = 0; j < NUMSTRIDES; j++)
	    best[i][j] = 0.0;
}

/* Iterate over first "elems" elements of array "data" with stride of
 * "stride".  */
void test(int elems, int stride)
{
    int i;
    double result = 0.0; 
    volatile double sink; 
    for (i = 0; i < elems; i += stride) 
	result += data[i];  
    sink = result; /* So compiler doesn't optimize away the loop */
}

/* Run test(elems, stride) and return read throughput (MB/s). "size"
 * is in bytes, "stride" is in array elements, and Mhz is CPU clock
 * frequency in Mhz. */
double run(int size, int stride, double Mhz)
{   
    double cycles;
    int elems = size / sizeof(double);       

    test(elems, stride);                     /* warm up the cache */
    start_counter();
    test(elems, stride);                     /* measure this */
    cycles = dget_counter();
    return (size / stride) / (cycles / Mhz); /* convert cycles to MB/s */
}

int main()
{
    int i;
    int sizeindex;
    int strideindex;
    double Mhz;
    FILE *fp;
    
    fp = fopen(fname,"w");     /* open output file */
    if (fp == NULL)
    {
	fprintf(stderr,"Can't open output file: %s\n",fname);
	exit(-1);
    }

    init_data(data, MAXELEMS); /* Initialize each element in data */
    Mhz = mhz();               /* Estimate the clock frequency */

    printf("Clock frequency is approx. %.1f MHz\n", Mhz);
    printf("Computing memory mountain (read throughput in MB/sec)\n");
    printf("Using best results after %d runs for each data point\n", NUMRUNS);
    
    init_best();               /* initialize array tracking best times */

    for (i = 0; i < NUMRUNS; i++)
	for (sizeindex = 0; sizeindex < NUMSIZES; sizeindex++)
	    for (strideindex = 0; strideindex < NUMSTRIDES; strideindex++)
	    {
		double res = run(size[sizeindex], stride[strideindex], Mhz);
		if (res > best[sizeindex][strideindex])
		    best[sizeindex][strideindex] = res;
	    }

    fprintf(fp,"\t");
    for (strideindex = 0; strideindex < NUMSTRIDES; strideindex++)
	fprintf(fp,"s%d\t", stride[strideindex]);
    fprintf(fp,"\n");

    for (sizeindex = 0; sizeindex < NUMSIZES; sizeindex++)
    {
	if (size[sizeindex] >= (1 << 20))
	    fprintf(fp,"%dm\t", size[sizeindex] / (1 << 20));
	else
	    fprintf(fp,"%dk\t", size[sizeindex] / 1024);

	for (strideindex = 0; strideindex < NUMSTRIDES; strideindex++)
	    fprintf(fp,"%.1f\t", best[sizeindex][strideindex]);

	fprintf(fp,"\n");
    }
    fclose(fp);
    printf("Results written to file %s\n",fname);
}


