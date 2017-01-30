#pragma once

#include <stdio.h>

int test_n = 0;
int total = 0;

int fail()
{
    printf("test %d failed\n", test_n++);
    return 1;
}

int pass()
{
    printf("test %d passed\n", test_n++);
    return 0;
}

void REQUIRE(int cond)
{
    total += (cond ? pass() : fail());
}

// returns true if arrays a and b are equal
int AR_EQ(int *a, int *b, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

// returns true if matrices a and b are equal
int MX_EQ(int **a, int a_rows, int a_cols,
          int **b, int b_rows, int b_cols)
{
    if (a_rows != b_rows)
        return 0;
    if (a_cols != b_cols)
        return 0;

    for (int i = 0; i < a_rows; i++)
    {
        for (int j = 0; j < a_cols; j++)
        {
            if (a[i][j] != b[i][j])
                return 0;
        }
    }
    return 1;
}
