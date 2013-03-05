/* This file contains code from the CS:APP website. These are
 * basically library routines for the code examples in the book
 */

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

/* Create vector of specified length */
vec_ptr new_vec(long int len)
{
    /* Allocate header structure */
    vec_ptr result = (vec_ptr) malloc(sizeof(vec_rec));
    if (!result)
        return NULL;  /* Couldn't allocate storage */
    result->len = len;
    result->allocated_len = len;  /* not shown in book */
    /* Allocate array */
    if (len > 0) {
        data_t *data = (data_t *)calloc(len, sizeof(data_t));
	if (!data) {
	    free((void *) result);
 	    return NULL; /* Couldn't allocate storage */
	}
	result->data = data;
    }
    else
	result->data = NULL;
    return result;
}

/*
 * Retrieve vector element and store at dest.
 * Return 0 (out of bounds) or 1 (successful)
 */
int get_vec_element(vec_ptr v, long int index, data_t *dest)
{
    if (index < 0 || index >= v->len)
	return 0;
    *dest = v->data[index];
    return 1;
}

/* Return length of vector */
long int vec_length(vec_ptr v)
{
    return v->len;
}

/* Return the base address of vector data */
data_t *get_vec_start(vec_ptr v)
{
    return v->data;
}


/*
 * Set vector element.
 * Return 0 (out of bounds) or 1 (successful)
 */
int set_vec_element(vec_ptr v, long int index, data_t val)
{
    if (index < 0 || index >= v->len)
	return 0;
    v->data[index] = val;
    return 1;
}

/* Set vector length.  If >= allocated length, will reallocate */
void set_vec_length(vec_ptr v, long int newlen)
{
    if (newlen > v->allocated_len) {
	free(v->data);
	v->data = calloc(newlen, sizeof(data_t));
	v->allocated_len = newlen;
    }
    v->len = newlen;
}
