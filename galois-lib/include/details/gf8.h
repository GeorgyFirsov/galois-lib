/**
 * @file gf8.h
 * @author Georgy Firsov (gfirsov007@gmail.com)
 * @brief Finite field of size 2 ^ 8 multiplication
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2023
 */


#ifndef GALOISLIB_GF8_INCLUDED
#define GALOISLIB_GF8_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/**
 * @brief Multiply two elements of GF(2 ^ 8).
 * 
 * @param lhs Left operand
 * @param rhs Right operand
 * @return Multiplication result 
 */
unsigned char gf8_multiply(unsigned char lhs, unsigned char rhs);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // !GALOISLIB_GF8_INCLUDED