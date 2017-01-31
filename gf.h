#pragma once

#include "util.h"
#include "types.h"

#define GFsize 256
#define m 8 // степень RS-полинома (согласно Стандарта ECMA-130 - восемь)

// несократимый порождающий полином
// согласно Стандарту ECMA-130: P(x) = x8 + x4 + x3 + x2 + 1
uint32_t p[m + 1] = {1, 0, 1, 1, 1, 0, 0, 0, 1};

// GF[i] = alpha_of[i]
uint32_t alpha_of[GFsize]; // таблица степеней примитивного члена
uint32_t index_of[GFsize]; // индексная таблица для быстрого умножения

//----------------------------------------------------------------------------
// генерируем look-up таблицу для быстрого умножения для GF(2 ^ m) на основе
// несократимого порождающего полинома Pc от p[0] до p[m].
//
// look-up таблица:
// index->polynomial из alpha_of[] содержит j = alpha ^ i,
// где alpha есть примитивный член, обычно равный 2
// а ^ - операция возведения в степень (не XOR!);
//
// polynomial form -> index из index_of[j = alpha ^ i] = i;
//
// c Simon Rockliff
//----------------------------------------------------------------------------

void generate_gf()
{
    uint32_t i, mask;

    mask = 1;
    alpha_of[m] = 0;

    for (i = 0; i < m; i++)
    {
        alpha_of[i] = mask;
        index_of[alpha_of[i]] = i;

        if (p[i] != 0)
            alpha_of[m] ^= mask;
        mask <<= 1;
    }
    index_of[alpha_of[m]] = m;
    mask >>= 1;

    for (i = m + 1; i < pow2(m) - 1; i++)
    {
        if (alpha_of[i - 1] >= mask)
            alpha_of[i] = alpha_of[m] ^ ((alpha_of[i - 1] ^ mask) << 1);
        else
            alpha_of[i] = alpha_of[i - 1] << 1;

        index_of[alpha_of[i]] = i;
    }
    index_of[0] = -1;
}

// функция возвращает результат умножения
// двух полиномов a на b в полях Галуа
uint32_t gf_mul(uint32_t a, uint32_t b)
{
    uint32_t sum;
    if (a == 0 || b == 0)
        return 0;
    if (a < 0 || b < 0)
        return -1;
    sum = index_of[a] + index_of[b]; // вычисляем сумму индексов полиномов
    if (sum >= GFsize - 1)
        sum -= GFsize - 1; // приводим сумму к модулю GF
    return alpha_of[sum];  // переводим результат в полиномиальную...
                           // ...форму и возвращаем результат
}

// функция возвращает результат деления
// двух полиномов a на b в полях Галуа
// при попытке деления на ноль функция
// возвращает -1
uint32_t gf_div(uint32_t a, uint32_t b)
{
    int32_t diff;
    if (a == 0)
        return 0; // немного оптимизации не повредит
    if (b == 0)
        return -1;                    // на ноль делить нельзя!
    diff = index_of[a] - index_of[b]; // вычисляем разность индексов
    if (diff < 0)
        diff += GFsize - 1; // приводим разность к модулю GF
    return alpha_of[diff];  // переводим результат в полиномиальную...
                            // ...форму и возвращаем результат
}

uint32_t gf_sum(uint32_t a, uint32_t b)
{
    return a ^ b;
}

// returns reminder after polynomial division of a/b
uint32_t *gf_polydiv_recursive(uint32_t *a, uint32_t alen, uint32_t *b, uint32_t blen, uint32_t *new_len)
{
    if (alen < blen)
        return a;

    uint32_t q = gf_div(a[alen - 1], b[blen - 1]);

    // g = [0,0...0, b[0], b[1], b[n]]
    // alen-blen zeros at the start
    uint32_t *g = vector_new(alen);
    for (uint32_t i = 0; i < alen; i++)
        g[i] = (i < alen - blen) ? 0 : b[i - (alen - blen)];

    for (uint32_t i = 0; i < alen; i++)
    {
        g[i] = gf_mul(g[i], q);
        a[i] = gf_sum(a[i], g[i]);
    }

    uint32_t a_new_len = alen - 1;
    uint32_t *a_new = submatrix(a, 0, a_new_len);

    vector_free(a);
    vector_free(g);

    if (new_len != NULL)
        *new_len = a_new_len;

    return gf_polydiv_recursive(a_new, a_new_len, b, blen, new_len);
}

uint32_t *gf_polydiv(uint32_t *a, uint32_t alen, uint32_t *b, uint32_t blen, uint32_t *new_len){
    uint32_t *v = vector_copy(a, alen);
    return gf_polydiv_recursive(v, alen, b, blen, new_len);
}

// finds determinant of 2x2 matrix in GF(256)
uint32_t gf_matr_det(uint32_t **M, uint32_t rows, uint32_t cols)
{
    if (rows != cols && rows != 2)
        fatal("gf_det", "only 2x2 matrix\n");

    uint32_t first = gf_mul(M[0][0], M[1][1]);
    uint32_t second = gf_mul(M[1][0], M[0][1]);
    return gf_sum(first, second);
}

// multiply matrices AxB in GF(256), any dimensions
uint32_t **gf_matr_mul(uint32_t **a, uint32_t a_rows, uint32_t a_cols,
                  uint32_t **b, uint32_t b_rows, uint32_t b_cols)
{
    if (a_cols != b_rows)
        fatal("gf_matr_mul", " a_cols != b_rows\n");

    uint32_t **c = matrix_new(a_rows, b_cols);
    for (uint32_t i = 0; i < a_rows; i++)
        for (uint32_t j = 0; j < b_cols; j++)
        {
            c[i][j] = 0;
            for (uint32_t _k = 0; _k < b_rows; _k++)
                c[i][j] = gf_sum(c[i][j], gf_mul(a[i][_k], b[_k][j]));
        }

    return c;
}

uint32_t *gf_matr_mul_vector(uint32_t **a, uint32_t rows, uint32_t cols, uint32_t* v, uint32_t len){
    if(rows != len)
        fatal("gf_matr_mul_vector", "rows != vector rows\n");

    uint32_t *c = vector_new(rows);
    for (uint32_t i = 0; i < rows; i++)
        for (uint32_t j = 0; j < 1; j++)
        {
            c[i] = 0;
            for (uint32_t _k = 0; _k < len; _k++)
                c[i] = gf_sum(c[i], gf_mul(a[i][_k], v[_k]));
        }

    return c;
}

// find inverse 2x2 matrix in GF(256)
uint32_t **gf_matr_inv(uint32_t **a, uint32_t a_rows, uint32_t a_cols)
{
    if (a_rows != a_cols && a_rows != 2)
        fatal("gf_matr_inv", "Ainv only for 2x2 matrices\n");

    uint32_t det = gf_matr_det(a, a_rows, a_cols);

    uint32_t **a0 = matrix_copy(a, a_rows, a_cols);
    uint32_t **a0inv = matrix_new(a_rows, a_cols);

    swap(&a0[0][0], &a0[1][1]);

    for (uint32_t i = 0; i < a_rows; i++)
        for (uint32_t j = 0; j < a_cols; j++)
            a0inv[i][j] = gf_div(a0[i][j], det);

    matrix_free(a0, a_rows, a_cols);

    return a0inv;
}

