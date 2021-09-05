/*
 * Tema 2 ASC
 * 2021 Spring
 */
#include "utils.h"
#include <cblas.h>
#include <string.h>
/* 
 * Add your BLAS implementation here
 */
double* my_solver(int N, double *A, double *B) {
	printf("BLAS SOLVER\n");
	
	double *first = (double *)malloc(N * N * sizeof(double));
	if (!first) {
		fprintf(stderr, "Malloc\n");
		return NULL;
	}

	memcpy(first, B, N * N * sizeof(double));

	cblas_dtrmm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit,
		N, N, 1, A, N, first, N);
	
	double *result = (double *)malloc(N * N * sizeof(double));
	if (!result) {
		fprintf(stderr, "Malloc\n");
		return NULL;
	}

	memcpy(result, A, N * N * sizeof(double));
	
	cblas_dtrmm(CblasRowMajor, CblasLeft, CblasUpper, CblasTrans, CblasNonUnit,
		N, N, 1, A, N, result, N);

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, N, N, N, 1, first,
		N, B, N, 1, result, N);

	free(first);
	return result;
}
