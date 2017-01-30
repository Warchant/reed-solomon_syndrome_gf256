#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEN(x) (sizeof(x) / sizeof(x[0]))

int *submatrix(int *a, int start, int end)
{
    int *g = (int *)malloc(sizeof(int) * (end - start));
    for (int i = start; i <= end; i++)
    {
        g[i - start] = a[i];
    }
    return g;
}

void vector_print(int *a, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

// allocate new matrix
int **matrix_new(int rows, int cols)
{
    int **e = (int **)malloc(sizeof(int *) * rows);
    for (int i = 0; i < cols; i++)
    {
        e[i] = (int *)malloc(sizeof(int) * cols);
    }

    return e;
}

// free allocated matrix
int matrix_free(int **a, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        if (a[i])
            free(a[i]);
        else
            return 0; // NULL
    }
    if (a)
        free(a);
    else
        return 0; // NULL

    return 1;
}

int **matrix_copy(int **a, int rows, int cols)
{
    int **b = matrix_new(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            b[i][j] = a[i][j];
        }
    }

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
    int *b = (int *)malloc(sizeof(int) * len);
    return b;
}

int *vector_copy(int *a, int len)
{
    int *b = vector_new(len);
    memcpy(b, a, len * sizeof(int));
    return b;
}

int vector_free(int *a)
{
    if (a)
    {
        free(a);
        return 1;
    }
    else
        return 0;
}

// a = [0,1,2]
// b = [4,5,6]
// stack(a,b) = [0,1,2,4,5,6]
int *stack(int *a, int alen, int *b, int blen)
{
    int *c = vector_new(alen + blen);
    for (int i = 0; i < alen + blen; i++)
    {
        c[i] = i < alen ? a[i] : b[i - alen];
    }

    return c;
}

int pow2(int q){
    return 1 << q;
}