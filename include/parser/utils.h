#ifndef UTILS_H
#define UTILS_H

#include "spldef.h"
#include <stdio.h>

typedef enum error_type error_t;

enum error_type
{
    SPLERR_CRIT,
    SPLERR_A,
    SPLERR_B,
    SPLWARN,
};

typedef enum trace_type trace_t;

enum trace_type
{
    SPLTR_STRUCT,
    SPLTR_FUNCTION,
};

/* This method prints a message, indicating in which structure the error lies. */
void spltrace(trace_t type, const char *name);

/* This method prints the corresponding colored message, and outputs the corresponding line in the file.
   
   The colbegin and colend does not have to be necessarily at the same line, since the function will
   output only one line.
 */
void splerror(error_t type, int linebegin, int colbegin, int lineend, int colend, const char *msg);

void splwarn(int linebegin, int colbegin, int lineend, int colend, const char *msg);

void set_error_flag(int val);

#endif