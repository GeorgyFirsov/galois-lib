/**
 * @file gf128.c
 * @author Georgy Firsov (gfirsov007@gmail.com)
 * @brief Finite field of size 2 ^ 128 multiplication
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2023
 */


#include "details/gf128.h"

#include <wmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>


/**
 * @brief Feedback value for multiplication by primitive element in GF(2 ^ 128).
 *        See: IEEE P1619/D16 Annex C
 *        
 * x ^ 128 + x ^ 7 + x ^ 2 + x + 1
 *           ---------------------
 */
#define GF_128_FDBK 0x87


/**
 * @brief Size of GF(2 ^ 128) element in bytes.
 */
#define GF_128_SIZE 16


__m128i gf128_multiply(__m128i lhs, __m128i rhs)
{
    //
    // Intel Carry-Less Multiplication Instruction and its Usage for Computing the GCM Mode
    //
    // Algorithm 2 (Carry-less multiplication of 128-bit operands using PCLMULQDQ)
    // Algorithm 4 (Application of the method for reduction modulo x^128 + x^7 + x^2 + x + 1)
    //

    __m128i tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;
    __m128i mask = _mm_setr_epi32(0xffffffff, 0x00, 0x00, 0x00);

    tmp1 = _mm_clmulepi64_si128(lhs, rhs, 0x00);
    tmp4 = _mm_clmulepi64_si128(lhs, rhs, 0x11);

    tmp2 = _mm_shuffle_epi32(lhs, 78);
    tmp3 = _mm_shuffle_epi32(rhs, 78);
    tmp2 = _mm_xor_si128(tmp2, lhs);
    tmp3 = _mm_xor_si128(tmp3, rhs);

    tmp2 = _mm_clmulepi64_si128(tmp2, tmp3, 0x00);

    tmp2 = _mm_xor_si128(tmp2, tmp1);
    tmp2 = _mm_xor_si128(tmp2, tmp4);

    tmp3 = _mm_slli_si128(tmp2, 8);
    tmp2 = _mm_srli_si128(tmp2, 8);
    tmp1 = _mm_xor_si128(tmp1, tmp3);
    tmp4 = _mm_xor_si128(tmp4, tmp2);

    tmp5 = _mm_srli_epi32(tmp4, 31);
    tmp6 = _mm_srli_epi32(tmp4, 30);
    tmp7 = _mm_srli_epi32(tmp4, 25);

    tmp5 = _mm_xor_si128(tmp5, tmp6);
    tmp5 = _mm_xor_si128(tmp5, tmp7);

    tmp6 = _mm_shuffle_epi32(tmp5, 147);

    tmp5 = _mm_and_si128(mask, tmp6);
    tmp6 = _mm_andnot_si128(mask, tmp6);
    tmp1 = _mm_xor_si128(tmp1, tmp6);
    tmp4 = _mm_xor_si128(tmp4, tmp5);

    tmp8  = _mm_slli_epi32(tmp4, 1);
    tmp1  = _mm_xor_si128(tmp1, tmp8);
    tmp9  = _mm_slli_epi32(tmp4, 2);
    tmp1  = _mm_xor_si128(tmp1, tmp9);
    tmp10 = _mm_slli_epi32(tmp4, 7);
    tmp1  = _mm_xor_si128(tmp1, tmp10);

    return _mm_xor_si128(tmp1, tmp4);
}


__m128i gf128_multiply_primitive(__m128i op)
{
    unsigned int idx;
    unsigned char carry_in;
    unsigned char carry_out;
    unsigned char* internal_op = (unsigned char*)&op;

    carry_in = 0;
    for (idx = 0; idx < GF_128_SIZE; ++idx)
    {
        carry_out        = (internal_op[idx] >> 7) & 0xff;
        internal_op[idx] = ((internal_op[idx] << 1) + carry_in) & 0xff;
        carry_in         = carry_out;
    }

    if (carry_out)
    {
        internal_op[0] ^= GF_128_FDBK;
    }

    return op;
}
