# Learning with errors-based encryption in C
Simple demonstration of a Learning With Errors (LWE)-based cryptosystem. These programs serve as a starting point for incorporating LWE primitives into the seL4 architecture.
- util.c contains foundational functions for generating random lattices, secret keys, centered modular arithmetic, and public keys.
- encryption.c implements the core LWE encryption algorithm. A single-bit message is encrypted using the public key and small random error.
- decryption.c implements decryption by computing the inner product of the secret key and the ciphertext, then recovering the message using a threshold decision on the centered difference.
- main.c demonstrates a full LWE encryption cycle: key generation, encryption of a single-bit message, and decryption to recover the original bit.