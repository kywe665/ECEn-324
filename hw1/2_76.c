#include <stdio.h>

int main(void)
{
    /* We are to write C expressions that multiply integer variable x
       by various values of K using only +, -, and << operators, and
       no more than 3 operators total. */

    int x = 10;
    int K;
    int result1;
    int result2; 
    
    /* Example: K = 9 */
    K = 9;
    result1 = (x << 3) + x;  /* you create expressions like this one */
    result2 = K * x;         /* compute using multiply to compare */
    printf("K=9: +-<< result = %d, * result = %d\n", result1, result2);
    
    /* Part A */
    K = 17;
    result1 = (x << 4) + x;   /* replace this 0 with your C expression */
    result2 = K * x;
    printf("K=17: +-<< result = %d, * result = %d\n", result1, result2);
    
    /* Part B */
    K = -7;
    result1 = (x) - (x << 3);   /* replace this 0 with your C expression */
    result2 = K * x;
    printf("K=-7: +-<< result = %d, * result = %d\n", result1, result2);
    
    /* Part C */
    // 0000 0000 1010
    // 0010 0101 1000
    // 0000 0010 1000
    K = 60;
    result1 = (x << 6) - (x << 2);   /* replace this 0 with your C expression */
    result2 = K * x;
    printf("K=60: +-<< result = %d, * result = %d\n", result1, result2);
    
    /* Part D */
    // 0000 0000 1010
    // 1011 1001 1111
    // 0000 1001 0110
    K = -112;
    result1 = (x << 4) - (x << 7);   /* replace this 0 with your C expression */
    result2 = K * x;
    printf("K=-112: +-<< result = %d, * result = %d\n", result1, result2);
    
    
}
