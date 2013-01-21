#include <stdio.h>

     main()
     {
	 float f,g;
	 f = 0.0;
	 g = f + 1.0;
	 while (f != g)
	 {
	     f = g;
	     g = f + 1.0;
	 }
	 printf("Does it ever get here?\n");
     }
