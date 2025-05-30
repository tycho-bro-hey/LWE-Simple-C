#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>


int randomUniformInt(int s);

int mod_q(int q);
int reduce_mod_q(int value, int q);
int* kroneckerProduct(const int *A, int m, int n, const int *B, int p, int q);

int** generate_lattice(int n, int N, int q);
void free_matrix(int **matrix, int n);

int* matrix_vector_multiply(int **matrix, int n, int N, int *vector, int q);
int mod_q(int q);
int reduce_mod_q(int value, int q);


#endif /* UTIL_H */
