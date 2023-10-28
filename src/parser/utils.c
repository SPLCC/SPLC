#include "utils.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_error_flag(int val)
{
    err_flag = val;
}

/* Return an array of lines fetched. No newline character will be present. */
static char *fetchline(FILE *file, int linebegin)
{
    char *lineptr = (char *)NULL;
    for (int cur = 0; cur < linebegin; ++cur)
    {
        lineptr = (char *)NULL;
        size_t n = 0;
        getline(&lineptr, &n, file);
    }
    /* Handling newline and EOF */
    size_t linelen = strlen(lineptr);
    if (linelen > 0 && lineptr[linelen - 1] == '\n')
        lineptr[linelen - 1] = '\0';
    return lineptr;
}

static const char *get_color_code(error_t type)
{
    const char *color_code = "\033[31m";
    switch (type)
    {
    case SPLERR_CRIT:
    case SPLERR_A:
    case SPLERR_B:
        color_code = "\033[31m";
        break;
    case SPLWARN:
        color_code = "\033[35m";
        break;
    default:
        color_code = "\033[36m";
        break;
    }
    return color_code;
}

static void print_colored_line(error_t type, const char *line, int linebegin, int colbegin, int colend)
{
    const char *color_code = get_color_code(type);
    printf("%8d |", linebegin);

    for (int i = 0; i < colbegin - 1; ++i)
        printf("%c", line[i]);

    printf("%s", color_code);
    for (int i = colbegin - 1; i < colend; ++i)
        printf("%c", line[i]);
    printf("\033[0m");

    for (int i = colend; line[i] != '\0'; ++i)
        printf("%c", line[i]);
    printf("\n");
}

static void print_indicator(error_t type, int colbegin, int colend)
{
    const char *color_code = get_color_code(type);

    printf("         |");

    for (int i = 1; i < colbegin; ++i)
        printf(" ");

    printf("%s^", color_code);
    for (int i = colbegin + 1; i <= colend; ++i)
        printf("~");
    printf("\033[0m\n");

    return;
}

void spltrace(trace_t type, const char *name)
{
    const char *type_str = "UNDEFINED";
    switch (type)
    {
    case SPLTR_FUNCTION:
        type_str = "function";
        break;
    case SPLTR_STRUCT:
        type_str = "struct";
        break;
    default:
        type_str = "unknown structure";
        break;
    }
    fprintf(stderr, "%s: In %s `%s`\n", filename, type_str, name);
    return;
}

static void spl_handle_msg(error_t type, int linebegin, int colbegin, int lineend, int colend, const char *msg)
{
    const char *color_code = get_color_code(type);
    const char *type_name = "undefined message";
    const char *type_suffix = "UNDEFINED";
    switch (type)
    {
    case SPLERR_CRIT:
        type_name = "critical error";
        type_suffix = "CRITICAL";
        break;
    case SPLERR_A:
        type_name = "error";
        type_suffix = "A";
        break;
    case SPLERR_B:
        type_name = "error";
        type_suffix = "B";
        break;
    case SPLWARN:
        type_name = "warning";
        type_suffix = "WARNING";
        break;
    default:
        type_name = "critical error";
        type_suffix = "UNKNOWN";
        break;
    }
    fprintf(stderr, "%s:%d:%d: %s%s:\033[0m %s [%s%s\033[0m]\n", filename, linebegin, colbegin, color_code, type_name,
            msg, color_code, type_suffix);

    FILE *file = NULL;
    if ((file = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "spl_parser: \033[31merror:\033[0m %s: file no longer exists\n", filename);
        return;
    }
    char *line = fetchline(file, linebegin);
    fclose(file);

    int line_len = (int)strlen(line);

    int t_colend = colbegin;
    if (lineend == linebegin && colend > colbegin)
        t_colend = colend - 1;
    else if (lineend != linebegin)
        t_colend = line_len;

    print_colored_line(type, line, linebegin, colbegin, t_colend);
    print_indicator(type, colbegin, t_colend);
    free(line);

    return;
}

void splerror(error_t type, int linebegin, int colbegin, int lineend, int colend, const char *msg)
{
    set_error_flag(1);
    spl_handle_msg(type, linebegin, colbegin, lineend, colend, msg);
}

void splwarn(int linebegin, int colbegin, int lineend, int colend, const char *msg)
{
    spl_handle_msg(SPLWARN, linebegin, colbegin, lineend, colend, msg);
}
