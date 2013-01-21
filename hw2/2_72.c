#include <stdio.h>
#include <string.h>

#define LENGTH 6
int x;
char y[LENGTH];

/* Copy integer into buffer if space is available */
/* WARNING: the following code is buggy */

void copy_int(int val, void *buf, int maxbytes) 
{ 
  printf("maxbytes: %i\n val: %i\n interp: %i\n", maxbytes, val, maxbytes- ((int) sizeof(val)));

    if (maxbytes- ((int) sizeof(val)) >= 0)
	memcpy(buf, (void *) &val, sizeof(val));
}

int main(void)
{
    int i,j;
    /* set x to obvious byte pattern */
    x = 0x01234567;
    for (j = 0; j < LENGTH; j++)
    {
	/* reset all of y before test */
	for (i = 0; i < LENGTH; i++)
	    y[i] = 0x00;
	/* do test: call copy_int() */
	copy_int(x, &y[j], LENGTH-j);
	/* output all of y to see what happened */
	printf("iteration %d\ny: ",j);
	for (i = 0; i < LENGTH; i++)
	    printf("%.2x ", (int) y[i]);
	printf("\n\n");
    }
}
