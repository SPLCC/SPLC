#include "utils.h"
#include "lex.yy.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int spl_include_dir_counter = 0;

const char **spl_include_directories = NULL;

int spl_file_counter = 0;

util_file_node *spl_all_file_nodes = NULL;

util_file_node spl_file_stack = NULL;

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
    {
        lineptr[linelen - 1] = '\0';
        linelen--;
    }
    if (linelen > 0 && lineptr[linelen - 1] == '\r')
    {
        lineptr[linelen - 1] = '\0';
        linelen--;
    }
    return lineptr;
}

static const char *get_spl_error_color_code(error_t type)
{
    const char *color_code = "\033[31m";
    switch (type)
    {
    case SPLC_ERR_CRIT:
    case SPLC_ERR_UNIV:
    case SPLC_ERR_A:
    case SPLC_ERR_B:
    case SPLMACRO_ERROR:
        color_code = "\033[31m";
        break;
    case SPLC_WARN:
    case SPLMACRO_WARN:
        color_code = "\033[35m";
        break;
    case SPLC_NOTE:
    default:
        color_code = "\033[36m";
        break;
    }
    return color_code;
}

static void print_colored_line(error_t type, const char *line, int linebegin, int colbegin, int colend)
{
    const char *color_code = get_spl_error_color_code(type);
    fprintf(stderr, "%8d |", linebegin);

    for (int i = 0; i < colbegin - 1; ++i)
        fprintf(stderr, "%c", line[i]);

    fprintf(stderr, "%s", color_code);
    for (int i = colbegin - 1; i < colend; ++i)
        fprintf(stderr, "%c", line[i]);
    fprintf(stderr, "\033[0m");

    for (int i = colend; line[i] != '\0'; ++i)
        fprintf(stderr, "%c", line[i]);
    fprintf(stderr, "\n");
}

static void print_indicator(error_t type, int colbegin, int colend)
{
    // fprintf(stderr, "Accepted parameters: %d %d\n", colbegin, colend);
    const char *color_code = get_spl_error_color_code(type);

    fprintf(stderr, "         |");

    for (int i = 1; i < colbegin; ++i)
        fprintf(stderr, " ");

    fprintf(stderr, "%s^", color_code);
    for (int i = colbegin + 1; i <= colend; ++i)
        fprintf(stderr, "~");
    fprintf(stderr, "\033[0m\n");

    return;
}

