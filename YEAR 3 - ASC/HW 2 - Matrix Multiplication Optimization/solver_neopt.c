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
	fprintf(stdout, "NEOPT SOLVER\n");

	double *first = (double*)calloc(N * N, sizeof(double));
	double *result = (double*)calloc(N * N, sizeof(double));	
	if (!result || !first) {
		fprintf(stderr, "Calloc\n");
	}

	/* a[i][j] = *(a + i*n + j)*/

	/* first = A x B */
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = i; k < N; k++) {
				*(first + i * N + j) += *(A + i * N + k) * *(B + k * N + j);
			}
		}
	}

	/* result = first * Bt */
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				*(result + i * N + j) += *(first + i * N + k) * *(B + j * N + k);
			}
		}
	}

	/* result = result + At * A */
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k <= MIN(i,j); k++) {
				/* k > i => 0 */
				/* k > j => 0 */		
				*(result + i * N + j) += *(A + k * N + i) * *(A + k * N + j);
			}
		}		
	}

	free(first);
	return result;
}
