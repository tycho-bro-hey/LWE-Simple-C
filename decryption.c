#include <stdio.h>
#include <stdlib.h>
#include "util.h"

// Decrypts a ciphertext of length N+1 using the secret key and modulus q.
// Returns the recovered message bit (0 or 1).
int decrypt(int *ciphertext, int *secret_key, int N, int q) {
    // Step 1: Compute inner product of secret key and first N elements of ciphertext
    int inner_product = 0;
    for (int i = 0; i < N; i++) {
        inner_product += secret_key[i] * ciphertext[i];
    }

    // Step 2: Subtract inner product from the last ciphertext element
    int difference = ciphertext[N] - inner_product;

    // Step 3: Bring result into [0, q) for comparison
    difference = ((difference % q) + q) % q;

    // Step 4: Decision threshold — closer to 0 → bit 0, closer to q/2 → bit 1
    if (difference > q / 4 && difference < (3 * q) / 4) {
        return 1;
    } else {
        return 0;
    }
}
