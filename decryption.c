#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int decrypt(int *ciphertext, int *secret_key, int N, int q) {
    // Step 1: Compute inner product of secret key and c1
    int inner_product = 0;
    for (int i = 0; i < N; i++) {
        inner_product += secret_key[i] * ciphertext[i];
    }

    // Step 2: Subtract inner product from c2
    int difference = ciphertext[N] - inner_product;

    // Step 3: Center the difference modulo q
    difference = reduce_mod_q(difference, q);

    // Step 4: Threshold decision
    if (difference > q / 4 || difference < -q / 4) {
        return 1;
    } else {
        return 0;
    }
}
