#include "rs_internal.h"
#include "test.h"

uint32_t main()
{
    generate_gf();

    { // generating_polynomial_test t=2
        uint32_t t = 2;
        uint32_t G[] = {116, 231, 216, 30, 1};
        uint32_t g_len = 0;
        uint32_t *g = generating_polynomial(t, &g_len);
        REQUIRE(AR_EQ(G, g, LEN(G)));
        REQUIRE(g_len == LEN(G));
        vector_free(g);
    }

    { // generating_polynomial_test t=1
        uint32_t t = 1;
        uint32_t G[] = {2, 1};
        uint32_t g_len = 0;
        uint32_t *g = generating_polynomial(t, &g_len);
        REQUIRE(AR_EQ(G, g, LEN(G)));
        REQUIRE(g_len == LEN(G));
        vector_free(g);
    }

    { // shift_msg_to_vec t=2
        uint32_t t = 2;
        uint32_t a[] = {0, 0, 0, 0, 104, 105};
        uint32_t b_len = 0;
        uint32_t *b = shift_msg_to_vec("hi", 2, t, &b_len);
        REQUIRE(AR_EQ(a, b, LEN(a)));
        vector_free(b);
    }

    { // shift_msg_to_vec t=1
        uint32_t t = 1;
        uint32_t a[] = {0, 0, 104, 105};
        uint32_t b_len = 0;
        uint32_t *b = shift_msg_to_vec("hi", 2, t, &b_len);
        REQUIRE(AR_EQ(a, b, LEN(a)));
        vector_free(b);
    }

    { // calculate_check_symbols t=2
        uint32_t t = 2;
        uint32_t v[] = {69, 211, 228, 205, 0, 0};
        uint32_t m_len = 0;
        uint32_t *M = shift_msg_to_vec("hi", 2, t, &m_len);
        uint32_t g_len = 0;
        uint32_t *G = generating_polynomial(t, &g_len);
        uint32_t a_len = 0;
        uint32_t *a = calculate_check_symbols(M, m_len, G, g_len, t, &a_len);
        REQUIRE(AR_EQ(v, a, LEN(v)));
        vector_free(G);
        vector_free(a);
        vector_free(M);
    }

    { // calculate syndrome test
        uint32_t t = 2;
        uint32_t expected[] = {130, 153, 245, 88};
        uint32_t c1[] = {192, 211, 44, 205, 104, 105};
        uint32_t actual_len = 0;
        uint32_t *actual = calculate_syndrome(c1, LEN(c1), t, &actual_len);
        REQUIRE(AR_EQ(actual, expected, LEN(expected)));
        REQUIRE(actual_len == LEN(expected));
        vector_free(actual);
    }

    { // generator test
        uint32_t t = 2;
        uint32_t size = 0;
        uint32_t *q = generating_polynomial(t, &size);
        uint32_t *w = generating_polynomial(t, NULL);
        uint32_t *e = generating_polynomial(t, NULL);
        uint32_t *r = generating_polynomial(t, NULL);

        REQUIRE(AR_EQ(q,w,size));
        REQUIRE(AR_EQ(w,e,size));
        REQUIRE(AR_EQ(e,r,size));
        REQUIRE(AR_EQ(r,q,size));

        vector_free(q);
        vector_free(w);
        vector_free(e);
        vector_free(r);
    }

    TEST_EXIT;
}