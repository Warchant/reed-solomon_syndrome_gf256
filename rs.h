#pragma once

#include "util.h"
#include "gf.h"

// takes `message` with length=`len`, adds exactly 2*t control symbols and
// writes output to `output`. output will be `len+2*t` bytes long
void encode_message(char *message, int len, char *output, int t)
{
    if (t > 2)
        fatal("encode_message", "t > 2 is not implemented");

    int k = len;
    int n = 2 * t + k;

    int *g = vector_new(2);
    g[0] = alpha_of[1]; // 2
    g[1] = alpha_of[0]; // 1

    if(t >= 2){

    }
    
}