#include "util.h"
#include "test.h"

uint32_t main()
{
    { // submatrix test
        uint32_t a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        uint32_t *b = submatrix(a, 3, 7);
        uint32_t c[] = {4, 5, 6, 7};
        REQUIRE(AR_EQ(b, c, LEN(c)));
        vector_free(b);
    }

    { // stack test
        uint32_t a[] = {0, 1, 2};
        uint32_t b[] = {4, 5, 6};
        uint32_t d[] = {0, 1, 2, 4, 5, 6};
        uint32_t c_len = 0;
        uint32_t *c = stack(a, 3, b, 3, &c_len);
        REQUIRE(AR_EQ(d, c, LEN(d)));
        REQUIRE(c_len == LEN(d));
        vector_free(c);
    }

    { // vector copy test
        uint32_t a[] = {5, 10, 100};
        uint32_t len = LEN(a);
        uint32_t *b = vector_copy(a, len);
        REQUIRE(AR_EQ(a, b, len));
        vector_free(b);
    }

    { // vector sum test
        uint32_t a[] = {1, 2, 3};
        uint32_t b[] = {9, 8, 7};
        uint32_t c[] = {10, 10, 10};
        uint32_t s_len = 0;
        uint32_t *s = vector_sum(a, LEN(a), b, LEN(b), &s_len);
        REQUIRE(AR_EQ(c, s, LEN(c)));
        REQUIRE(s_len == LEN(c));
        vector_free(s);
    }

    { // vector to str test
        uint8_t *expected = "hello world";
        uint32_t v[] = {104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100};
        uint8_t *actual = vector_to_str(v, LEN(v));
        REQUIRE(STR_EQ(expected, actual, LEN(v)));
        free(actual);
    }

    { // sum test
        uint32_t a[] = {1, 2, 4, 8, 16, 32};
        REQUIRE(sum(a, LEN(a)) == 63);
    }

    { // substring test
        uint8_t s[] = "hello world";
        uint8_t expected[] = "llo ";
        uint32_t len = 0;
        uint8_t *res = substring(s, 2, 5, &len);
        REQUIRE(STR_EQ(expected, res, LEN(expected)));
        REQUIRE(len == 4);
        free(res);
    }

    { // strlength test
        uint8_t s[] = "hello world!";
        REQUIRE(strlength(s) == LEN(s) - 1);
    }

    TEST_EXIT;
}
