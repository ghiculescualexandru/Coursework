/*
 * Tema 2 ASC
 * 2021 Spring
 */
#include "utils.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

/*
 * Add your unoptimized implementation here
 */
double* my_solver(int N, double *A, double* B) {
	fprintf(stdout, "OPT SOLVER\n");

	double *first = (double*)calloc(N * N, sizeof(double));
	double *result = (double*)malloc(N * N * sizeof(double));	
	if (!result || !first) {
		fprintf(stderr, "Calloc\n");
	}

	/* a[i][j] = *(a + i*n + j)*/

	for (int i = 0; i < N; i++) {
		double *orig_pa = A + i * N + 0;
		double *orig_pr = first + i * N + 0;
		for (int k = i; k < N; k++)  {
			register double a = *(orig_pa + k);
			double *pb = B + k * N + 0;
			double *pr = orig_pr;
			for (int j = 0; j < N; j++) {
				*pr += a * *pb;
				pb++;
				pr++;							
			}
		}
	}

	/* result = first * Bt */
	for (int i = 0; i < N; i++) {
		double *orig_pa = first + i * N + 0;
		for (int j = 0; j < N; j++) {
			double *pa = orig_pa;
			double *pb = B + j * N + 0;
			register double sum = 0.0;	
			for (int k = 0; k < N; k++) {
				sum += *pa * *pb;
				pa++;
				pb++;
			}
			*(result + i * N + j) = sum;
		}
	}

	/* result = result + At * A */
	for (int k = 0; k < N; k++) {
		double *orig_pa = A + k * N + 0;
		for (int i = k; i < N; i++) {
			register double a = *(orig_pa + i);
			double *pb = orig_pa + k;
			double *pr = result + i * N + k;
			for (int j = k; j < N; j++) {
				*pr += a * *pb;
				pr++;
				pb++;		
			}
		}		
	}	

	free(first);
	return result;
}
