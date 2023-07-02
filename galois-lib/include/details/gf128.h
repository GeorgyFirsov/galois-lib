/**
 * @file gf128.h
 * @author Georgy Firsov (gfirsov007@gmail.com)
 * @brief Finite field of size 2 ^ 128 multiplication
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2023
 */


#ifndef GALOISLIB_GF128_INCLUDED
#define GALOISLIB_GF128_INCLUDED


#include <emmintrin.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/**
 * @brief Multiply two elements of GF(2 ^ 128).
 *
 * @param lhs Left operand
 * @param rhs Righr operand
 */
__m128i gf128_multiply(__m128i lhs, __m128i rhs);


/**
 * @brief Multiply element of GF(2 ^ 128) by primitive element x.
 * 
 * @param op Operand to multiply
 */
__m128i gf128_multiply_primitive(__m128i op);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // !GALOISLIB_GF128_INCLUDED
