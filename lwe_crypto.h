#ifndef ENCRYPTION_H
#define ENCRYPTION_H

// Encrypts integer message m ∈ Z_t
int* encrypt(int **public_key, int n, int N, int message, int t, int s, int q);

// Decrypts ciphertext and returns integer message m ∈ Z_t
int decrypt(int *ciphertext, int *secret_key, int N, int t, int q);

#endif // ENCRYPTION_H
