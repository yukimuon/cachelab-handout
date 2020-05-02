/* 
 * tracegen.c - Running the binary tracegen with valgrind produces
 * a memory trace of all of the registered transpose functions. 
 * 
 * The beginning and end of each registered transpose function's trace
 * is indicated by reading from "marker" addresses. These two marker
 * addresses are recorded in file for later use.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "cachelab.h"

/* Markers used to bound trace regions of interest */
volatile char MARKER_START, MARKER_END;

//static int A[256][256];
//static int B[256][256];
static int m = 3;
static int n = 4;
static int p = 5;
static int A[256][256];
static int B[256][256];
static int C[256][256];
static int C_ref[256][256];

int main(int argc, char* argv[]){
    int i, j;
    char c;
    while( (c=getopt(argc,argv,"M:N:P:")) != -1){
        switch(c){
        case 'M':
            m = atoi(optarg);
            break;
        case 'N':
            n = atoi(optarg);
            break;
        case 'P':
            p = atoi(optarg);
            break;
        case '?':
        default:
            printf("./tracegen failed to parse its options.\n");
            exit(1);
        }
    }

    printf("Testing parameters M=%d, N=%d, P=%d\n", m, n, p);

    srand(time(0));

    /* Fill A with data */
    randMatrix(m, p, A);
    randMatrix(p, n, B);
    initMatrix(m, n, C);
    initMatrix(m, n, C_ref);

    mm_ref(m, n, p, A, B, C_ref);

    /* Record marker addresses */
    FILE* marker_fp = fopen(".marker","w");
    assert(marker_fp);
    fprintf(marker_fp, "%llx %llx", 
            (unsigned long long int) &MARKER_START,
            (unsigned long long int) &MARKER_END );
    fclose(marker_fp);

    MARKER_START = 33;
    mm(m, n, p, A, B, C);
    MARKER_END = 34;

    for (i=0; i<m; i++){
        for (j=0; j<n; j++){
            if (C[i][j]!=C_ref[i][j]){
                printf("Mismatch in matrix multiply @ m: %d, n:%d\n", i, j);
                printf("Matrix A\n");
                printMatrix(m, p, A);
                printf("Matrix B\n");
                printMatrix(p, n, B);
                printf("Matrix C\n");
                printMatrix(m, n, C);
                return -1;
            }
        }
    }

    return 0;
}


