#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

/*
 * Returns a random integer uniformly sampled from [-s, s].
 * Assumes s is non-negative.
 */
int randomUniformInt(int s);

/*
 * Returns a random integer uniformly sampled modulo q, centered around 0.
 */
int mod_q(int q);

/*
 * Allocates and returns an n x N matrix filled with random integers
 * sampled modulo q (centered around 0).
 */
int** generate_lattice(int n, int N, int q);

/*
 * Frees an n x N (or n x (N+1)) matrix allocated with generate_lattice
 * or the new concatenated matrix.
 */
void free_matrix(int **matrix, int n);

/*
 * Allocates and returns an array of size N,
 * where each element is either 0 or 1 (randomly sampled).
 */
int* generate_sk(int N);

/*
 * Multiplies an n x N matrix by a vector of length N.
 * Returns a dynamically allocated vector of length n, where each element
 * is the dot product of the corresponding row of the matrix and the vector.
 */
int* matrix_vector_multiply(int **matrix, int n, int N, int *vector);

/*
 * Computes the matrix-vector product of the n x N lattice and the secret key vector
 * (of length N), then adds element-wise the provided uniform_vector (of length n)
 * to that result. It then concatenates this resulting combined vector as an extra
 * column to the original lattice, returning a new matrix of size n x (N+1).
 */
int** generate_pk(int **lattice, int n, int N, int *secret_key, int *uniform_vector);

int* encrypt(int **public_key, int n, int N, int message_bit, int s, int q);

int decrypt(int *ciphertext, int *secret_key, int N, int q);

#endif /* UTIL_H */