void spltrace(trace_t type, int show_source, const char *name)
{
    const char *type_str = "UNDEFINED";
    switch (type)
    {
    case SPLTR_MACRO:
        type_str = "macro";
        break;
    case SPLTR_FILE_INCL:
        type_str = "file included ";
        break;
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
    fprintf(stderr, "%s%sIn %s `%s`:\n", show_source != 0 ? spl_file_stack->filename : "", show_source != 0 ? ": " : "",
            type_str, name);
    return;
}

static char *spl_get_msg_type_prefix(error_t type)
{
    const char *type_name = "undefined message";
    switch (type)
    {
    case SPLC_ERR_CRIT:
        type_name = "critical error";
        break;
    case SPLC_ERR_UNIV:
    case SPLC_ERR_A:
    case SPLC_ERR_B:
        type_name = "error";
        break;
    case SPLC_WARN:
        type_name = "warning";
        break;
    case SPLC_NOTE:
        type_name = "note";
        break;
    case SPLMACRO_ERROR:
        type_name = "error";
        break;
    case SPLMACRO_WARN:
        type_name = "warning";
        break;
    default:
        type_name = "critical error";
        break;
    }
    return strdup(type_name);
}

static char *spl_get_msg_type_suffix(error_t type)
{
    char *type_suffix = NULL;
    switch (type)
    {
    case SPLC_ERR_CRIT:
        type_suffix = NULL;
        break;
    case SPLC_ERR_UNIV:
        type_suffix = NULL;
    case SPLC_ERR_A:
        type_suffix = "A";
        break;
    case SPLC_ERR_B:
        type_suffix = "B";
        break;
    case SPLC_WARN:
        type_suffix = NULL;
        break;
    case SPLC_NOTE:
        type_suffix = NULL;
        break;
    case SPLMACRO_ERROR:
        type_suffix = "Wmacro-error";
        break;
    case SPLMACRO_WARN:
        type_suffix = "Wmacro-warning";
        break;
    default:
        type_suffix = NULL;
        break;
    }
    return (type_suffix != NULL) ? strdup(type_suffix) : NULL;
}

static void _builtin_spl_handle_msg_noloc(error_t type, const char *msg)
{
    const char *color_code = get_spl_error_color_code(type);
    char *type_name = spl_get_msg_type_prefix(type);
    char *type_suffix = spl_get_msg_type_suffix(type);
    char *filename = (spl_file_stack != NULL) ? spl_file_stack->filename : "(root)";
    fprintf(stderr, "%s: %s%s:\033[0m %s", filename, color_code, type_name, msg);
    if (type_suffix != NULL)
    {
        fprintf(stderr, " [%s%s\033[0m]", color_code, type_suffix);
    }
    fprintf(stderr, "\n");
    free(type_name);
    free(type_suffix);

    return;
}

static void _builtin_spl_handle_msg(error_t type, const spl_loc *const location, const char *msg)
{
    // fprintf(stderr, "msg param %d %d - %d %d\n", linebegin, colbegin, lineend, colend);
    const char *color_code = get_spl_error_color_code(type);
    char *type_name = spl_get_msg_type_prefix(type);
    char *type_suffix = spl_get_msg_type_suffix(type);
    const char *const orig_file = spl_all_file_nodes[location->fid]->filename;
    fprintf(stderr, "%s:%d:%d: %s%s:\033[0m %s", orig_file, location->linebegin, location->colbegin, color_code,
            type_name, msg);
    if (type_suffix != NULL)
    {
        fprintf(stderr, " [%s%s\033[0m]", color_code, type_suffix);
    }
    fprintf(stderr, "\n");
    free(type_name);
    free(type_suffix);

    FILE *file = NULL;
    if ((file = fopen(orig_file, "r")) == NULL)
    {
        fprintf(stderr, "%s: \033[31merror:\033[0m %s: file no longer exists\n", progname, orig_file);
        return;
    }
    char *line = fetchline(file, location->linebegin);
    fclose(file);

    int line_len = (int)strlen(line);

    int t_colend = location->colbegin;
    if (location->lineend == location->linebegin && location->colend > location->colbegin)
        t_colend = location->colend - 1;
    else if (location->lineend != location->linebegin)
        t_colend = line_len;

    print_colored_line(type, line, location->linebegin, location->colbegin, t_colend);
    print_indicator(type, location->colbegin, t_colend);
    free(line);

    return;
}

static void spl_handle_msg(error_t type, const spl_loc *const location, const char *msg)
{
    if (location != NULL)
        _builtin_spl_handle_msg(type, location, msg);
    else
        _builtin_spl_handle_msg_noloc(type, msg);
    return;
}

void splerror_noloc(error_t type, const char *msg)
{
    set_error_flag(1);
    spl_handle_msg(type, NULL, msg);
}

static void _builtin_splerror(error_t type, const spl_loc *const location, const char *msg)
{
    set_error_flag(1);
    spl_handle_msg(type, location, msg);
}

static void _builtin_splwarn(const spl_loc *const location, const char *msg)
{
    spl_handle_msg(SPLC_WARN, location, msg);
}

static void _builtin_splnote(const spl_loc *const location, const char *msg)
{
    spl_handle_msg(SPLC_NOTE, location, msg);
}

static void _builtin_print_trace(util_file_node node)
{
    if (node == NULL)
        return;
    if (node->next != NULL)
    {
        _builtin_print_trace(node->next);
        spltrace(SPLTR_FILE_INCL, 0, node->filename);
        _builtin_splnote(&node->location, "file included here");
        // printf("Current node %s, last_node: %p, %s\n", node->filename, node->next, (node->next != NULL) ?
        // node->next->filename : "");
    }
}

static void print_trace()
{
    _builtin_print_trace(spl_file_stack);
}

void splerror(error_t type, const spl_loc location, const char *msg)
{
    print_trace();
    _builtin_splerror(type, &location, msg);
}

void splwarn(const spl_loc location, const char *msg)
{
    print_trace();
    _builtin_splwarn(&location, msg);
}

void splnote(const spl_loc location, const char *msg)
{
    print_trace();
    _builtin_splnote(&location, msg);
}

static int _builtin_spl_enter_file(const char *restrict _filename, const spl_loc *const location)
{
    FILE *new_file = NULL;
    if ((new_file = fopen(_filename, "r")) == NULL)
    {
        if (location != NULL)
        {
            splerror(SPLC_ERR_CRIT, *location,
                     "failed to read file. Please check whether the path exists or this program has access right.");
        }
        else
        {
            splerror_noloc(
                SPLC_ERR_CRIT,
                "failed to read file. Please check whether the path exists or this program has access right.");
        }
        return -1;
    }

    util_file_node node = (util_file_node)malloc(sizeof(util_file_node_struct));
    node->fid = spl_file_counter++;
    node->filename = strdup(_filename);
    node->file = new_file;
    node->file_buffer = yy_create_buffer(new_file, YY_BUF_SIZE);
    node->location = *location;
    node->yylineno = yylineno;
    node->yycolno = yycolno;
    node->next = spl_file_stack;
    spl_file_stack = node;

    spl_all_file_nodes = (util_file_node *)realloc(spl_all_file_nodes, spl_file_counter * sizeof(util_file_node));
    if (spl_all_file_nodes == NULL)
    {
        splerror_noloc(SPLC_ERR_CRIT, "out of memory when opening file");
        exit(1);
    }
    spl_all_file_nodes[spl_file_counter - 1] = node;

    yy_switch_to_buffer(node->file_buffer);
    yynewfile = 1;
    yylineno = 1;
    yycolno = 1;

    return 0;
}

int spl_enter_root(const char *restrict _filename)
{
    return _builtin_spl_enter_file(_filename, NULL);
}

int spl_enter_file(const char *restrict _filename, const spl_loc location)
{
    return _builtin_spl_enter_file(_filename, &location);
}

int _builtin_spl_exit_file()
{
    if (spl_file_stack == NULL)
    {
        return 1;
    }

    util_file_node tmp = spl_file_stack;

    yy_delete_buffer(tmp->file_buffer);
    fclose(tmp->file);

    spl_file_stack = tmp->next;
    if (spl_file_stack != NULL)
    {
        yy_switch_to_buffer(spl_file_stack->file_buffer);
        yynewfile = 0;
        yylineno = tmp->yylineno;
        yycolno = tmp->yycolno;
    }

    return spl_file_stack == NULL;
}

int spl_exit_file()
{
    return _builtin_spl_exit_file();
}

void set_error_flag(int val)
{
    err_flag = val;
}
