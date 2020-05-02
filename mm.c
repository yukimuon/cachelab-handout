#include "cachelab.h"

void block_cal(int M, int N, int P, int si, int sj, int sk, 
                int mt1[M][P], int mt2[P][N], int output[M][N], 
                    int BLOCKSIZE){
    for (int i = si; i < si+BLOCKSIZE; i++){
        for (int j = sj; j < sj+BLOCKSIZE; j++){
            int tmp = output[i][j];
            for(int k = sk; k < sk+BLOCKSIZE; k+=4)
            {
                tmp += mt1[i][k] * mt2[k][j];
                tmp += mt1[i][k+1] * mt2[k+1][j];
                tmp += mt1[i][k+2] * mt2[k+2][j];
                tmp += mt1[i][k+3] * mt2[k+3][j];
            }
            output[i][j] = tmp;
        }
    }
}

void mm(int M, int N, int P, int A[M][P], int B[P][N], int C[M][N]){
    int BLOCKSIZE = 8;
    int m = (M / BLOCKSIZE) * BLOCKSIZE;
    int n = (N / BLOCKSIZE) * BLOCKSIZE;
    int p = (P / BLOCKSIZE) * BLOCKSIZE;
    for(int i = 0; i < m; i+=BLOCKSIZE){
        for(int j = 0; j < n; j+=BLOCKSIZE){
            C[i][j] = 0;
            for(int k = 0; k < p; k+=BLOCKSIZE){
                block_cal(M, N, P, i, j, k, A, B, C, BLOCKSIZE);
            }
        }
    }
    for(int i = m; i < M; i++){
        for(int j = 0; j < n; j++){
            int sum = 0;
            for(int k = 0; k < p; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    for(int j = n; j < N; j++){
        for(int i = 0; i < m; i++){
            int sum = 0;
            for(int k = 0; k < p; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    for(int i = m; i < M; i++){
        for(int j = n; j < N; j++){
            int sum = 0;
            for(int k = 0; k < p; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    for(int k = p; k < P; k++){
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
