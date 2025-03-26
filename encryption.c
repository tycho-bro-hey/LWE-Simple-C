#include <stdio.h>
#include <stdlib.h>
#include "util.h"

// Encrypts an integer message (m ∈ Z_t) using an LWE-style public key.
// public_key: an n x (N+1) matrix (last column is b = P*s + e)
// Returns ciphertext: an array of length N + 1: (c1[0..N-1], c2)
int* encrypt(int **public_key, int n, int N, int message, int t, int s, int q) {
    // Step 1: Compute message scaling factor α = floor(q / t)
    int alpha = q / t;

    // Step 2: Sample r ∈ {−1, 0, 1}^n
    int *r = malloc(n * sizeof(int));
    if (r == NULL) {
        perror("Failed to allocate memory for r");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        r[i] = randomUniformInt(1);  // in {-1, 0, 1}
    }

    // Step 3: Allocate ciphertext vector (length N + 1)
    int *ciphertext = malloc((N + 1) * sizeof(int));
    if (ciphertext == NULL) {
        perror("Failed to allocate memory for ciphertext");
        exit(EXIT_FAILURE);
    }

    // Step 4: Compute c1 = r^T * P (result is length N)
    for (int j = 0; j < N; j++) {
        ciphertext[j] = 0;
        for (int i = 0; i < n; i++) {
            ciphertext[j] += public_key[i][j] * r[i];
        }
        ciphertext[j] = reduce_mod_q(ciphertext[j], q);
    }

    // Step 5: Compute c2 = r^T * b + m * α + e
    int c2 = 0;
    for (int i = 0; i < n; i++) {
        c2 += public_key[i][N] * r[i];
    }

    c2 += message * alpha;          // scaled message embedding
    c2 += randomUniformInt(s);      // optional noise
    c2 = reduce_mod_q(c2, q);       // centered mod q

    ciphertext[N] = c2;  // final element of ciphertext

    free(r);
    return ciphertext;
}
