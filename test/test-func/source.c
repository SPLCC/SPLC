#include "header.h"

#define Bmacro c

#define AB \
        a + Bmacro

// #define RECURSIVE RECURSIVE

int main()
{
    // printf("lorem\n");
    int a, b;
    AB;
    return 0;
}