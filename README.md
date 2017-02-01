# Description

C implementation of Reed-Solomon error-correction codes. For detection syndrome decoding is used. Works in GF(256).

- highly portable, includes only `stdio.h` and `stdlib.h`
- header-only
- syndrom decoding
- implemented for t=1 and t=2 (can locate and correct 1 or 2 errors), probably I'll implement it for more
- tested on Ubuntu and Arduino Nano (for Arduino, change contents of `fatal` and `vector_print` methods from `util.h`, because it has no `printf` and `exit` methods) 

Probably this code will be updated once.

# How to use

## To build and run tests: 
```bash
mkdir build
cd build
cmake ..
make
ctest
```

## How to use:
```C
#include "rs.h"

int main(){
    // sender side
    uint8_t* msg = (uint8_t*)"hello world";
    uint8_t t = 2; // we want to fix 2 mistakes at max
    uint32_t len = 0; // new length of codeword will be 2*t + len(msg)
    uint8_t *codeword = encode_message(msg, strlength(msg), t, &len); // yes, I know about strlen. for this I need to include one more header
    
    // send codeword through noisy channel

    // receiver side
    uint8_t out_len = 0; // length of decoded message
    uint8_t *corrected_msg = decode_message(codeword, len, t, &out_len);
    printf("%s\n", corrected_msg); // "hello world"

    return 0;
}
```
