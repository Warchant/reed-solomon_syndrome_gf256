#include "test.h"
#include "rs.h"

int main()
{
    generate_gf();

    { // encode_message test
        char msg[] = "hi";
        int t = 2;
        char expected[] = {69, 211, 228, 205, 104, 105};
        int actual_len = 0;
        char *actual = encode_message(msg, 2, t, &actual_len);
        REQUIRE(STR_EQ(expected, actual, LEN(expected)));
        REQUIRE(actual_len == LEN(expected));
        free(actual);
    }

    { // decode_message test (NO ERRORS)
        int t = 2;
        int C[] = {69, 211, 228, 205, 104, 105};
        char expected[] = "hi";
        char *codeword = vector_to_str(C, LEN(C));
        int out_len = 0;
        char *M = decode_message(codeword, LEN(C), t, &out_len);
        REQUIRE(STR_EQ(expected, M, LEN(expected)));
        REQUIRE(out_len == LEN(expected) - 1);
        free(M);
        free(codeword);
    }

    { // decode_message test (WITH 1 ERROR)
        int t = 2;
        int C[] = {69, 211, 228, 205, 1, 105};
        char expected[] = "hi";
        char *codeword = vector_to_str(C, LEN(C));
        int out_len = 0;
        char *M = decode_message(codeword, LEN(C), t, &out_len);
        REQUIRE(STR_EQ(expected, M, LEN(expected)));
        REQUIRE(out_len == LEN(expected) - 1);
        free(M);
        free(codeword);
    }

    { // decode_message test (WITH 2 ERRORS)
        int t = 2;
        int C[] = {69, 211, 228, 205, 1, 2};
        char expected[] = "hi";
        char *codeword = vector_to_str(C, LEN(C));
        int out_len = 0;
        char *M = decode_message(codeword, LEN(C), t, &out_len);
        REQUIRE(STR_EQ(expected, M, LEN(expected)));
        REQUIRE(out_len == LEN(expected) - 1);
        free(M);
        free(codeword);
    }

    TEST_EXIT;
}