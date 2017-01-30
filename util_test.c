#include "util.h"
#include "test.h"

int main()
{
    { // submatrix test
        int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        int *b = submatrix(a, 3, 7);
        int c[] = {4, 5, 6, 7, 8};
        REQUIRE(AR_EQ(b, c, LEN(c)));
    }

    { // stack test
        int a[] = {0,1,2};
        int b[] = {4,5,6};
        int d[] = {0,1,2,4,5,6};
        int*c   = stack(a,3,b,3);
        REQUIRE(AR_EQ(d, c, 6));
        vector_free(c);
    }
    return total;
}

