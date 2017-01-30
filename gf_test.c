#include "gf.h"
#include "test.h"
#include "util.h"

int main()
{
    generate_gf();

    REQUIRE(gf_mul(116, 180) == 96);
    REQUIRE(gf_div(180, 116) == 116);
    REQUIRE(gf_sum(1, 4) == 5);
    REQUIRE(alpha_of[7] == 128);
    REQUIRE(gf_mul(alpha_of[250], alpha_of[10]) == 32);
    REQUIRE(gf_div(116, 192) == 251);

    { // gf_polydiv test
        int a[] = {1, 2, 3, 4, 5, 9};
        int b[] = {6, 7, 8, 9, 10};
        int c[] = {126, 21, 32, 203};
        int *d = gf_polydiv(a, LEN(a), b, LEN(b));
        REQUIRE(AR_EQ(c, d, LEN(c)));
        free(d);
    }

    { // gf_matr_det test
        int **e = matrix_new(2, 2);
        e[0][0] = 100;
        e[1][1] = 91;
        e[0][1] = 50;
        e[1][0] = 90;
        REQUIRE(gf_matr_det(e, 2, 2) == 111);
        matrix_free(e, 2, 2);
    }

    { // gf_matr_mul
        int **e = matrix_new(2, 3);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                e[i][j] = i * j;
            }
        }

        int **d = matrix_new(3, 3);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                d[i][j] = i + j;
            }
        }

        int **c = gf_matr_mul(e, 2, 3, d, 3, 3);
        int **z = matrix_new(2, 3);

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
        int **a = matrix_new(2, 2);
        a[0][0] = 1;
        a[0][1] = 150;
        a[1][0] = 13;
        a[1][1] = 93;

        int **b = matrix_new(2, 2);
        b[0][0] = 171;
        b[0][1] = 35;
        b[1][0] = 72;
        b[1][1] = 236;

        int **c = gf_matr_inv(a, 2, 2);

        REQUIRE(MX_EQ(c, 2, 2, b, 2, 2));

        matrix_free(a, 2, 2);
        matrix_free(b, 2, 2);
        matrix_free(c, 2, 2);
    }

    vector_print(alpha_of, GFsize);
    vector_print(index_of, GFsize);
    return total;
}