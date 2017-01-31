#pragma once

#include "gf.h"
#include "util.h"

// returns G - generating polynomial for specific t
int *generating_polynomial(int t, int *len)
{
    int glen = 2;
    int *g = vector_new(glen);
    g[0] = alpha_of[1]; // =2
    g[1] = alpha_of[0]; // =1

    if (t >= 2)
    {
        for (int i = 2; i <= 2 * t; i++)
        {
            int *g1 = vector_copy(g, glen);
            for (int j = 0; j < glen; j++)
                g1[j] = gf_mul(g1[j], alpha_of[i]);

            // TODO: this can be more effective
            // g = [[0] + g]
            int *g0 = stack(vector_new(1), 1, g, glen, &glen);
            vector_free(g);
            g = g0;
            if (len != NULL)
                *len = glen;

            for (int j = 0; j < glen; j++)
                g[j] = gf_sum(g0[j], g1[j]);
        }
        return g;
    }
    else
    {
        if (len != NULL)
            *len = glen;
        return g;
    }
}

// shift_msg_to_vec("hi", 2, 2):
//  return [2*t zeroes + ascii2int(msg)] = [0,0,0,0,0x68,0x69]
int *shift_msg_to_vec(char *msg, int len, int t, int *new_len)
{
    int v_len = 2 * t + len;

    if (new_len != NULL)
        *new_len = v_len;

    int *v = vector_new(v_len);
    for (int i = 0; i < len; i++)
        *(v + 2 * t + i) = msg[i];

    return v;
}

// returns the remainder of division M/G expanded to rows(M)
// M - message, G - generating polynomial
// t - correction capability of the code
int *calculate_check_symbols(int *M, int m_len, int *G, int g_len, int t, int *new_len)
{
    int a_len = m_len - 2 * t;
    if (a_len <= 0)
        return gf_polydiv(M, m_len, G, g_len, new_len);

    int *a = vector_new(a_len);

    int r_len = 0;
    int *r = gf_polydiv(M, m_len, G, g_len, &r_len);

    int *v = stack(r, r_len, a, a_len, new_len);

    vector_free(a);
    vector_free(r);

    return v;
}
