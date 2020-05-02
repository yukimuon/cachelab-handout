/* 
 * cachelab.h - Prototypes for Cache Lab helper functions
 */

#ifndef CACHELAB_TOOLS_H
#define CACHELAB_TOOLS_H

#define MAX_TRANS_FUNCS 100
#define MAX_VAL 10

void mm(int M, int N, int P, int A[M][P], int B[P][N], int C[M][N]);
void mm_ref(int M, int N, int P, int A[M][P], int B[P][N], int C[M][N]);

/* Fill the matrix with data */
void initMatrix(int M, int N, int A[M][N]);
void randMatrix(int M, int N, int A[M][N]);
void printMatrix(int M, int N, int A[M][N]);

#endif /* CACHELAB_TOOLS_H */
