#include <stdio.h>
#include <stdlib.h>
#include "util.h"

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
