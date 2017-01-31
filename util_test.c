#include "util.h"
#include "test.h"

int main()
{
    { // submatrix test
        int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        int *b = submatrix(a, 3, 7);
        int c[] = {4, 5, 6, 7, 8};
        REQUIRE(AR_EQ(b, c, LEN(c)));
        vector_free(b);
    }

    { // stack test
        int a[] = {0, 1, 2};
        int b[] = {4, 5, 6};
        int d[] = {0, 1, 2, 4, 5, 6};
        int c_len = 0;
        int *c = stack(a, 3, b, 3, &c_len);
        REQUIRE(AR_EQ(d, c, LEN(d)));
        REQUIRE(c_len == LEN(d));
        vector_free(c);
    }

    { // vector copy test
        int a[] = {5, 10, 100};
        int len = LEN(a);
        int *b = vector_copy(a, len);
        REQUIRE(AR_EQ(a, b, len));
        vector_free(b);
    }

    { // vector sum test
        int a[] = {1, 2, 3};
        int b[] = {9, 8, 7};
        int c[] = {10, 10, 10};
        int s_len = 0;
        int *s = vector_sum(a, LEN(a), b, LEN(b), &s_len);
        REQUIRE(AR_EQ(c, s, LEN(c)));
        REQUIRE(s_len == LEN(c));
        vector_free(s);
    }

    { // vector to str test
        char* expected = "hello world";
        int v[] = {104,101,108,108,111,32,119,111,114,108,100};
        char* actual = vector_to_str(v, LEN(v));
        REQUIRE(STR_EQ(expected, actual, LEN(v)));
        free(actual);
    }

    TEST_EXIT;
}
