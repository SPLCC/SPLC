#ifndef UTILS_H
#define UTILS_H

#include "spldef.h"
#include <stdio.h>

typedef enum error_type error_t;

enum error_type
{
    SPLC_WARN,
    SPLC_NOTE,
    SPLMACRO_ERROR,
    SPLMACRO_WARN,

    SPLC_ERR_CRIT = 0x1000,
    SPLC_ERR_UNIV, /* splc: universal error */
    SPLC_ERR_A,
    SPLC_ERR_B,
};

#define SPLC_ERR_MASK 0x1000

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
void splerror(error_t type, const spl_loc location, const char *msg);

void splwarn(const spl_loc location, const char *msg);

void splnote(const spl_loc location, const char *msg);

/* When switching parser into parsing a new file, this function must be called to preserve the previously opened files. 
   Return 0 on success, else there is an error to be handled. */
int spl_enter_root(const char *restrict _filename);

/* When switching parser into parsing a new file, this function must be called to preserve the previously opened files. 
   Return 0 on success, else there is an error to be handled.
   Specify where the file is included in the previous file in the global stack */
int spl_enter_file(const char *restrict _filename, const spl_loc location);

/* When the parser finishes parsing a new file and returned, this function must be called to starting parsing on the previously opened files.
   If there is still file left, return 0. Else return nonzero.
   The file node will be kept, with the information of entry preserved. */
int spl_exit_file();

void set_error_flag(int val);

#endif