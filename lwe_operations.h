#ifndef LWE_OPERATIONS_H
#define LWE_OPERATIONS_H

// Homomorphic addition: ct_result = ct1 + ct2 mod q
void homomorphic_add(int *ct_result, const int *ct1, const int *ct2, int length, int q);

// Homomorphic scalar multiplication: ct_result = a * ct mod q
void homomorphic_scalar_mult(int *ct_result, const int *ct, int scalar, int length, int q);

#endif // LWE_OPERATIONS_H
