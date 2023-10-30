#ifndef UTILS_H
#define UTILS_H

#include "spldef.h"
#include <stdio.h>

typedef enum error_type error_t;

enum error_type
{
    SPLC_ERR_CRIT,
    SPLC_ERR_UNIV, /* splc: universal error */
    SPLC_ERR_A,
    SPLC_ERR_B,
    SPLC_WARN,
    SPLC_NOTE,
    SPLMACRO_ERROR,
    SPLMACRO_WARN,
};

typedef enum trace_type trace_t;

enum trace_type
{
    SPLTR_MACRO,
    SPLTR_FILE_INCL,
    SPLTR_STRUCT,
    SPLTR_FUNCTION,
};

/* This method prints a message, indicating in which structure the error lies. If show_source is zero, then do not display source file. */
void spltrace(trace_t type, int show_source, const char *name);

/* Print an error without position */
void splerror_nopos(error_t type, const char *msg);

/* This method prints the corresponding colored message, and outputs the corresponding line in the file.
   
   The colbegin and colend does not have to be necessarily at the same line, since the function will
   output only one line.
 */
void splerror(error_t type, int linebegin, int colbegin, int lineend, int colend, const char *msg);

void splwarn(int linebegin, int colbegin, int lineend, int colend, const char *msg);

void splnote(int linebegin, int colbegin, int lineend, int colend, const char *msg);

/* When switching parser into parsing a new file, this function must be called to preserve the previously opened files. 
   Return 0 on success, else there is an error to be handled. */
int spl_enter_root(const char *restrict _filename);

/* When switching parser into parsing a new file, this function must be called to preserve the previously opened files. 
   Return 0 on success, else there is an error to be handled. */
int spl_enter_file(const char *restrict _filename, int linebegin, int colbegin, int lineend, int colend);

/* When the parser finishes parsing a new file and returned, this function must be called to starting parsing on the previously opened files.
   If there is still file left, return 0. Else return nonzero. */
int spl_exit_file();

void set_error_flag(int val);

#endif