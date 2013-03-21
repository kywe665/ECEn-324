/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "clkrbj",     /* Team name */

    "clkrbj",                /* First member Route-Y ID */
    "Kyle Weller",     /* First member full name */
    "kywe665@gmail.com",  /* First member email address */

    "",                   /* Second member Route-Y ID */
    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++)
	      dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
  int i,j;
    pixel *dptr,*sptr;
    for(i=0;i<dim;i+=16)
        for(j=dim-1;j>=0;j--){
            dptr=dst+RIDX(dim-1-j,i,dim);
            sptr=src+RIDX(i,j,dim);
            *(dptr)=*(sptr);sptr+=dim;
            *(dptr+1)=*(sptr);sptr+=dim;
            *(dptr+2)=*(sptr);sptr+=dim;
            *(dptr+3)=*(sptr);sptr+=dim;
            *(dptr+4)=*(sptr);sptr+=dim;
            *(dptr+5)=*(sptr);sptr+=dim;
            *(dptr+6)=*(sptr);sptr+=dim;
            *(dptr+7)=*(sptr);sptr+=dim;
            *(dptr+8)=*(sptr);sptr+=dim;
            *(dptr+9)=*(sptr);sptr+=dim;
            *(dptr+10)=*(sptr);sptr+=dim;
            *(dptr+11)=*(sptr);sptr+=dim;
            *(dptr+12)=*(sptr);sptr+=dim;
            *(dptr+13)=*(sptr);sptr+=dim;
            *(dptr+14)=*(sptr);sptr+=dim;
            *(dptr+15)=*(sptr);
        }   
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	    for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	      accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

static void add_pixel_2(pixel_sum *sum, pixel *a, pixel *b) {
    sum->red = a->red + b->red;
    sum->green = a->green + b->green;
    sum->blue = a->blue + b->blue;
}
 
static void add_pixel_3(pixel_sum *sum, pixel *a, pixel *b, pixel *c) {
    sum->red = a->red + b->red + c->red;
    sum->green = a->green + b->green + c->green;
    sum->blue = a->blue + b->blue + c->blue;
}
 
static void do_row_sum(pixel_sum *sums, int dim, pixel *row) {
    int i;
    add_pixel_2(sums++, row, row+1);
    row++;
    for (i=1; i<dim-1; i++) {
        add_pixel_3(sums++, row-1, row, row+1);
        row++;
    }
    add_pixel_2(sums, row-1, row);
}
 
static void avg_pixel_sums_2(pixel *dst, pixel_sum *a, pixel_sum *b, int num) {
    dst->red = (a->red + b->red)/num;
    dst->green = (a->green + b->green)/num;
    dst->blue = (a->blue + b->blue)/num;
}
 
static void avg_pixel_sums_3(pixel *dst, pixel_sum *a, pixel_sum *b, pixel_sum *c, int num) {
    dst->red = (a->red + b->red + c->red)/num;
    dst->green = (a->green + b->green + c->green)/num;
    dst->blue = (a->blue + b->blue + c->blue)/num;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++)
	      dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    pixel_sum *row_sums = malloc(3*dim*sizeof(pixel_sum));
    pixel_sum *r0 = row_sums;
    pixel_sum *r1 = r0+dim;
    pixel_sum *r2;
    int i, j;
 
    do_row_sum(r0, dim, src);
    do_row_sum(r1, dim, src+dim);
    avg_pixel_sums_2(dst++, r0++, r1++, 4);
    for (i=1; i<dim-1; i++)
        avg_pixel_sums_2(dst++, r0++, r1++, 6);
    avg_pixel_sums_2(dst++, r0, r1, 4);
 
    for (i=1; i<dim-1; i++) {
        r0 = row_sums+((i-1)%3)*dim;
        r1 = row_sums+(i%3)*dim;
        r2 = row_sums+((i+1)%3)*dim;
        do_row_sum(r2, dim, src+(i+1)*dim);
        avg_pixel_sums_3(dst++, r0++, r1++, r2++, 6);
        for (j=1; j<dim-1; j++)
            avg_pixel_sums_3(dst++, r0++, r1++, r2++, 9);
        avg_pixel_sums_3(dst++, r0, r1, r2, 6);
    }

    r0 = row_sums+((i-1)%3)*dim;
    r1 = row_sums+(i%3)*dim;
    avg_pixel_sums_2(dst++, r0++, r1++, 4);
    for (i=1; i<dim-1; i++)
        avg_pixel_sums_2(dst++, r0++, r1++, 6);
    avg_pixel_sums_2(dst, r0, r1, 4);
    free(row_sums);
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

