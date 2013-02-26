#include <stdio.h>

#define SIZE 6

int array1[SIZE+1] = { 0xdddd, 0xeeee, 0xbbbb, 0xaaaa, 0xffff, 0xcccc, 0x0101 } ;
int array2[SIZE+1] = { 0xdddd, 0xeeee, 0xbbbb, 0xaaaa, 0xffff, 0xcccc, 0x0101 } ;
    
void bubble_a(int *data, int count)
{
    /* code from book goes here */
  int i, last;
  for (last = count-1; last > 0; last--) {
    for(i = 0; i < last; i++) {
      if(data[i+1] < data[i]) {
        //swap adjacent
        int t = data[i+1];
        data[i+1] = data[i];
        data[i] = t;
      }
    }
  }
}

void bubble_p(int *arr, int n)  /* pointer version */
{
    /* pointer version of code you write */
  int i,j,temp;
  for(i=0;i<(n-1);i++) {
		for(j=0;j<(n-1-i);j++) {
			if(*(arr+j)>*(arr+(j+1))) {
				temp=*(arr+j);
				*(arr+j)=*(arr+(j+1));
				*(arr+(j+1))=temp;
			}
		}
  }
}

int main()
{
    int i;

    printf("array version\n");
    printf("  Before: ");
    for (i = 0; i < SIZE; i++)
	printf("0x%.4x ", array1[i]);
    printf("\n");
    bubble_a(array1, SIZE);
    printf("  After:  ");
    for (i = 0; i < SIZE; i++)
	printf("0x%.4x ", array1[i]);
    printf("\n");
    printf("pointer version\n");
    printf("  Before: ");
    for (i = 0; i < SIZE; i++)
	printf("0x%.4x ", array2[i]);
    printf("\n");
    bubble_p(array2, SIZE);
    printf("  After:  ");
    for (i = 0; i < SIZE; i++)
	printf("0x%.4x ", array2[i]);
    printf("\n");
}
