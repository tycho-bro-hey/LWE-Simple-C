#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Returns a random integer uniformly sampled from [-s, s]
int randomUniformInt(int s) {
    int range = 2 * s + 1;
    return (rand() % range) - s;
}

// Returns a random integer modulo q, centered in [-q/2, q/2)
int reduce_mod_q(int value, int q) {
    int r = value % q;
    if (r < 0) r += q;
    return r - (q / 2);
}

// Allocates and returns an n x N matrix filled with centered mod-q values
int** generate_lattice(int n, int N, int q) {
    int **matrix = malloc(n * sizeof(int *));
    if (matrix == NULL) {
        perror("Failed to allocate lattice rows");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(N * sizeof(int));
        if (matrix[i] == NULL) {
            perror("Failed to allocate lattice columns");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < N; j++) {
            matrix[i][j] = reduce_mod_q(rand(), q);
        }
    }
    return matrix;
}

void free_matrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

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

int* matrix_vector_multiply(int **matrix, int n, int N, int *vector, int q) {
    int *result = malloc(n * sizeof(int));
    if (result == NULL) {
        perror("Failed to allocate memory for the result vector");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < N; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
        result[i] = reduce_mod_q(result[i], q);  // ← Add this
    }
    return result;
}


// Generates the public key matrix: [P | P·s + e]
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
