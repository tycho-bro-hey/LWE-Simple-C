# Learning with errors-based encryption in C
Simple demonstration of a Learning With Errors (LWE)-based cryptosystem. These programs serve as a starting point for incorporating LWE primitives into the seL4 architecture.
- util.c contains foundational functions for generating random lattices, secret keys, centered modular arithmetic, and public keys.
- keygen.c handles key generation and provides functions for generating binary secret keys and computing public keys.
- encryption.c implements the core LWE encryption algorithm. A integer-based messages are encrypted using the public key and small random error.
- decryption.c implements decryption by computing the inner product of the secret key and the ciphertext, then recovering the message using rounding.
- main.c demonstrates a full LWE encryption cycle: key generation, encryption of integer messages, and decryption to recover the original messages.