#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "keygen.h"

int* generate_sk(int N) {
    int *vector = malloc(N * sizeof(int));
    if (vector == NULL) {
        perror("Failed to allocate memory for the secret key vector");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        vector[i] = rand() % 2;
    }
    return vector;
}

int** generate_pk(int **P, int n, int N, int *secret_key, int *error_vector, int q) {
    int *Ps = matrix_vector_multiply(P, n, N, secret_key, q);

    int **public_key = malloc(n * sizeof(int *));
    if (public_key == NULL) {
        perror("Failed to allocate memory for public key rows");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        public_key[i] = malloc((N + 1) * sizeof(int));
        if (public_key[i] == NULL) {
            perror("Failed to allocate memory for public key columns");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < N; j++) {
            public_key[i][j] = reduce_mod_q(P[i][j], q);
        }

        public_key[i][N] = reduce_mod_q(Ps[i] + error_vector[i], q);
    }

    free(Ps);
    return public_key;
}
