#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Returns a random integer uniformly sampled from [-s, s].
// Assumes s is non-negative.
int randomUniformInt(int s) {
    int range = 2 * s + 1;  // Total number of values from -s to s.
    return (rand() % range) - s;
}

// mod_q returns a random integer uniformly sampled modulo q,
// centered around 0.
int mod_q(int q) {
    int r = rand() % q;  // r is in [0, q-1]
    if (2 * r >= q) {
        r -= q;
    }
    return r;
}

// generate_lattice allocates and returns an n x N matrix filled with random integers
// sampled modulo q (centered around 0).
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

// free_matrix frees an n x N (or n x (N+1)) matrix allocated with generate_lattice
// or the new concatenated matrix.
void free_matrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// generate_sk allocates and returns an array of size N,
// where each element is either 0 or 1 (randomly sampled).
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

// matrix_vector_multiply multiplies an n x N matrix by a vector of length N.
// It returns a dynamically allocated vector of length n, where each element
// is the dot product of the corresponding row of the matrix and the vector.
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

// generate_pk computes the matrix-vector product of the n x N lattice and the secret key vector
// (of length N), then adds element-wise the provided uniform_vector (of length n) to that result.
// It then concatenates this resulting combined vector as an extra column to the original lattice,
// returning a new matrix of size n x (N+1).
int** generate_pk(int **lattice, int n, int N, int *secret_key, int *uniform_vector) {
    // Compute the matrix-vector multiplication.
    int *mv_result = matrix_vector_multiply(lattice, n, N, secret_key);

    // Allocate the new matrix with n rows and (N+1) columns.
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
        // Copy the original row.
        for (int j = 0; j < N; j++) {
            new_matrix[i][j] = lattice[i][j];
        }
        // Concatenate the combined value as the last column.
        new_matrix[i][N] = mv_result[i] + uniform_vector[i];
    }
    free(mv_result);
    return new_matrix;
}

int main(void) {
    // Seed the random number generator.
    srand((unsigned int)time(NULL));

    // Dimensions for the lattice and vectors.
    int n = 5;      // Number of rows in the lattice, and length of the uniform vector.
    int N = 7;      // Number of columns in the lattice, and length of the secret key.
    int q = 1000;   // Modulus for the random lattice entries.
    int s = 1;      // Parameter for randomUniformInt.

    // Create an n x N lattice with entries centered mod q.
    int **lattice = generate_lattice(n, N, q);
    // Generate a random secret key vector of length N.
    int *secret_key = generate_sk(N);
    // Create a uniform vector of length n using randomUniformInt.
    int *uniform_vector = malloc(n * sizeof(int));
    if (uniform_vector == NULL) {
        perror("Failed to allocate memory for the uniform vector");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        uniform_vector[i] = randomUniformInt(s);
    }

    // Use generate_pk to concatenate the combined vector to the original lattice.
    int **concatenated_matrix = generate_pk(lattice, n, N, secret_key, uniform_vector);

    // Print the lattice.
    printf("Random %d x %d lattice (values centered mod %d):\n", n, N, q);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < N; j++) {
            printf("%3d ", lattice[i][j]);
        }
        printf("\n");
    }

    // Print the secret key vector.
    printf("\nSecret key vector of length %d:\n", N);
    for (int j = 0; j < N; j++) {
        printf("%d ", secret_key[j]);
    }
    printf("\n");

    // Print the sampled error vector.
    printf("\nSampled error vector of length %d (sampled from randomUniformInt with s = %d):\n", n, s);
    for (int i = 0; i < n; i++) {
        printf("%d ", uniform_vector[i]);
    }
    printf("\n");

    // Print the concatenated matrix.
    // Print the public key.
    printf("\nPublic key:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < N + 1; j++) {
            printf("%3d ", concatenated_matrix[i][j]);
        }
        printf("\n");
    }
    // Clean up dynamically allocated memory.
    free_matrix(lattice, n);
    free(secret_key);
    free(uniform_vector);
    free_matrix(concatenated_matrix, n);

    return 0;
}

