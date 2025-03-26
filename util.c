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