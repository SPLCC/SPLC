#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void splerror(error_t type, int lineno, const char *msg, const char *reason)
{
    char *type_str = "UNDEFINED";
    switch (type)
    {
    case ERR_A:
        type_str = "A";
        break;
    case ERR_B:
        type_str = "B";
        break;
    default : type_str = "UNKNOWN";
        break;
    }
    fprintf(stderr, "Error type %s at Line %d: %s%s%s\n", type_str, lineno, msg, reason == NULL ? "" : " ", reason == NULL ? "" : reason);

    err_flag = 1;
}
