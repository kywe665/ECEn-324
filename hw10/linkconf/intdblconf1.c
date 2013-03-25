#include <stdio.h>

/* initialization makes this x a strong symbol */
int x;
int y;

int p1()
{
    x = 7;
}

main()
{
    /* p1 treats x as an int */
    p1();
    y = x>>1;
    printf("x: %d, y: %d\n",x,y);
    /* useful to know addresses of x and y: is y immediately after x
       in memory layout? */
    printf("&x: %p, &y: %p\n",&x,&y);
    /* p2 treats x as a double. Will this overwrite y? */
    p2();
    printf("x: %d, y: %d\n",x,y);
}
