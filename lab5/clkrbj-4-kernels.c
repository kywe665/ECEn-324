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

typedef struct {
    int red;
    int green;
    int blue;
} pixel_int;


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
	 int maxI = max(i-1, 0);
	 int maxJ = max(j-1, 0); 
	 int minI = min(i+1, dim-1);
	 int minJ = min(j+1, dim-1); 

    initialize_pixel_sum(&sum);
    for(ii = maxI; ii <= minI; ii++) 
	for(jj = maxJ; jj <= minJ; jj++) 
	    accumulate_sum(&sum, src[ii*dim+jj]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
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

static void edges(int dim, pixel *src, pixel *dst) {
	pixel_int cache;
	pixel result;
	
	cache.red = cache.green = cache.blue = 0; 
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src++;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;		
	src+=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src--;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src--;
	
	result.red   = cache.red>>2;
	result.green = cache.green>>2;
	result.blue  = cache.blue>>2;
	*dst = result;
	dst += dim-1;
	
	cache.red = cache.green = cache.blue = 0; 
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src--;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;		
	src+=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src++;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src+=1+dim*(dim-3);
	
	result.red   = cache.red>>2;
	result.green = cache.green>>2;
	result.blue  = cache.blue>>2;
	*dst = result;
	dst+=1+dim*(dim-2);
	
	cache.red = cache.green = cache.blue = 0; 
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src++;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src-=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src--;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src+=dim-1;
	
	result.red   = cache.red>>2;
	result.green = cache.green>>2;
	result.blue  = cache.blue>>2;
	*dst = result;	
	dst+=dim-1;
	
	cache.red = cache.green = cache.blue = 0; 
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src--;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src+=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src++;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	
	result.red   = cache.red>>2;
	result.green = cache.green>>2;
	result.blue  = cache.blue>>2;
	*dst = result;	
}

static void top(int pos,int dim,pixel *src,pixel *dst) {
	pixel_int cache;
	pixel result;
	cache.red = cache.green = cache.blue = 0; 
	
	src += pos;
	dst += pos;
	
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src--;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src+=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src++;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src++;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src-=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	
	result.red   = cache.red/6;
	result.green = cache.green/6;
	result.blue  = cache.blue/6;
	*dst = result;	
}

static void left(int pos,int dim,pixel *src,pixel *dst) {
	pixel_int cache;
	pixel result;
	cache.red = cache.green = cache.blue = 0; 
	
	src += pos;
	dst += pos;
	
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src-=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src++;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src+=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src+=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src--;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	
	result.red   = cache.red/6;
	result.green = cache.green/6;
	result.blue  = cache.blue/6;
	*dst = result;	
}

static void bottom(int pos,int dim,pixel *src,pixel *dst) {
	pixel_int cache;
	pixel result;
	cache.red = cache.green = cache.blue = 0; 
	
	src += pos;
	dst += pos;
	
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src++;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src-=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src--;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src--;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src+=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	
	result.red   = cache.red/6;
	result.green = cache.green/6;
	result.blue  = cache.blue/6;
	*dst = result;	
}

static void right(int pos,int dim,pixel *src,pixel *dst) {
	pixel_int cache;
	pixel result;
	cache.red = cache.green = cache.blue = 0; 
	
	src += pos;
	dst += pos;
	
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src-=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src--;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src+=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src+=dim;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	src++;
	cache.red   += src->red;
	cache.green += src->green;
	cache.blue  += src->blue;	
	
	result.red   = cache.red/6;
	result.green = cache.green/6;
	result.blue  = cache.blue/6;
	*dst = result;	
}

char smooth_descr[] = "smooth: the good one";
void smooth(int dim, pixel *src, pixel *dst) 
{
	int i,j;
	int temp;
	
	temp = dim*dim-dim;
	
	edges(dim,src,dst);
	
	for(j = 1;j < dim-1;j++) {
		top(j,dim,src,dst);
		bottom((dim-1)*dim+j,dim,src,dst);
	}
	
	for(i = 1;i < dim-1;i++) {
		left(i*dim,dim,src,dst);
		right(i*dim+dim-1,dim,src,dst);
	}
	
	temp = dim;
	
	pixel_int tleft;
	pixel_int tright;
	pixel_int bleft;
	pixel_int bright;
	dst+=1+dim;
	src+=1+dim;
	
	for(i = 1;i < dim-1; i+=2) {  
		for(j = 1;j < dim-1;j+=2) {
	
			tleft.red = src->red;
			tleft.green = src->green;
			tleft.blue = src->blue; 
			src++;
			tleft.red += src->red;
			tleft.green += src->green;
			tleft.blue += src->blue; 
			src+=dim;
			tleft.red += src->red;
			tleft.green += src->green;
			tleft.blue += src->blue; 
			src--;
			tleft.red += src->red;
			tleft.green += src->green;
			tleft.blue += src->blue; 
			src--;
			tright = bleft = bright = tleft;
			tleft.red += src->red;
			tleft.green += src->green;
			tleft.blue += src->blue; 
			src-=dim;
			tleft.red += src->red;
			tleft.green += src->green;
			tleft.blue += src->blue; 
			bleft = tleft;
			src-=dim;
			tleft.red += src->red;
			tleft.green += src->green;
			tleft.blue += src->blue; 
			src++;
			tleft.red += src->red;
			tleft.green += src->green;
			tleft.blue += src->blue; 
			tright.red += src->red;
			tright.green += src->green;
			tright.blue += src->blue; 
			src++;
			tleft.red += src->red;
			tleft.green += src->green;
			tleft.blue += src->blue; 
			tright.red += src->red;
			tright.green += src->green;
			tright.blue += src->blue; 
			src++;	
			tright.red += src->red;
			tright.green += src->green;
			tright.blue += src->blue; 	
			src+=dim;
			bright.red += src->red;
			bright.green += src->green;
			bright.blue += src->blue; 
			tright.red += src->red;
			tright.green += src->green;
			tright.blue += src->blue; 
			src+=dim;
			bright.red += src->red;
			bright.green += src->green;
			bright.blue += src->blue; 
			tright.red += src->red;
			tright.green += src->green;
			tright.blue += src->blue; 
			src+=dim;
			bright.red += src->red;
			bright.green += src->green;
			bright.blue += src->blue; 
			src--;
			bright.red += src->red;
			bright.green += src->green;
			bright.blue += src->blue; 
			bleft.red += src->red;
			bleft.green += src->green;
			bleft.blue += src->blue; 
			src--;
			bright.red += src->red;
			bright.green += src->green;
			bright.blue += src->blue; 
			bleft.red += src->red;
			bleft.green += src->green;
			bleft.blue += src->blue; 
			src--;
			bleft.red += src->red;
			bleft.green += src->green;
			bleft.blue += src->blue; 
			src-=2*dim-3;
			
			dst->red   = tleft.red/9;
			dst->green = tleft.green/9;
			dst->blue  = tleft.blue/9; 
			dst+=dim;
			dst->red   = bleft.red/9;
			dst->green = bleft.green/9;
			dst->blue  = bleft.blue/9; 
			dst++;
			dst->red   = bright.red/9;
			dst->green = bright.green/9;
			dst->blue  = bright.blue/9; 
			dst-=dim;
			dst->red   = tright.red/9;
			dst->green = tright.green/9;
			dst->blue  = tright.blue/9; 
			dst++;
		
		}
		src+=2+dim;
		dst+=2+dim;
	}
	
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

