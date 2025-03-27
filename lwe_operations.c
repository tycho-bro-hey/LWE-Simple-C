#include <stdio.h>
#include "lwe_operations.h"
#include "util.h"

// Homomorphic addition: element-wise addition of two ciphertexts
void homomorphic_add(int *ct_result, const int *ct1, const int *ct2, int length, int q) {
    for (int i = 0; i < length; i++) {
        ct_result[i] = reduce_mod_q(ct1[i] + ct2[i], q);
    }
}

// Homomorphic scalar multiplication: multiplies every component of ciphertext by scalar
void homomorphic_scalar_mult(int *ct_result, const int *ct, int scalar, int length, int q) {
    for (int i = 0; i < length; i++) {
        ct_result[i] = reduce_mod_q(ct[i] * scalar, q);
    }
}
