#include <stdio.h>
#include <stdlib.h>
#include "util.h"

// Encrypts an integer message using an LWE-style public key.
// public_key: an n x (N+1) matrix (last column is b = P*s + e)
// Returns ciphertext: an array of length n + 1: (c1[0..n−1], c2)
int* encrypt(int **public_key, int n, int N, int message, int s, int q) {
    // Sample r ∈ {−1, 0, 1}^n
    int *r = malloc(n * sizeof(int));
    if (r == NULL) {
        perror("Failed to allocate memory for r");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        r[i] = randomUniformInt(1);  // in {-1, 0, 1}
    }

    // Allocate ciphertext vector (length n + 1)
    int *ciphertext = malloc((N + 1) * sizeof(int));
    if (ciphertext == NULL) {
        perror("Failed to allocate memory for ciphertext");
        exit(EXIT_FAILURE);
    }

    // Compute c1 = P * r (vector of length n)
    for (int j = 0; j < N; j++) {
        ciphertext[j] = 0;
        for (int i = 0; i < n; i++) {
            ciphertext[j] += public_key[i][j] * r[i];
        }
        ciphertext[j] = reduce_mod_q(ciphertext[j], q);
    }

    // Compute c2 = b^T * r + q/2 * message + error
    int c2 = 0;
    for (int i = 0; i < n; i++) {
        c2 += public_key[i][N] * r[i];  // b[i] * r[i]
    }

    c2 += randomUniformInt(s);         // add noise
    c2 += message * (q / 2);           // embed the message
    c2 = reduce_mod_q(c2, q);          // centered mod q

    ciphertext[n] = c2;  // append c2 as the last element

    // Clean up
    free(r);
    return ciphertext;
}
