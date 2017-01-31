#pragma once

#include "rs_internal.h"

// takes `message` with length=`len`, adds exactly 2*t control symbols and
// writes output to `output`. output will be `len+2*t` bytes long
char *encode_message(char *msg, int msg_len, int t, int *output_len)
{
    if (t > 2)
        fatal("encode_message", "t > 2 is not implemented");

    int k = msg_len;
    int n = 2 * t + k;

    int m_len = 0;
    int *M = shift_msg_to_vec(msg, msg_len, t, &m_len);
    vector_print(M, m_len);

    int g_len = 0;
    int *G = generating_polynomial(t, &g_len);
    vector_print(G, g_len);

    int b_len = 0;
    int *B = calculate_check_symbols(M, m_len, G, g_len, t, &b_len);
    vector_print(B, b_len);

    vector_free(G);

    int c_len = 0;
    int *C = vector_sum(B, b_len, M, m_len, &c_len);

    vector_free(M);
    vector_free(B);

    if (output_len != NULL)
        *output_len = c_len;

    char *ret = vector_to_str(C, c_len);
    vector_free(C);
    return ret;
}