#ifndef UTILS_H
#define UTILS_H

#include "spldef.h"
#include <stdio.h>

typedef enum error_type error_t;

enum error_type
{
    CRITICAL,
    ERR_A,
    ERR_B,
};

/* This method prints the corresponding colored message, and outputs the corresponding line in the file.
   
   The colbegin and colend does not have to be necessarily at the same line, since the function will
   output only one line.
 */
void splerror(error_t type, int linebegin, int colbegin, int lineend, int colend, const char *msg);

#endif