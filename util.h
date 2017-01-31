#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEN(x) (sizeof(x) / sizeof(x[0]))

void vector_print(int *, int);
int **matrix_new(int, int);
void matrix_free(int **, int, int);
int **matrix_copy(int **, int, int);
void fatal(const char *, const char *);
void swap(int *, int *);
int *vector_new(int);
int *vector_copy(int *, int);
void vector_free(int *);
int *stack(int *, int, int *, int, int *);
int *submatrix(int *, int, int);
int pow2(int);

void vector_print(int *a, int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", a[i]);

    printf("\n");
}

// allocate new matrix
int **matrix_new(int rows, int cols)
{
    int **e = (int **)calloc(rows + 1, sizeof(int *));
    for (int i = 0; i < cols; i++)
        e[i] = (int *)calloc(cols, sizeof(int));

    return e;
}

// free allocated matrix
void matrix_free(int **a, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
        free(a[i]);
    free(a);
}

// returns pointer to new copy of matrix `a`
int **matrix_copy(int **a, int rows, int cols)
{
    int **b = matrix_new(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            b[i][j] = a[i][j];

    return b;
}

void fatal(const char *who, const char *msg)
{
    fprintf(stderr, "[%s] %s", who, msg);
    exit(1);
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int *vector_new(int len)
{
    return (int *)calloc(len, sizeof(int));
}

int *vector_copy(int *a, int len)
{
    int *b = vector_new(len);
    memcpy(b, a, len * sizeof(int));
    return b;
}

void vector_free(int *a)
{
    free(a);
}

// a = [0,1,2]
// b = [4,5,6]
// stack(a,b) = [0,1,2,4,5,6]
// out_size = len(a) + len(b)
int *stack(int *a, int alen, int *b, int blen, int *out_size)
{
    int size = alen + blen;
    if (out_size != NULL)
        *out_size = alen + blen;

    int *c = vector_new(size);

    for (int i = 0; i < size; i++)
        c[i] = i < alen ? a[i] : b[i - alen];

    return c;
}

int *submatrix(int *a, int start, int end)
{
    int *g = vector_new(end - start);
    for (int i = start; i <= end; i++)
        g[i - start] = a[i];

    return g;
}

int pow2(int q)
{
    return 1 << q;
}

int *vector_sum(int *a, int a_len, int *b, int b_len, int *new_len)
{
    if (a_len != b_len)
        fatal("vector_sum", "a_len should be equal to b_len\n");

    int *c = vector_new(a_len);

    if (new_len != NULL)
        *new_len = a_len;
    
    for (int i = 0; i < a_len; i++)
        c[i] = a[i] + b[i];

    return c;
}

char *vector_to_str(int *a, int a_len)
{
    char *b = (char *)calloc(a_len + 1, sizeof(char));
    for (int i = 0; i < a_len; i++)
        b[i] = a[i];

    return b;
}