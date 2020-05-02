/*
 * cachelab.c - Cache Lab helper functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cachelab.h"
#include <time.h>

/* 
 * initMatrix - Initialize the given matrix 
 */
void initMatrix(int M, int N, int A[M][N])
{
    int i, j;
    for (i = 0; i < M; i++){
        for (j = 0; j < N; j++){
            A[i][j]=0;
        }
    }
}

void randMatrix(int M, int N, int A[M][N]) {
    int i, j;
    for (i = 0; i < M; i++){
        for (j = 0; j < N; j++){
            A[i][j]=rand()%MAX_VAL;
        }
    }
}

void printMatrix(int M, int N, int A[M][N]) {
    int i, j;
    for (i = 0; i < M; i++){
        for (j = 0; j < N; j++){
            printf(" %d ", A[i][j]);
        }
        printf("\n");
    }
}
