#include "gf.h"
#include "test.h"
#include "util.h"

uint32_t main()
{
    generate_gf();

    REQUIRE(gf_mul(116, 180) == 96);
    REQUIRE(gf_div(180, 116) == 116);
    REQUIRE(gf_sum(1, 4) == 5);
    REQUIRE(alpha_of[7] == 128);
    REQUIRE(gf_mul(alpha_of[250], alpha_of[10]) == 32);
    REQUIRE(gf_div(116, 192) == 251);

    { // gf_polydiv test
        uint32_t a[] = {1, 2, 3, 4, 5, 9};
        uint32_t b[] = {6, 7, 8, 9, 10};
        uint32_t c[] = {126, 21, 32, 203};
        uint32_t len = 0;
        uint32_t *d = gf_polydiv(a, LEN(a), b, LEN(b), &len);
        REQUIRE(AR_EQ(c, d, LEN(c)));
        REQUIRE(LEN(c) == len);
        vector_free(d);
    }

    { // gf_polydiv test
        uint32_t a[] = {0,0,0,0,104,101,108,108,111};
        uint32_t b[] = {116,231,216,30,1};
        uint32_t c[] = {228,212,208,62};
        uint32_t len = 0;
        uint32_t *d = gf_polydiv(a, LEN(a), b, LEN(b), &len);
        REQUIRE(AR_EQ(c, d, LEN(c)));
        REQUIRE(LEN(c) == len);
        vector_free(d);
    }

    { // gf_matr_det test
        uint32_t **e = matrix_new(2, 2);
        e[0][0] = 100;
        e[1][1] = 91;
        e[0][1] = 50;
        e[1][0] = 90;
        REQUIRE(gf_matr_det(e, 2, 2) == 111);
        matrix_free(e, 2, 2);
    }

    { // gf_matr_mul
        uint32_t **e = matrix_new(2, 3);
        for (uint32_t i = 0; i < 2; i++)
            for (uint32_t j = 0; j < 3; j++)
                e[i][j] = i * j;

        uint32_t **d = matrix_new(3, 3);
        for (uint32_t i = 0; i < 3; i++)
            for (uint32_t j = 0; j < 3; j++)
                d[i][j] = i + j;

        uint32_t **c = gf_matr_mul(e, 2, 3, d, 3, 3);
        uint32_t **z = matrix_new(2, 3);

        z[0][0] = 0;
        z[0][1] = 0;
        z[0][2] = 0;

        z[1][0] = 5;
        z[1][1] = 4;
        z[1][2] = 11;

        REQUIRE(MX_EQ(c, 2, 3, z, 2, 3));

        matrix_free(c, 2, 3);
        matrix_free(z, 2, 3);
        matrix_free(e, 2, 3);
        matrix_free(d, 3, 3);
    }

    { // gf_matr_inv
        uint32_t **a = matrix_new(2, 2);
        a[0][0] = 1;
        a[0][1] = 150;
        a[1][0] = 13;
        a[1][1] = 93;

        uint32_t **b = matrix_new(2, 2);
        b[0][0] = 171;
        b[0][1] = 35;
        b[1][0] = 72;
        b[1][1] = 236;

        uint32_t **c = gf_matr_inv(a, 2, 2);

        REQUIRE(MX_EQ(c, 2, 2, b, 2, 2));

        matrix_free(a, 2, 2);
        matrix_free(b, 2, 2);
        matrix_free(c, 2, 2);
    }

    { // gf_matr_mul_vector
        uint32_t **a = matrix_new(2, 2);
        a[0][0] = 1;
        a[0][1] = 2;
        a[1][0] = 3;
        a[1][1] = 4;
        uint32_t v[] = {5, 6};
        uint32_t expected[] = {9, 23};
        uint32_t *actual = gf_matr_mul_vector(a, 2, 2, v, 2);
        REQUIRE(AR_EQ(expected, actual, LEN(expected)));
        matrix_free(a, 2, 2);
        vector_free(actual);
    }

    TEST_EXIT;
}