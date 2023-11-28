#ifndef UTILS_H
#define UTILS_H

#include "splcdef.h"
#include <stdio.h>

typedef enum error_type error_t;

enum error_type
{
    SPLC_WARN,
    SPLC_NOTE,
    SPLC_DIAG, /* splc: diagnostic message */
    SPLC_MACRO_ERROR,
    SPLC_MACRO_WARN,

    SPLC_ERR_FATAL = 0x1000,
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

/* This method prints a message, indicating in which structure the error lies. If show_source is zero, then do not
 * display source file. */
void splctrace(trace_t type, int show_source, const char *name);

void splcfail(const char *msg);

/* This method prints the corresponding colored message, and outputs the corresponding line in the file.

   The colbegin and colend does not have to be necessarily at the same line, since the function will
   output only one line.
 */
void splcerror(error_t type, const splc_loc location, const char *msg);

/* Print an error without location */
void splcerror_noloc(error_t type, const char *msg);

void splcwarn(const splc_loc location, const char *msg);

/* Print a warning without location */
void splcwarn_noloc(const char *msg);

void splcnote(const splc_loc location, const char *msg);

void splcdiag(const char *msg);

/* When switching parser into parsing a new file, this function must be called to preserve the previously opened files.
   Return 0 on success, else there is an error to be handled. */
int splc_enter_root(const char *restrict _filename);

/* When switching parser into parsing a new file, this function must be called to preserve the previously opened files.
   Return 0 on success, else there is an error to be handled.
   Specify where the file is included in the previous file in the global stack */
int splc_enter_file(const char *restrict _filename, const splc_loc location);

/* When the parser finishes parsing a new file and returned, this function must be called to starting parsing on the
   previously opened files. If there is still file left, return 0. Else return nonzero. The file node will be kept, with
   the information of entry preserved. */
int splc_exit_file();

/* Return the filename of the node. This string shall not be freed. */
const char *const splc_get_node_filename(int fid);

void update_error(int val);

void update_warning(int val);

/* Global variables */

extern int splcf_enable_diag;

extern int splcf_no_diagnostics_color;

/* Macros */

/* Call this to print a formatted message and immediately fail */
#define SPLC_FFAIL(_msg, ...)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stderr, "\033[1m%s\033[0m: \033[31mfatal error\033[0m: " _msg "\n", progname, __VA_ARGS__);             \
        abort();                                                                                                       \
    } while (0)

/* Check if pointer is NULL. If fail, print a message and exit. */
#define SPLC_ALLOC_PTR_CHECK(x, _msg)                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((x) == NULL)                                                                                               \
        {                                                                                                              \
            fprintf(stderr, "\033[1m%s\033[0m: \033[31mfatal error\033[0m: %s\n", progname, _msg);                     \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (0)

/* Print a formatted message without location and exit splc(1) */
#define SPLC_FEXIT_NOLOC(_msg, ...)                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = snprintf(NULL, 0, _msg, __VA_ARGS__) + 1;                                                      \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        sprintf(buffer, _msg, __VA_ARGS__);                                                                            \
        splcerror_noloc(SPLC_ERR_FATAL, buffer);                                                                       \
        free(buffer);                                                                                                  \
        exit(1);                                                                                                       \
    } while (0)

/* Call this to print a formatted error */
#define SPLC_FERROR(type, _location, _msg, ...)                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = snprintf(NULL, 0, _msg, __VA_ARGS__) + 1;                                                      \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        sprintf(buffer, _msg, __VA_ARGS__);                                                                            \
        splcerror(type, _location, buffer);                                                                            \
        free(buffer);                                                                                                  \
    } while (0)

/* Call this to print a formatted error without location */
#define SPLC_FERROR_NOLOC(type, _msg, ...)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = snprintf(NULL, 0, _msg, __VA_ARGS__) + 1;                                                      \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        sprintf(buffer, _msg, __VA_ARGS__);                                                                            \
        splcerror_noloc(type, buffer);                                                                                 \
        free(buffer);                                                                                                  \
    } while (0)

/* Call this to print a formatted warning */
#define SPLC_FWARN(_location, _msg, ...)                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = snprintf(NULL, 0, _msg, __VA_ARGS__) + 1;                                                      \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        sprintf(buffer, _msg, __VA_ARGS__);                                                                            \
        splcwarn(_location, buffer);                                                                                   \
        free(buffer);                                                                                                  \
    } while (0)

/* Call this to print a formatted warning */
#define SPLC_FWARN_NOLOC(_msg, ...)                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = snprintf(NULL, 0, _msg, __VA_ARGS__) + 1;                                                      \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        sprintf(buffer, _msg, __VA_ARGS__);                                                                            \
        splcwarn_noloc(buffer);                                                                                        \
        free(buffer);                                                                                                  \
    } while (0)

#ifndef SPLC_DISABLE_DIAG
/* Call this to print a formatted diagnostic message */
#define SPLC_FDIAG(_msg, ...)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = snprintf(NULL, 0, _msg, __VA_ARGS__) + 1;                                                      \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        sprintf(buffer, _msg, __VA_ARGS__);                                                                            \
        splcdiag(buffer);                                                                                              \
        free(buffer);                                                                                                  \
    } while (0)
#else
#define SPLC_FDIAG(_msg, ...)
#endif // SPLC_DISABLE_DIAG

#define SPLC_ASSERT(cond)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(cond))                                                                                                   \
        {                                                                                                              \
            SPLC_FFAIL("on \033[1m%s:%d\033[0m, " #cond "\nassertion failed", __FILE__, __LINE__);                     \
        }                                                                                                              \
    } while (0)

#endif /* UTILS_H */