#include "rs_internal.h"
#include "test.h"

int main()
{
    generate_gf();

    { // generating_polynomial_test t=2
        int t = 2;
        int G[] = {116, 231, 216, 30, 1};
        int g_len = 0;
        int *g = generating_polynomial(t, &g_len);
        REQUIRE(AR_EQ(G, g, LEN(G)));
        REQUIRE(g_len == LEN(G));
        vector_free(g);
    }

    { // generating_polynomial_test t=1
        int t = 1;
        int G[] = {2, 1};
        int g_len = 0;
        int *g = generating_polynomial(t, &g_len);
        REQUIRE(AR_EQ(G, g, LEN(G)));
        REQUIRE(g_len == LEN(G));
        vector_free(g);
    }

    { // shift_msg_to_vec t=2
        int t = 2;
        int a[] = {0, 0, 0, 0, 104, 105};
        int b_len = 0;
        int *b = shift_msg_to_vec("hi", 2, t, &b_len);
        REQUIRE(AR_EQ(a, b, LEN(a)));
        vector_free(b);
    }

    { // shift_msg_to_vec t=1
        int t = 1;
        int a[] = {0, 0, 104, 105};
        int b_len = 0;
        int *b = shift_msg_to_vec("hi", 2, t, &b_len);
        REQUIRE(AR_EQ(a, b, LEN(a)));
        vector_free(b);
    }

    { // calculate_check_symbols t=2
        int t = 2;
        int v[] = {69, 211, 228, 205, 0, 0};
        int m_len = 0;
        int *M = shift_msg_to_vec("hi", 2, t, &m_len);
        int g_len = 0;
        int *G = generating_polynomial(t, &g_len);
        int a_len = 0;
        int *a = calculate_check_symbols(M, m_len, G, g_len, t, &a_len);
        REQUIRE(AR_EQ(v, a, LEN(v)));
        vector_free(G);
        vector_free(a);
    }

    { // calculate syndrome test
        int t = 2;
        int expected[] = {130, 153, 245, 88};
        int c1[] = {192, 211, 44, 205, 104, 105};
        int actual_len = 0;
        int *actual = calculate_syndrome(c1, LEN(c1), t, &actual_len);
        REQUIRE(AR_EQ(actual, expected, LEN(expected)));
        REQUIRE(actual_len == LEN(expected));
        vector_free(actual);
    }

    TEST_EXIT;
}