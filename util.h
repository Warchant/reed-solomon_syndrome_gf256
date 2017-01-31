#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "types.h"

#define LEN(x) (sizeof(x) / sizeof(x[0]))

void vector_print(uint32_t *a, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
        printf("%d ", a[i]);
    printf("\n");
}

// allocate new matrix
uint32_t **matrix_new(uint32_t rows, uint32_t cols)
{
    uint32_t **e = (uint32_t **)calloc(rows + 1, sizeof(uint32_t *));
    for (uint32_t i = 0; i < cols; i++)
        e[i] = (uint32_t *)calloc(cols, sizeof(uint32_t));

    return e;
}

// free allocated matrix
void matrix_free(uint32_t **a, uint32_t rows, uint32_t cols)
{
    for (uint32_t i = 0; i < rows + 1; i++)
        free(a[i]);
    free(a);
}

// returns pointer to new copy of matrix `a`
uint32_t **matrix_copy(uint32_t **a, uint32_t rows, uint32_t cols)
{
    uint32_t **b = matrix_new(rows, cols);
    for (uint32_t i = 0; i < rows; i++)
        for (uint32_t j = 0; j < cols; j++)
            b[i][j] = a[i][j];
    return b;
}

void fatal(const uint8_t *who, const uint8_t *msg)
{
    fprintf(stderr, "[%s] %s", who, msg);
    exit(1);
}

void swap(uint32_t *a, uint32_t *b)
{
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

uint32_t *vector_new(uint32_t len)
{
    return (uint32_t *)calloc(len, sizeof(uint32_t));
}

uint32_t *vector_copy(uint32_t *a, uint32_t len)
{
    uint32_t *b = vector_new(len);
    for (uint32_t i = 0; i < len; i++)
        b[i] = a[i];
    return b;
}

void vector_free(uint32_t *a)
{
    free(a);
}

// a = [0,1,2]
// b = [4,5,6]
// stack(a,b) = [0,1,2,4,5,6]
// out_size = len(a) + len(b)
uint32_t *stack(uint32_t *a, uint32_t alen, uint32_t *b, uint32_t blen, uint32_t *out_size)
{
    uint32_t size = alen + blen;
    if (out_size != NULL)
        *out_size = alen + blen;

    uint32_t *c = vector_new(size);

    for (uint32_t i = 0; i < size; i++)
        c[i] = i < alen ? a[i] : b[i - alen];
    return c;
}

// returns subvector in a, starting from start and ending in end (not including)
uint32_t *submatrix(uint32_t *a, uint32_t start, uint32_t end)
{
    uint32_t *g = vector_new(end - start);
    for (uint32_t i = start; i < end; i++)
        g[i - start] = a[i];
    return g;
}

uint32_t pow2(uint32_t q)
{
    return 1 << q;
}

uint32_t *vector_sum(uint32_t *a, uint32_t a_len, uint32_t *b, uint32_t b_len, uint32_t *new_len)
{
    if (a_len != b_len)
        fatal("vector_sum", "a_len should be equal to b_len\n");

    uint32_t *c = vector_new(a_len);

    if (new_len != NULL)
        *new_len = a_len;

    for (uint32_t i = 0; i < a_len; i++)
        c[i] = a[i] + b[i];
    return c;
}

uint8_t *vector_to_str(uint32_t *a, uint32_t a_len)
{
    uint8_t *b = (uint8_t *)calloc(a_len + 1, sizeof(uint8_t));
    for (uint32_t i = 0; i < a_len; i++)
        b[i] = a[i];
    return b;
}

uint32_t *str_to_vector(uint8_t *s, uint32_t s_len)
{
    uint32_t *v = vector_new(s_len);
    for (uint32_t i = 0; i < s_len; i++)
        v[i] = ((uint32_t)s[i] < 0) ? 256 + s[i] : s[i];
    return v;
}

uint32_t sum(uint32_t *v, uint32_t v_len)
{
    uint32_t s = 0;
    for (uint32_t i = 0; i < v_len; i++)
        s += v[i];
    return s;
}

// returns new string = substring[start,end]
uint8_t *substring(uint8_t *s, uint32_t start, uint32_t end, uint32_t *new_size)
{
    if (end - start <= 0)
        fatal("substring", "end-start <= 0\n");

    // size of substr + \0
    uint32_t len = end - start + 1;
    if (new_size != NULL)
        *new_size = len;

    uint8_t *ret = (uint8_t *)calloc(len + 1, sizeof(uint8_t));
    for (uint32_t i = start; i <= end; i++)
        ret[i - start] = s[i];
    return ret;
}

// same as strlen
uint32_t strlength(uint8_t *s)
{
    uint32_t len = 0;
    for (uint32_t i = 0; s[i]; i++)
        len++;
    return len;
}

uint32_t count(uint32_t *v, uint32_t len, uint32_t item)
{
    uint32_t c = 0;
    for (uint32_t i = 0; i < len; i++)
        if (v[i] == item)
            c++;
    return c;
}
