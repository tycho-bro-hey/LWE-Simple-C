#ifndef KEYGEN_H
#define KEYGEN_H

// Generates a binary secret key vector of length N
int* generate_sk(int N);

// Computes the public key matrix [P | P·s + e] using lattice P, secret key, and error vector
int** generate_pk(int **P, int n, int N, int *secret_key, int *error_vector, int q);

#endif // KEYGEN_H
