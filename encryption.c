#include <stdio.h>
#include <stdlib.h>
#include "util.h"

// Basic LWE encryption function using the public key.
int* encrypt(int **public_key, int n, int N, int message_bit, int s, int q) {
    // Step 1: Sample binary vector r ∈ {0,1}^n
    int *r = malloc(n * sizeof(int));
    if (r == NULL) {
        perror("Failed to allocate memory for vector r");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        r[i] = rand() % 2;
    }

    // Step 2: Sample error vector e ∈ [-s, s]^N+1
    int *error_vector = malloc((N + 1) * sizeof(int));
    if (error_vector == NULL) {
        perror("Failed to allocate memory for error vector");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N + 1; i++) {
        error_vector[i] = randomUniformInt(s);
    }

    // Step 3: Allocate ciphertext vector of length N+1
    int *ciphertext = malloc((N + 1) * sizeof(int));
    if (ciphertext == NULL) {
        perror("Failed to allocate memory for ciphertext");
        exit(EXIT_FAILURE);
    }

    // Step 4: Compute public_key^T · r  → result is length N+1
    for (int j = 0; j < N + 1; j++) {
        ciphertext[j] = 0;
        for (int i = 0; i < n; i++) {
            ciphertext[j] += public_key[i][j] * r[i];
        }

        // Add noise and message-dependent shift
        ciphertext[j] += error_vector[j];
        if (message_bit == 1) {
            ciphertext[j] += q / 2;
        }

        // Reduce mod q to keep values bounded (optional in this version)
        ciphertext[j] = ((ciphertext[j] % q) + q) % q;
    }

    // Clean up
    free(r);
    free(error_vector);

    return ciphertext;
}
