#include "util.h"
#include <time.h>  

int randomUniformInt(int s) {
    int range = 2 * s + 1;
    return (rand() % range) - s;
}

int mod_q(int q) {
    int r = rand() % q;
    if (2 * r >= q) {
        r -= q;
    }
    return r;
}

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
            matrix[i][j] = mod_q(q);
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

int* matrix_vector_multiply(int **matrix, int n, int N, int *vector) {
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
    }
    return result;
}

int** generate_pk(int **lattice, int n, int N, int *secret_key, int *uniform_vector) {
    int *mv_result = matrix_vector_multiply(lattice, n, N, secret_key);
    int **new_matrix = malloc(n * sizeof(int *));
    if (new_matrix == NULL) {
        perror("Failed to allocate memory for new matrix rows");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        new_matrix[i] = malloc((N + 1) * sizeof(int));
        if (new_matrix[i] == NULL) {
            perror("Failed to allocate memory for new matrix columns");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < N; j++) {
            new_matrix[i][j] = lattice[i][j];
        }
        new_matrix[i][N] = mv_result[i] + uniform_vector[i];
    }
    free(mv_result);
    return new_matrix;
}
