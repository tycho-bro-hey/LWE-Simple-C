#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "keygen.h"
#include "lwe_crypto.h"
#include "lwe_operations.h"

int main(void) {
    // seed the random number generator
    srand((unsigned int)time(NULL));

    // LWE parameters
    int n = 256;
    int N = 256;
    int q = 4096;
    int s = 1;     
    int t = 32;        

    // Generate lattice P, secret key s, and error vector e
    int **lattice = generate_lattice(n, N, q);
    int *secret_key = generate_sk(N);
    int *error_vector = malloc(n * sizeof(int));
    if (!error_vector) {
        perror("Failed to allocate error vector");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        error_vector[i] = randomUniformInt(s);
    }

    // Generate public key: [P | P·s + e]
    int **public_key = generate_pk(lattice, n, N, secret_key, error_vector, q);

    // Print secret key
    printf("Secret key:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", secret_key[i]);
    }
    printf("\n\n");

    int weight = 0;
    for (int i = 0; i < N; i++) weight += secret_key[i];
    printf("Secret key weight: %d (", weight);
    for (int i = 0; i < N; i++) printf("%d", secret_key[i]);
    printf(")\n\n");

    // test messages
    int test_messages[] = {0, 3, 6};
    int num_tests = sizeof(test_messages) / sizeof(test_messages[0]);

    for (int i = 0; i < num_tests; i++) {
        int m = test_messages[i];
        int *ciphertext = encrypt(public_key, n, N, m, t, s, q);
        int decrypted = decrypt(ciphertext, secret_key, N, t, q);

        printf("Original: %2d | Decrypted: %2d %s\n", m, decrypted,
               m == decrypted ? "correct" : "incorrect");

        free(ciphertext);
    }

    // test LWE operations
    printf("\n--- Homomorphic Addition Test ---\n");
    int m1 = 3, m2 = 5;
    int *ct1 = encrypt(public_key, n, N, m1, t, s, q);
    int *ct2 = encrypt(public_key, n, N, m2, t, s, q);

    int *ct_sum = malloc((N + 1) * sizeof(int));
    homomorphic_add(ct_sum, ct1, ct2, N + 1, q);

    int dec_sum = decrypt(ct_sum, secret_key, N, t, q);

    printf("m1 = %d, m2 = %d, expected sum = %d, decrypted = %d %s\n",
           m1, m2, (m1 + m2) % t, dec_sum,
           ((m1 + m2) % t == dec_sum) ? "correct" : "incorrect");

    free(ct1);
    free(ct2);
    free(ct_sum);

    printf("\n--- Homomorphic Scalar Multiplication Test ---\n");
    int m = 4;
    int scalar = 3;
    int *ct = encrypt(public_key, n, N, m, t, s, q);

    int *ct_scaled = malloc((N + 1) * sizeof(int));
    homomorphic_scalar_mult(ct_scaled, ct, scalar, N + 1, q);

    int dec_scaled = decrypt(ct_scaled, secret_key, N, t, q);

    printf("m = %d, scalar = %d, expected product = %d, decrypted = %d %s\n",
           m, scalar, (m * scalar) % t, dec_scaled,
           ((m * scalar) % t == dec_scaled) ? "correct" : "incorrect");

    free(ct);
    free(ct_scaled);

    free_matrix(lattice, n);
    free_matrix(public_key, n);
    free(secret_key);
    free(error_vector);

    return 0;
}
