/*
 * test-mm.c - Checks the correctness and performance of all of the
 *     student's transpose functions and records the results for their
 *     official submitted version as well.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <getopt.h>
#include <sys/types.h>
#include "cachelab.h"
#include <sys/wait.h> // fir WEXITSTATUS
#include <limits.h> // for INT_MAX

/* Maximum array dimension */
#define MAXN 128

/* Globals set on the command line */
static int M = 0;
static int N = 0;
static int P = 0;

/* 
 * eval_perf - Evaluate the performance of the registered transpose functions
 */
void eval_perf()
{
    int flag;
    unsigned int len;
    unsigned long long int marker_start, marker_end, addr;
    char buf[1000], valgrind_cmd[255], csim_cmd[255];
    char filename[128];

    /* Open the complete trace file */
    FILE* full_trace_fp;  
    FILE* part_trace_fp; 

    /* Evaluate the performance of each registered transpose function */

    printf("Step 1: Validating and generating memory traces for Matrix Multiply in mm.c\n");
    /* Use valgrind to generate the trace */
    
    sprintf(valgrind_cmd, "valgrind --tool=lackey --trace-mem=yes --log-fd=1 -v ./tracegen -M %d -N %d -P %d  > fulltrace.tmp", M, N, P);
    flag=WEXITSTATUS(system(valgrind_cmd));
    if (0!=flag) {
        printf("Validation error! Run ./tracegen -M %d -N %d -P %d for details.\n",M,N,P);      
        return;
    }
    
    /* Get the start and end marker addresses */
    FILE* marker_fp = fopen(".marker", "r");
    assert(marker_fp);
    fscanf(marker_fp, "%llx %llx", &marker_start, &marker_end);
    fclose(marker_fp);
    
    full_trace_fp = fopen("fulltrace.tmp", "r");
    assert(full_trace_fp);
    
    
    /* Filtered trace for each transpose function goes in a separate file */
    sprintf(filename, "parttrace.tmp");
    part_trace_fp = fopen(filename, "w");
    assert(part_trace_fp);
    
    /* Locate trace corresponding to the trans function */
    flag = 0;
    while (fgets(buf, 1000, full_trace_fp) != NULL) {
    
        /* We are only interested in memory access instructions */
        if (buf[0]==' ' && buf[2]==' ' &&
            (buf[1]=='S' || buf[1]=='M' || buf[1]=='L' )) {
            sscanf(buf+3, "%llx,%u", &addr, &len);
    
            /* If start marker found, set flag */
            if (addr == marker_start)
                flag = 1;
    
            /* Valgrind creates many spurious accesses to the
               stack that have nothing to do with the students
               code. At the moment, we are ignoring all stack
               accesses by using the simple filter of recording
               accesses to only the low 32-bit portion of the
               address space. At some point it would be nice to
               try to do more informed filtering so that would
               eliminate the valgrind stack references while
               include the student stack references. */
            if (flag && addr < 0xffffffff) {
                fputs(buf, part_trace_fp);
            }
    
            /* if end marker found, close trace file */
            if (addr == marker_end) {
                flag = 0;
                fclose(part_trace_fp);
                break;
            }
        }
    }
    fclose(full_trace_fp);
    
    /* Run the reference simulator */
    printf("Step 2: Evaluating performance\n");
    sprintf(csim_cmd, "./csim -t parttrace.tmp");
    system(csim_cmd);
    
}

/*
 * usage - Print usage info
 */
void usage(char *argv[]){
    printf("Usage: %s [-h] -M <rows for C, rows for A> -N <cols for C, cols for B> -P <cols for A, rows for B> given matrix multiply for C = AxB \n", argv[0]);
    printf("Options:\n");
    printf("  -h          Print this help message.\n");
    printf("  -M <val>    Matrix dimensions (rows for C, rows for A) (max %d)\n", MAXN);
    printf("  -N <val>    Matrix dimensions (cols for C, cols for B) (max %d)\n", MAXN);
    printf("  -P <val>    Matrix dimensions (cols for A, rows for B) (max %d)\n", MAXN);
    printf("Example: %s -M 8 -N 8 -P 8\n", argv[0]);       
}

/*
 * sigsegv_handler - SIGSEGV handler
 */
void sigsegv_handler(int signum){
    printf("Error: Segmentation Fault.\n");
    printf("TEST_TRANS_RESULTS=0:0\n");
    fflush(stdout);
    exit(1);
}

/*
 * sigalrm_handler - SIGALRM handler
 */
void sigalrm_handler(int signum){
    printf("Error: Program timed out.\n");
    printf("TEST_TRANS_RESULTS=0:0\n");
    fflush(stdout);
    exit(1);
}

/* 
 * main - Main routine
 */
int main(int argc, char* argv[])
{
    char c;

    while ((c = getopt(argc,argv,"M:N:P:h")) != -1) {
        switch(c) {
        case 'M':
            M = atoi(optarg);
            break;
        case 'N':
            N = atoi(optarg);
            break;
        case 'P':
            P = atoi(optarg);
            break;
        case 'h':
            usage(argv);
            exit(0);
        default:
            usage(argv);
            exit(1);
        }
    }
  
    if (M == 0 || N == 0 || P == 0) {
        printf("Error: Missing required argument\n");
        usage(argv);
        exit(1);
    }

    if (M > MAXN || N > MAXN || P > MAXN) {
        printf("Error: M or N or P exceeds %d\n", MAXN);
        usage(argv);
        exit(1);
    }

    /* Install SIGSEGV and SIGALRM handlers */
    if (signal(SIGSEGV, sigsegv_handler) == SIG_ERR) {
        fprintf(stderr, "Unable to install SIGALRM handler\n");
        exit(1);
    }

    if (signal(SIGALRM, sigalrm_handler) == SIG_ERR) {
        fprintf(stderr, "Unable to install SIGALRM handler\n");
        exit(1);
    }

    /* Time out and give up after a while */
    alarm(120);

    /* Check the performance of the student's transpose function */
    eval_perf();
  
    return 0;
}
