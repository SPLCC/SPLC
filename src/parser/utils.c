#include "utils.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Return an array of lines fetched */
static char *fetchline(FILE *file, int linebegin)
{
    char *lineptr = (char *)NULL;
    for (int cur = 0; cur < linebegin; ++cur)
    {
        lineptr = (char *)NULL;
        size_t n = 0;
        getline(&lineptr, &n, file);
    }
    return lineptr;
}

static void print_colored_line(error_t type, const char *line, int linebegin, int colbegin, int colend)
{
    const char *color_code = "\033[31m";
    switch (type)
    {
    case CRITICAL:
    case ERR_A:
    case ERR_B:
        color_code = "\033[31m";
        break;
    default:
        color_code = "\033[36m";
        break;
    }

    printf("%8d |", linebegin);

    for (int i = 0; i < colbegin - 1; ++i)
        printf("%c", line[i]);

    printf("%s", color_code);
    for (int i = colbegin - 1; i < colend; ++i)
        printf("%c", line[i]);
    printf("\033[0m");

    for (int i = colend; line[i] != '\0'; ++i)
        printf("%c", line[i]);
}

static void print_indicator(error_t type, int colbegin, int colend)
{
    const char *color_code = "\033[31m";
    switch (type)
    {
    case CRITICAL:
    case ERR_A:
    case ERR_B:
        color_code = "\033[31m";
        break;
    default:
        color_code = "\033[36m";
        break;
    }

    printf("         |");

    for (int i = 1; i < colbegin; ++i)
        printf(" ");

    printf("%s^", color_code);
    for (int i = colbegin + 1; i <= colend; ++i)
        printf("~");
    printf("\033[0m\n");

    return;
}

void splerror(error_t type, int linebegin, int colbegin, int lineend, int colend, const char *msg)
{
    char *type_str = "UNDEFINED";
    err_flag = 1;
    switch (type)
    {
    case CRITICAL:
        type_str = "CRITICAL";
        break;
    case ERR_A:
        type_str = "A";
        break;
    case ERR_B:
        type_str = "B";
        break;
    default:
        type_str = "UNKNOWN";
        break;
    }
    fprintf(stderr, "%s:%d:%d: \033[31merror:\033[0m %s [\033[31m%s\033[0m]\n", filename, linebegin, colbegin, msg,
            type_str);

    FILE *file = NULL;
    if ((file = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "spl_parser: \033[31merror:\033[0m %s: file no longer exists\n", filename);
        return;
    }
    char *line = fetchline(file, linebegin);
    int line_len = (int)strlen(line);
    fclose(file);
    print_colored_line(type, line, linebegin, colbegin, lineend == linebegin ? colend : line_len);
    print_indicator(type, colbegin, lineend == linebegin ? colend : line_len);
    free(line);

    return;
}
