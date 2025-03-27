#include <stdio.h>
#include <stdlib.h>
#include "util.h"

// Encrypts an integer message (m ∈ Z_t) using an LWE-style public key.
// public_key: an n x (N+1) matrix (last column is b = pk*s + e)
// Returns ciphertext: an array of length N + 1: (c1[0..N-1], c2)
int* encrypt(int **public_key, int n, int N, int message, int t, int s, int q) {
    // compute message scaling factor alpha = floor(q / t)
    int alpha = q / t;

    // sample r ∈ {−1, 0, 1}^n
    int *r = malloc(n * sizeof(int));
    if (r == NULL) {
        perror("Failed to allocate memory for r");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        r[i] = randomUniformInt(1);  // in {-1, 0, 1}
    }

    // allocate ciphertext vector (length N + 1)
    int *ciphertext = malloc((N + 1) * sizeof(int));
    if (ciphertext == NULL) {
        perror("Failed to allocate memory for ciphertext");
        exit(EXIT_FAILURE);
    }

    // compute c1 = r^T * P (result is length N)
    for (int j = 0; j < N; j++) {
        ciphertext[j] = 0;
        for (int i = 0; i < n; i++) {
            ciphertext[j] += public_key[i][j] * r[i];
        }
        ciphertext[j] = reduce_mod_q(ciphertext[j], q);
    }

    // compute c2 = r^T * b + m * alpha + e
    int c2 = 0;
    for (int i = 0; i < n; i++) {
        c2 += public_key[i][N] * r[i];
    }

    c2 += message * alpha;         
    c2 += randomUniformInt(s);      
    c2 = reduce_mod_q(c2, q);       

    ciphertext[N] = c2;  // final element of ciphertext

    free(r);
    return ciphertext;
}

// Decrypts a ciphertext of length N + 1 using secret key of length N
// Recovers an integer message m ∈ Z_t
int decrypt(int *ciphertext, int *secret_key, int N, int t, int q) {
    // compute inner product sᵗ * c₁
    int inner_product = 0;
    for (int i = 0; i < N; i++) {
        inner_product += secret_key[i] * ciphertext[i];
    }

    // compute difference: c₂ - ⟨s, c₁⟩
    int difference = ciphertext[N] - inner_product;

    // centered mod q
    difference = reduce_mod_q(difference, q);

    // recover message via scaled rounding
    int alpha = q / t;
    int recovered_message = (difference + (alpha / 2)) / alpha;

    /*
    // print intermediate values for debugging
    printf("c2: %d\n", ciphertext[N]);
    printf("(s, c1): %d\n", inner_product);
    printf("Difference: %d\n", difference);
    printf("Recovered message: %d\n", recovered_message);
    printf("\n");
    */

    return recovered_message;
}