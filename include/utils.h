#ifndef UTILS_H
#define UTILS_H

#include "spldef.h"

typedef enum error_type error_t;

enum error_type
{
    ERR_A,
    ERR_B,
};

void splerror(error_t type, int lineno, const char *msg, const char *reason);

#endif