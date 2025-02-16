#include <stdio.h>
#include "key_gen.h"
#include <time.h>

int main(void) {
    // Seed the random number generator.
    srand((unsigned int)time(NULL));

    // Dimensions for the lattice and vectors.
    int n = 4;      // Number of rows in the lattice, and length of the uniform vector.
    int N = 4;      // Number of columns in the lattice, and length of the secret key.
    int q = 1024;   // Modulus for the random lattice entries.
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
