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
        printf("[%d] %s\n", actual_len, actual);
        free(actual);
    }

    TEST_EXIT;
}