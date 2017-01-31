#pragma once

#include "util.h"

#define GFsize 256
#define m 8 // степень RS-полинома (согласно Стандарта ECMA-130 - восемь)

// несократимый порождающий полином
// согласно Стандарту ECMA-130: P(x) = x8 + x4 + x3 + x2 + 1
int p[m + 1] = {1, 0, 1, 1, 1, 0, 0, 0, 1};

// GF[i] = alpha_of[i]
int alpha_of[GFsize]; // таблица степеней примитивного члена
int index_of[GFsize]; // индексная таблица для быстрого умножения

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
    int i, mask;

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

    for (i = m + 1; i < pow2(m); i++)
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
int gf_mul(int a, int b)
{
    int sum;
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
int gf_div(int a, int b)
{
    int diff;
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

int gf_sum(int a, int b)
{
    return a ^ b;
}

// returns reminder after polynomial division of a/b
int *gf_polydiv(int *a, int alen, int *b, int blen, int *new_len)
{
    if (alen < blen)
        return a;

    // backup a
    int *a_copy = vector_copy(a, alen);

    int q = gf_div(a[alen - 1], b[blen - 1]);

    // g = [0,0...0, b[0], b[1], b[n]]
    // alen-blen zeros at the start
    int *g = (int *)calloc(alen, sizeof(int));
    for (int i = 0; i < alen; i++)
        g[i] = (i < alen - blen) ? 0 : b[i - (alen - blen)];

    for (int i = 0; i < alen; i++)
    {
        g[i] = gf_mul(g[i], q);
        a_copy[i] = gf_sum(a_copy[i], g[i]);
    }

    int a_new_len = alen - 1;
    int *a_new = submatrix(a_copy, 0, a_new_len);

    free(a_copy);
    free(g);

    if (new_len != NULL)
        *new_len = a_new_len;

    return gf_polydiv(a_new, a_new_len, b, blen, new_len);
}

// finds determinant of 2x2 matrix in GF(256)
int gf_matr_det(int **M, int rows, int cols)
{
    if (rows != cols && rows != 2)
        fatal("gf_det", "only 2x2 matrix\n");

    int first = gf_mul(M[0][0], M[1][1]);
    int second = gf_mul(M[1][0], M[0][1]);
    return gf_sum(first, second);
}

// multiply matrices AxB in GF(256), any dimensions
int **gf_matr_mul(int **a, int a_rows, int a_cols,
                  int **b, int b_rows, int b_cols)
{
    if (a_cols != b_rows)
        fatal("gf_matr_mul", " a_cols != b_rows\n");

    int **c = matrix_new(a_rows, b_cols);
    for (int i = 0; i < a_rows; i++)
        for (int j = 0; j < b_cols; j++)
        {
            c[i][j] = 0;
            for (int _k = 0; _k < b_rows; _k++)
                c[i][j] = gf_sum(c[i][j], gf_mul(a[i][_k], b[_k][j]));
        }

    return c;
}

// find inverse 2x2 matrix in GF(256)
int **gf_matr_inv(int **a, int a_rows, int a_cols)
{
    if (a_rows != a_cols && a_rows != 2)
        fatal("gf_matr_inv", "i can find Ainv only for 2x2 matrices\n");

    int det = gf_matr_det(a, a_rows, a_cols);

    int **a0 = matrix_copy(a, a_rows, a_cols);
    int **a0inv = matrix_new(a_rows, a_cols);

    swap(&a0[0][0], &a0[1][1]);

    for (int i = 0; i < a_rows; i++)
        for (int j = 0; j < a_cols; j++)
            a0inv[i][j] = gf_div(a0[i][j], det);

    matrix_free(a0, a_rows, a_cols);

    return a0inv;
}
