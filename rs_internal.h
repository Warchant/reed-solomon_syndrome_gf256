#pragma once

#include "gf.h"
#include "util.h"
#include "types.h"

// returns G - generating polynomial for specific t
uint32_t *generating_polynomial(uint32_t t, uint32_t *len)
{
    if(t == 1){
        uint32_t *v = vector_new(2);
        v[0] = 2;
        v[1] = 1;
        if(len != NULL)
            *len = 2;
        return v;
    }
    else if(t == 2){
        uint32_t *v = vector_new(5);
        v[0] = 116;
        v[1] = 231;
        v[2] = 216;
        v[3] = 30;
        v[4] = 1;
        if(len != NULL)
           *len = 5;
        return v;
    }
    else{
        return NULL;
    }
    /* TODO: fix bug
    uint32_t glen = 2;
    uint32_t *g = vector_new(glen);
    g[0] = alpha_of[1]; // =2
    g[1] = alpha_of[0]; // =1

    if (t >= 2)
    {
        for (uint32_t i = 2; i <= 2 * t; i++)
        {
            uint32_t *g1 = vector_copy(g, glen);
            for (uint32_t j = 0; j < glen; j++)
                g1[j] = gf_mul(g1[j], alpha_of[i]);

            // TODO: this can be more effective
            // g = [[0] + g]
            uint32_t *z = vector_new(1);
            uint32_t *g0 = stack(z, 1, g, glen, &glen);
            vector_free(z);
            vector_free(g);
            g = g0;
            if (len != NULL)
                *len = glen;

            for (uint32_t j = 0; j < glen; j++)
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
    */
}

// shift_msg_to_vec("hi", 2, 2):
//  return [2*t zeroes + ascii2int(msg)] = [0,0,0,0,0x68,0x69]
uint32_t *shift_msg_to_vec(uint8_t *msg, uint32_t len, uint32_t t, uint32_t *new_len)
{
    uint32_t v_len = 2 * t + len;

    if (new_len != NULL)
        *new_len = v_len;

    uint32_t *v = vector_new(v_len);
    for (uint32_t i = 0; i < len; i++)
        *(v + 2 * t + i) = msg[i];

    return v;
}

// returns the remainder of division M/G expanded to rows(M)
// M - message, G - generating polynomial
// t - correction capability of the code
uint32_t *calculate_check_symbols(uint32_t *M, uint32_t m_len, uint32_t *G, uint32_t g_len, uint32_t t, uint32_t *new_len)
{
    uint32_t a_len = m_len - 2 * t;
    if (a_len <= 0)
        return gf_polydiv(M, m_len, G, g_len, new_len);

    uint32_t r_len = 0;
    uint32_t *r = gf_polydiv(M, m_len, G, g_len, &r_len);

    uint32_t *a = vector_new(a_len + (2*t-r_len));
    
    uint32_t *v = stack(r, r_len, a, a_len + (2*t-r_len), new_len);

    vector_free(a);
    vector_free(r);

    return v;
}

uint32_t *calculate_syndrome(uint32_t *c1, uint32_t c_len, uint32_t t, uint32_t *new_len)
{
    uint32_t len = 2 * t;
    if (new_len != NULL)
        *new_len = len;

    uint32_t *S = vector_new(len);
    for (uint32_t i = 1; i <= len; i++)
    {
        S[i - 1] = 0;
        for (uint32_t j = 0; j < c_len; j++)
            S[i - 1] = gf_sum(S[i - 1], gf_mul(c1[j], alpha_of[i * j % GFsize]));
    }

    return S;
}

uint8_t *extract_message(uint32_t *c1, uint32_t c_len, uint32_t t, uint32_t *output_len)
{
    uint8_t *M = vector_to_str(c1, c_len);
    uint8_t *ret = substring(M, 2 * t, c_len - 1, output_len);
    free(M);
    return ret;
}