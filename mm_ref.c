#include "cachelab.h"

void mm_ref(int M, int N, int P, int A[M][P], int B[P][N], int C[M][N]){
    int i, j, k, sum;
    for (i=0; i<M; i++){
        for (j=0; j<N; j++){
            sum = 0;
            for (k=0; k<P; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}
