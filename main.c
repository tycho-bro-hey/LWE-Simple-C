#include <stdio.h>
#include "util.h"
#include <time.h>

int main(void) {
    // Seed the random number generator.
    srand((unsigned int)time(NULL));

    // Dimensions for the lattice and vectors.
    int n = 4;      // mumber of rows in the lattice, and length of the error vector.
    int N = 4;      // mumber of columns in the lattice, and length of the secret key.
    int q = 1024;   // modulus for ciphertext space.
    int s = 1;      // parameter for randomUniformInt --> this should be a discrete Gaussian.

    // Create an n x N lattice with entries centered mod q.
    int **lattice = generate_lattice(n, N, q);
    // Generate a random secret key vector of length N.
    int *secret_key = generate_sk(N);
    // Create a uniform vector of length n using randomUniformInt.
    int *error_vector = malloc(n * sizeof(int));
    if (error_vector == NULL) {
        perror("Failed to allocate memory for the uniform vector");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        error_vector[i] = randomUniformInt(s);
    }

    // use generate_pk to concatenate the combined vector to the original lattice.
    int **public_key = generate_pk(lattice, n, N, secret_key, error_vector);

    // print the lattice
    printf("Random %d x %d lattice (values centered mod %d):\n", n, N, q);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < N; j++) {
            printf("%3d ", lattice[i][j]);
        }
        printf("\n");
    }

    // print the secret key vector
    printf("\nSecret key vector of length %d:\n", N);
    for (int j = 0; j < N; j++) {
        printf("%d ", secret_key[j]);
    }
    printf("\n");

    // print the sampled error vector
    printf("\nSampled error vector of length %d (sampled from randomUniformInt with s = %d):\n", n, s);
    for (int i = 0; i < n; i++) {
        printf("%d ", error_vector[i]);
    }
    printf("\n");

    // print the public key
    printf("\nPublic key:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < N + 1; j++) {
            printf("%3d ", public_key[i][j]);
        }
        printf("\n");
    }

    int message = 1; // Message to encrypt
    int* ciphertext = encrypt(public_key, n, N, message, s, q);

    printf("\nCiphertext (encrypted message %d):\n", message);
    for (int i = 0; i < N + 1; i++) {
        printf("%d ", ciphertext[i]);
    }
    printf("\n");

    // clean up dynamically allocated memory
    free_matrix(lattice, n);
    free(secret_key);
    free(error_vector);
    free_matrix(public_key, n);
    free(ciphertext);

    return 0;
}
