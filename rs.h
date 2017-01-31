#pragma once

#include "rs_internal.h"

uint8_t *encode_message(uint8_t *msg, uint32_t msg_len, uint32_t t, uint32_t *output_len);
uint8_t *decode_message(uint8_t *codeword, uint32_t c_len, uint32_t t, uint32_t *output_len);

// takes `message` with length=`len`, adds exactly 2*t control symbols and
// writes output to `output`. output will be `len+2*t` bytes long
uint8_t *encode_message(uint8_t *msg, uint32_t msg_len, uint32_t t, uint32_t *output_len)
{
    if (t > 2)
        fatal("encode_message", "not implemented for t > 2");

    uint32_t k = msg_len;
    uint32_t n = 2 * t + k;

    uint32_t m_len = 0;
    uint32_t *M = shift_msg_to_vec(msg, msg_len, t, &m_len);

    uint32_t g_len = 0;
    uint32_t *G = generating_polynomial(t, &g_len);

    uint32_t b_len = 0;
    uint32_t *B = calculate_check_symbols(M, m_len, G, g_len, t, &b_len);

    vector_free(G);

    uint32_t c_len = 0;
    uint32_t *C = vector_sum(B, b_len, M, m_len, &c_len);

    vector_free(M);
    vector_free(B);

    if (output_len != NULL)
        *output_len = c_len;

    uint8_t *ret = vector_to_str(C, c_len);
    vector_free(C);
    return ret;
}

// receives "codeword" (output of encode_message), its length,
// code parameter = t, returns decoded string and its length in `output_len`
uint8_t *decode_message(uint8_t *codeword, uint32_t c_len, uint32_t t, uint32_t *output_len)
{
    if (t > 2)
        fatal("decode_message", "not implemented for t > 2\n");

    uint32_t *c1 = str_to_vector(codeword, c_len);

    uint32_t g_len = 0;
    uint32_t *G = generating_polynomial(t, &g_len);

    uint32_t b_len = 0;
    uint32_t *b = gf_polydiv(c1, c_len, G, g_len, &b_len);
    uint32_t b_sum = sum(b, b_len);

    vector_free(G);
    vector_free(b);

    // no errors found
    if (b_sum == 0)
    {
        uint8_t *ret = extract_message(c1, c_len, t, output_len);
        vector_free(c1);
        return ret;
    }

    // uh, there are some errors:

    // syndrome
    uint32_t s_len = 0;
    uint32_t *S = calculate_syndrome(c1, c_len, t, &s_len);

    uint32_t det = gf_sum(gf_mul(S[0], S[2]), gf_mul(S[1], S[1]));

    uint32_t *a1 = vector_new(t);
    for (uint32_t i = 0; i < t; i++)
        a1[i] = S[t + i];

    // one error
    if (det == 0)
    {
        uint32_t *a0 = vector_new(t);
        for (uint32_t i = 0; i < t; i++)
            a0[i] = S[t + i - 1];

        uint32_t error_index = index_of[gf_div(a1[0], a0[0])];

        uint32_t *e = vector_new(c_len);
        e[error_index] = gf_div(S[0], alpha_of[error_index]);

        for (uint32_t i = 0; i < c_len; i++)
            c1[i] = gf_sum(c1[i], e[i]);

        uint8_t *ret = extract_message(c1, c_len, t, output_len);

        vector_free(e);
        vector_free(a0);
        vector_free(a1);
        vector_free(S);
        vector_free(c1);

        return ret;
    }

    // more errors
    uint32_t **a0 = matrix_new(t, t);
    for (uint32_t i = 0; i < t; i++)
        for (uint32_t j = 0; j < t; j++)
            a0[i][j] = S[i + j];

    uint32_t **a0inv = gf_matr_inv(a0, t, t);
    uint32_t *sigma = gf_matr_mul_vector(a0inv, t, t, a1, t);

    // after we calculated sigma, we don't need a0 and a0inv
    matrix_free(a0, t, t);
    matrix_free(a0inv, t, t);
    vector_free(a1);

    uint32_t *zeros = vector_new(GFsize - 1);
    for (uint32_t i = 0; i < GFsize - 1; i++)
    {
        uint32_t s2 = gf_mul(sigma[0], gf_mul(alpha_of[i], alpha_of[i]));
        uint32_t s1 = gf_mul(sigma[1], alpha_of[i]);
        zeros[i] = gf_sum(1, gf_sum(s1, s2));
    }

    uint32_t zero_n = count(zeros, GFsize - 1, 0);
    if (zero_n == 0)
    {
        // for some reason errors can not be located
        return NULL;
    }

    uint32_t *error_indices = vector_new(zero_n);
    uint32_t index = 0;
    for (uint32_t i = 0; i < GFsize - 1; i++)
    {
        if (zeros[i] == 0)
        {
            if (index >= zero_n)
                return NULL;
            error_indices[index++] = GFsize - 1 - i;
        }
    }

    vector_free(zeros);
    vector_free(sigma);

    uint32_t **E0 = matrix_new(2, 2);
    E0[0][0] = alpha_of[error_indices[0]];
    E0[0][1] = alpha_of[error_indices[1]];
    E0[1][0] = gf_mul(alpha_of[error_indices[0]], alpha_of[error_indices[0]]);
    E0[1][1] = gf_mul(alpha_of[error_indices[1]], alpha_of[error_indices[1]]);

    uint32_t **E0inv = gf_matr_inv(E0, 2, 2);

    uint32_t *E1 = vector_new(2);
    E1[0] = S[0];
    E1[1] = S[1];

    uint32_t *e = gf_matr_mul_vector(E0inv, 2, 2, E1, 2);

    matrix_free(E0, 2, 2);
    matrix_free(E0inv, 2, 2);
    vector_free(E1);

    uint32_t *e_fix = vector_new(c_len);
    e_fix[error_indices[0]] = e[0];
    e_fix[error_indices[1]] = e[1];

    vector_free(error_indices);
    vector_free(e);

    for (uint32_t i = 0; i < c_len; i++)
        c1[i] = gf_sum(c1[i], e_fix[i]);

    uint8_t *ret = extract_message(c1, c_len, t, output_len);

    vector_free(c1);
    vector_free(S);
    vector_free(e_fix);
    return ret;
}