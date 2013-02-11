#include <stdio.h>
#define M 4
typedef int Marray_t[M][M];

void print_matrix(Marray_t A) {
	int i,j;
	for (i = 0; i < M; i++) {
		for (j = 0; j < M; j++) {
			printf("%8d   ", A[i][j]);
		}
		printf("\n");
	}	
}

void transpose(Marray_t A) {
	int i, j;
	for(i = 0; i < M; i++) {
		for (j = 0; j < i; j++) {
			int t = A[i][j];
			A[i][j] = A[j][i];
			A[j][i] = t;
		}	
	}
}

void transpose_optimized(Marray_t A) {
  int j;
  for (int i = 0; i < M; i++)  {
    int col = A[i][i];
    j = i;
    do {
      int t =  (int) A[i + j];
      A[i][j] = col;
      j++;
      col = (int) A[(M * j) + i];
    } while (j < i);
  }
}

int main() {
	int i, j;
	Marray_t A = {{1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4}};
	print_matrix(A);
	printf("\n");
	transpose(A);
	//transpose_optimized(A);
	print_matrix(A);
}
