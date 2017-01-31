#pragma once

#include <stdio.h>

#include "types.h"

uint32_t test_n = 0;
uint32_t total = 0;

#define TEST_EXIT return total

uint8_t fail()
{
    printf("test %d failed\n", test_n++);
    return 1;
}

uint8_t pass()
{
    printf("test %d passed\n", test_n++);
    return 0;
}

void REQUIRE(uint32_t cond)
{
    total += (cond ? pass() : fail());
}

// returns true if arrays a and b are equal
uint8_t AR_EQ(uint32_t *a, uint32_t *b, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}

uint8_t STR_EQ(uint8_t *a, uint8_t *b, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}

// returns true if matrices a and b are equal
uint8_t MX_EQ(uint32_t **a, uint32_t a_rows, uint32_t a_cols,
          uint32_t **b, uint32_t b_rows, uint32_t b_cols)
{
    if (a_rows != b_rows)
        return 0;
    if (a_cols != b_cols)
        return 0;

    for (uint32_t i = 0; i < a_rows; i++)
        for (uint32_t j = 0; j < a_cols; j++)
            if (a[i][j] != b[i][j])
                return 0;
    return 1;
}
