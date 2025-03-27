# Learning with errors-based encryption in C
Simple demonstration of a Learning With Errors (LWE)-based cryptosystem. These programs serve as a starting point for incorporating LWE primitives into the seL4 architecture.
- ```util.c``` contains foundational functions for generating random lattices, secret keys, centered modular arithmetic, and public keys.
- ```keygen.c``` handles key generation and provides functions for generating binary secret keys and computing public keys.
- ```lwe_crypto.c``` implements the LWE encryption and decryption algorithms for an integer-based message space.
- ```lwe_operations.c``` implements basic homomorphic operations on ciphertexts, including addition and scalar multiplication.
- ```main.c``` demonstrates a full LWE cryptosystem: key generation, encryption of integer messages, and decryption to recover the original messages. also includes test cases for homomorphic operations