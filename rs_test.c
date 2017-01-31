#include "test.h"
#include "rs.h"

uint32_t main()
{
    generate_gf();

    { // encode_message test
        uint8_t msg[] = "hi";
        uint32_t t = 2;
        uint8_t expected[] = {69, 211, 228, 205, 104, 105};
        uint32_t actual_len = 0;
        uint8_t *actual = encode_message(msg, 2, t, &actual_len);
        REQUIRE(STR_EQ(expected, actual, LEN(expected)));
        REQUIRE(actual_len == LEN(expected));
        free(actual);
    }

    { // decode_message test (NO ERRORS)
        uint32_t t = 2;
        uint32_t C[] = {69, 211, 228, 205, 104, 105};
        uint8_t expected[] = "hi";
        uint8_t *codeword = vector_to_str(C, LEN(C));
        uint32_t out_len = 0;
        uint8_t *M = decode_message(codeword, LEN(C), t, &out_len);
        REQUIRE(STR_EQ(expected, M, LEN(expected)));
        REQUIRE(out_len == LEN(expected) - 1);
        free(M);
        free(codeword);
    }

    { // decode_message test (WITH 1 ERROR)
        uint32_t t = 2;
        uint32_t C[] = {69, 211, 228, 205, 1, 105};
        uint8_t expected[] = "hi";
        uint8_t *codeword = vector_to_str(C, LEN(C));
        uint32_t out_len = 0;
        uint8_t *M = decode_message(codeword, LEN(C), t, &out_len);
        REQUIRE(STR_EQ(expected, M, LEN(expected)));
        REQUIRE(out_len == LEN(expected) - 1);
        free(M);
        free(codeword);
    }

    { // decode_message test (WITH 2 ERRORS)
        uint32_t t = 2;
        uint32_t C[] = {69, 211, 228, 205, 1, 2};
        uint8_t expected[] = "hi";
        uint8_t *codeword = vector_to_str(C, LEN(C));
        uint32_t out_len = 0;
        uint8_t *M = decode_message(codeword, LEN(C), t, &out_len);
        REQUIRE(STR_EQ(expected, M, LEN(expected)));
        REQUIRE(out_len == LEN(expected) - 1);
        free(M);
        free(codeword);
    }

    { // encode
        uint32_t t = 2;
        uint8_t msg[] = "hello";
        uint32_t msg_len = LEN(msg) - 1;
        uint32_t len = 0;
        uint8_t *codeword = encode_message(msg, msg_len, t, &len);
        printf("%s\n", codeword);
        uint8_t *M = decode_message(codeword, len, t, &len);
        REQUIRE(STR_EQ(msg, M, msg_len));
        REQUIRE(len == msg_len);
        free(codeword);
        free(M);
    }

    { // encode
        uint32_t t = 1;
        uint8_t msg[] = "hello";
        uint32_t msg_len = LEN(msg) - 1;
        uint32_t len = 0;
        uint8_t *codeword = encode_message(msg, msg_len, t, &len);
        printf("%s\n", codeword);
        uint8_t *M = decode_message(codeword, len, t, &len);
        REQUIRE(STR_EQ(msg, M, msg_len));
        REQUIRE(len == msg_len);
        free(codeword);
        free(M);
    }

    TEST_EXIT;
}