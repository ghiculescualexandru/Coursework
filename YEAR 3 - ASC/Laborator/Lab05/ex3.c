#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
 
void BMMultiply(int n, double* a, double* b, double* c)
{
    int bi=0;
    int bj=0;
    int bk=0;
    int i=0;
    int j=0;
    int k=0;
    // TODO: set block dimension blockSize
    int blockSize=100; 
 
    for(bi=0; bi<n; bi+=blockSize)
        for(bj=0; bj<n; bj+=blockSize)
            for(bk=0; bk<n; bk+=blockSize)
                for(i=0; i<blockSize; i++)
                    for(j=0; j<blockSize; j++)
                        for(k=0; k<blockSize; k++)
                            *(c + (bi+i)*n + (bj+j)) = *(a + (bi+i)*n + (bk+k)) * *(b + (bk+k)*n + (bj+j));
}
 
int main(void)
{
    int n;
    double* A;
    double* B;
    double* C;
    int numreps = 10;
    int i=0;
    int j=0;
    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed;
    // TODO: set matrix dimension n
    n = 500;
    // allocate memory for the matrices
 
    // TODO: allocate matrices A, B & C
    ///////////////////// Matrix A //////////////////////////
    // TODO ...
    A = (double *)malloc(n * n * sizeof(double));
    ///////////////////// Matrix B ////////////////////////// 
    // TODO ...
    B = (double *)malloc(n * n * sizeof(double));
    ///////////////////// Matrix C //////////////////////////
    // TODO ...
    C = (double *)malloc(n * n * sizeof(double));
    // Initialize matrices A & B
    for(i = 0; i < n * n; i++)
    { 
        A[i] = 1;
        B[i] = 2;
    }
 
    //multiply matrices
 
    printf("Multiply matrices %d times...\n", numreps);
    for (i=0; i<numreps; i++)
    {
        gettimeofday(&tv1, &tz);
        BMMultiply(n,A,B,C);
        gettimeofday(&tv2, &tz);
        elapsed += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    }
    printf("Time = %lf \n",elapsed);
 
    //deallocate memory for matrices A, B & C
    // TODO ...
    free(A);
    free(B);
    free(C);

    return 0;
}