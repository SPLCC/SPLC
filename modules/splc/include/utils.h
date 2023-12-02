#ifndef UTILS_H
#define UTILS_H

#include "msg_type.h" // Include message types
#include "splcdef.h"
#include <stdio.h>

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
void splctrace(const trace_t type, int show_source, const char *name);

/* This method prints the corresponding colored message, and outputs the corresponding line in the file.

   The colbegin and colend does not have to be necessarily at the same line, since the function will
   output only one line.
   If location is NULL, then file trace will not be printed. */
void splc_internal_handle_msg(const splc_msg_t type, const splc_loc location, const char *msg);

/* When switching parser into parsing a new file, this function must be called to preserve the previously opened files.
   Return 0 on success, else there is an error to be handled. */
int splc_enter_root(const char *restrict _filename);

/* When switching parser into parsing a new buffer, this function must be called to preserve the previously opened files.
   Return 0 on success, else there is an error to be handled.
   Specify where the buffer is included in the previous file in the global stack */
int splc_push_file_buffer(const char *restrict _filename, const splc_loc location);

/* When switching parser into parsing a new buffer, this function must be called to preserve the previously opened files.
   Return 0 on success, else there is an error to be handled.
   Specify where the buffer is included in the previous file in the global stack */
int splc_push_char_buffer(const char *restrict _macroname, const splc_loc location, const char *content);

/* When the parser finishes parsing a new buffer and returned, this function must be called to starting parsing on the
   previously opened buffers. If there is still file left, return 0. Else return nonzero. The file node will be kept, with
   the information of entry preserved. */
int splc_pop_buffer();

/* Return the buffername of the node. This string shall not be freed. */
const char *const splc_get_buffer_node_name(int fid);

void update_error(int val);

void update_warning(int val);

/* Global variables */

extern int splcf_verbose;

extern int splcf_no_diagnostics_color;

/* Macros */

#define SPLC_FAIL(_msg)                                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stderr, "\033[1m%s\033[0m: \033[31mfatal error\033[0m: " _msg "\n", progname);                         \
        abort();                                                                                                       \
    } while (0)

/* Call this to print a formatted message and immediately fail */
#define SPLC_FFAIL(_msg, ...)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stderr, "\033[1m%s\033[0m: \033[31mfatal error\033[0m: " _msg "\n", progname, __VA_ARGS__);            \
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
        splc_internal_handle_msg(SPLM_ERR_FATAL, SPLC_INVALID_LOC, buffer);                                            \
        free(buffer);                                                                                                  \
        exit(1);                                                                                                       \
    } while (0)

/* Call this to print an error */
#define SPLC_ERROR(type, _location, _msg)                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = strlen(_msg) + 1;                                                                              \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        memcpy(buffer, _msg, needed);                                                                                  \
        splc_internal_handle_msg(type, _location, buffer);                                                             \
        free(buffer);                                                                                                  \
    } while (0)

/* Call this to print an error without location */
#define SPLC_ERROR_NOLOC(type, _msg)                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = strlen(_msg) + 1;                                                                              \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        memcpy(buffer, _msg, needed);                                                                                  \
        splc_internal_handle_msg(type, SPLC_INVALID_LOC, buffer);                                                      \
        free(buffer);                                                                                                  \
    } while (0)

/* Call this to print a formatted error */
#define SPLC_FERROR(type, _location, _msg, ...)                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = snprintf(NULL, 0, _msg, __VA_ARGS__) + 1;                                                      \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        sprintf(buffer, _msg, __VA_ARGS__);                                                                            \
        splc_internal_handle_msg(type, _location, buffer);                                                             \
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
        splc_internal_handle_msg(type, SPLC_INVALID_LOC, buffer);                                                      \
        free(buffer);                                                                                                  \
    } while (0)

/* Call this to print a warning */
#define SPLC_WARN(type, _location, _msg)                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = strlen(_msg) + 1;                                                                              \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        memcpy(buffer, _msg, needed);                                                                                  \
        splc_internal_handle_msg(type, _location, buffer);                                                             \
        free(buffer);                                                                                                  \
    } while (0)

/* Call this to print a warning without location */
#define SPLC_WARN_NOLOC(type, _msg)                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = strlen(_msg) + 1;                                                                              \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        memcpy(buffer, _msg, needed);                                                                                  \
        splc_internal_handle_msg(type, SPLC_INVALID_LOC, buffer);                                                      \
        free(buffer);                                                                                                  \
    } while (0)

/* Call this to print a formatted warning */
#define SPLC_FWARN(type, _location, _msg, ...)                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = snprintf(NULL, 0, _msg, __VA_ARGS__) + 1;                                                      \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        sprintf(buffer, _msg, __VA_ARGS__);                                                                            \
        splc_internal_handle_msg(type, _location, buffer);                                                             \
        free(buffer);                                                                                                  \
    } while (0)

/* Call this to print a formatted warning */
#define SPLC_FWARN_NOLOC(type, _msg, ...)                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = snprintf(NULL, 0, _msg, __VA_ARGS__) + 1;                                                      \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                     \
        sprintf(buffer, _msg, __VA_ARGS__);                                                                            \
        splc_internal_handle_msg(type, SPLC_INVALID_LOC, buffer);                                                      \
        free(buffer);                                                                                                  \
    } while (0)

#ifndef SPLC_DISABLE_DIAG

/* Call this to print a diagnostic message */
#define SPLC_DIAG(_msg)                                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        size_t needed = strlen(_msg) + 1;                                                                              \
        char *buffer = (char *)malloc(needed);                                                                         \
        SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing message");                                   \
        memcpy(buffer, _msg, needed);                                                                                  \
        splc_internal_handle_msg(SPLM_DIAG, SPLC_INVALID_LOC, buffer);                                                 \
        free(buffer);                                                                                                  \
    } while (0)

/* Call this to print a formatted diagnostic message */
#define SPLC_FDIAG(_msg, ...)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        if (splcf_verbose)                                                                                             \
        {                                                                                                              \
            size_t needed = snprintf(NULL, 0, _msg, __VA_ARGS__) + 1;                                                  \
            char *buffer = (char *)malloc(needed);                                                                     \
            SPLC_ALLOC_PTR_CHECK(buffer, "cannot allocate memory for printing error");                                 \
            sprintf(buffer, _msg, __VA_ARGS__);                                                                        \
            splc_internal_handle_msg(SPLM_DIAG, SPLC_INVALID_LOC, buffer);                                             \
            free(buffer);                                                                                              \
        }                                                                                                              \
    } while (0)

#else

#define SPLC_DIAG(_msg)
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