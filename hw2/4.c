#include <stdio.h>

     main()
     {
	 float f,g;
	 f = 0.0;
	 g = f + 1.0;
  int i = 0;
	 while (f != g)
	 {
      //printf("i: %d\n", i);
        i++;
	     f = g;
	     g = f + 1.0;
	 }
	 printf("Does it ever get here? i: %d\n", i);
     }
