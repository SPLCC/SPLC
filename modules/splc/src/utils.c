#include "utils.h"
#include "dir_search.h"
#include "lex.yy.h"
#include "syntax.tab.h"

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int splcf_verbose = 0;

int splcf_no_diagnostics_color = 0;

#define SPLC_COLORED_DIAG (splcf_no_diagnostics_color == 0)

int splc_buffer_node_cnt = 0;

util_yy_buffer_node *splc_all_buffer_nodes = NULL;

util_yy_buffer_node splc_buffer_node_stack = NULL;

size_t splc_buffer_node_stack_size = 0;

/* Return an array of lines fetched. No newline character will be present. */
static char *fetchline(FILE *file, int linebegin)
{
    char *lineptr = (char *)NULL;
    for (int cur = 0; cur < linebegin; ++cur)
    {
        free(lineptr);
        lineptr = (char *)NULL;
        size_t n = 0;
        if (getline(&lineptr, &n, file) < 0)
            break;
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

static void print_colored_line(splc_msg_t type, const char *line, int linebegin, int colbegin, int colend)
{
    const char *color_code = get_splc_msg_color_code(type);
    fprintf(stderr, "%8d | ", linebegin);

    for (int i = 0; i < colbegin - 1; ++i)
        fprintf(stderr, "%c", line[i]);

    fprintf(stderr, "\033[1m%s", color_code);
    for (int i = colbegin - 1; i < colend; ++i)
        fprintf(stderr, "%c", line[i]);
    fprintf(stderr, "\033[0m");

    for (int i = colend; line[i] != '\0'; ++i)
        fprintf(stderr, "%c", line[i]);
    fprintf(stderr, "\n");
}

static void print_indicator(splc_msg_t type, int colbegin, int colend)
{
    // fprintf(stderr, "Accepted parameters: %d %d\n", colbegin, colend);
    const char *color_code = get_splc_msg_color_code(type);

    fprintf(stderr, "         | ");

    for (int i = 1; i < colbegin; ++i)
        fprintf(stderr, " ");

    fprintf(stderr, "%s^", color_code);
    for (int i = colbegin + 1; i <= colend; ++i)
        fprintf(stderr, "~");
    fprintf(stderr, "\033[0m\n");

    return;
}

static void _builtin_splc_handle_msg_noloc(splc_msg_t type, const char *msg)
{
    const char *color_code = get_splc_msg_color_code(type);
    const char *type_name = splc_get_msg_type_prefix(type);
    const char *type_suffix = splc_get_msg_type_suffix(type);
    const char *buffername = (splc_buffer_node_stack != NULL) ? splc_buffer_node_stack->buffername : progname;
    const char *prefix = "";
    if ((splc_buffer_node_stack != NULL) && (SPLC_IS_BUFFER_CHAR_ARRAY(splc_buffer_node_stack)))
    {
        prefix = "In expansion of macro ";
    }

    fprintf(stderr, "%s\033[1m%s:\033[0m %s%s:\033[0m %s", prefix, buffername, color_code, type_name, msg);
    if (type_suffix != NULL)
    {
        fprintf(stderr, " [\033[1m%s%s\033[0m]", color_code, type_suffix);
    }
    fprintf(stderr, "\n");

    return;
}

static char *get_char_buffer_content_line(const char *content)
{
    size_t needed = 1;
    const char *ptr = content;
    while (*ptr != '\0' && *ptr != '\r' && *ptr != '\n')
    {
        ++needed;
        ++ptr;
    }
    char *result = malloc(needed);
    SPLC_ALLOC_PTR_CHECK(result, "cannot allocate buffer for char buffer inclusion");
    memcpy(result, content, ptr - content);
    result[needed - 1] = '\0';
    return result;
}

static void _builtin_splc_handle_msg(splc_msg_t type, const splc_loc *const location, const char *msg)
{
    // fprintf(stderr, "msg param %d %d - %d %d\n", location->linebegin, location->colbegin, location->lineend,
    // location->colend);
    const char *color_code = get_splc_msg_color_code(type);
    const char *type_name = splc_get_msg_type_prefix(type);
    const char *type_suffix = splc_get_msg_type_suffix(type);
    const char *const orig_file = splc_all_buffer_nodes[location->fid]->buffername;
    const char *prefix = "";
    if (SPLC_IS_BUFFER_CHAR_ARRAY(splc_all_buffer_nodes[location->fid]))
    {
        prefix = "In expansion of macro ";
    }
    fprintf(stderr, "%s\033[1m%s:%d:%d:\033[0m %s%s:\033[0m %s", prefix, orig_file, location->linebegin, location->colbegin,
            color_code, type_name, msg);
    if (type_suffix != NULL)
    {
        fprintf(stderr, " [\033[1m%s%s\033[0m]", color_code, type_suffix);
    }
    fprintf(stderr, "\n");

    char *line = NULL;
    if (SPLC_IS_BUFFER_FILE(splc_all_buffer_nodes[location->fid]))
    {
        FILE *file = NULL;
        if ((file = fopen(orig_file, "r")) == NULL)
        {
            SPLC_FERROR_NOLOC(SPLM_ERR_FATAL, "file no longer exists: %s\n", orig_file);
            return;
        }
        line = fetchline(file, location->linebegin);
        fclose(file);
    }
    else if (SPLC_IS_BUFFER_CHAR_ARRAY(splc_all_buffer_nodes[location->fid]))
    {
        line = get_char_buffer_content_line(splc_all_buffer_nodes[location->fid]->content);
    }

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

/* If passed a NULL location or location that has fid=1, then the trace information will not be printed. */
static void splc_dispatch_msg(splc_msg_t type, const splc_loc *const location, const char *msg)
{
    if (!SPLC_IS_MSG_NONVERBOSE(type) && !splcf_verbose)
        return;
    if (location != NULL && !SPLC_IS_LOC_ROOT(*location))
        _builtin_splc_handle_msg(type, location, msg);
    else
        _builtin_splc_handle_msg_noloc(type, msg);
    return;
}

static const char *splc_get_trace_string(trace_t type)
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
    return type_str;
}

void splctrace(const trace_t type, int show_source, const char *name)
{
    const char *prefix = "";
    if (SPLC_IS_BUFFER_CHAR_ARRAY(splc_buffer_node_stack))
    {
        prefix = "In expansion of macro ";
    }
    const char *type_str = splc_get_trace_string(type);
    fprintf(stderr, "%s%s%sIn %s \033[1m'%s':%d\033[0m:\n", show_source != 0 ? prefix : "", 
                                                            show_source != 0 ? splc_buffer_node_stack->buffername : "",
                                                            show_source != 0 ? ": " : "", 
                                                            type_str, 
                                                            name, 
                                                            splc_buffer_node_stack->yylineno);
    return;
}

/* Update error/warning count if necessary */
void splc_update_log_status(const splc_msg_t type)
{
    if (SPLC_IS_MSG_WARNING(type))
        update_warning(1);
    if (SPLC_IS_MSG_ERROR(type))
        update_error(1);
}

static void _builtin_print_file_trace(util_yy_buffer_node node)
{
    if (node == NULL || SPLC_IS_LOC_ROOT(node->location) || node->next->next == NULL)
    {
        return;
    }
    util_yy_buffer_node top = node;
    const char *suffix = (top->next->next == NULL) ? ":\n" : ",\n";
    const char *prefix = "";
    if (SPLC_IS_BUFFER_FILE(splc_buffer_node_stack))
    {
        prefix = "In file included from ";
    }
    else if (SPLC_IS_BUFFER_CHAR_ARRAY(splc_buffer_node_stack))
    {
        prefix = "In expansion of macro ";
    }
    fprintf(stderr, "%s\033[1m%s:%d\033[0m%s", prefix, top->next->buffername, top->yylineno, suffix);
    top = top->next;
    while (top && !SPLC_IS_LOC_ROOT(top->location))
    {
        if (SPLC_IS_BUFFER_CHAR_ARRAY(splc_buffer_node_stack))
        {
            prefix = "macro ";
        }
        SPLC_ASSERT(top->next != NULL);
        suffix = (top->next->next == NULL) ? ":\n" : ",\n";
        fprintf(stderr, "                 from %s\033[1m%s:%d\033[0m%s", prefix, top->next->buffername, top->yylineno, suffix);
        top = top->next;
    }
}

static inline void print_file_trace(util_yy_buffer_node node)
{
    if (node == NULL)
        _builtin_print_file_trace(splc_buffer_node_stack);
    else
        _builtin_print_file_trace(node);
}

void splc_internal_handle_msg(const splc_msg_t type, const splc_loc location, const char *msg)
{
    int require_loc = !SPLC_IS_LOC_INVALID(location);
    if (require_loc)
    {
        print_file_trace(splc_all_buffer_nodes[location.fid]);
    }
    splc_update_log_status(type);
    splc_dispatch_msg(type, require_loc ? &location : NULL, msg);
}

int buffer_stack_check_node_exists(const char *restrict nodename)
{
    if (splc_buffer_node_stack == NULL)
        return 0;
    util_yy_buffer_node tmp = splc_buffer_node_stack;
    while (tmp != NULL)
    {
        if (strcmp(nodename, tmp->buffername) == 0)
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

static int _builtin_splc_enter_file_buffer(const char *restrict _filename, const splc_loc *const location)
{
    if (splc_buffer_node_stack_size >= 1000)
    {
        SPLC_FFAIL("on entering file \033[1m%s:\033[0m maximum recursion depth met: %zu", _filename, splc_buffer_node_stack_size);
    }

    FILE *new_file = NULL;
    char *filename = NULL; /* This contains actually the full path of file. After got from splc_search_incl_dirs, there
                              is no need to free it, as it will be directly placed in the file node. */
    if ((filename = splc_search_incl_dirs(_filename)) == NULL || (new_file = fopen(filename, "r")) == NULL)
    {
        const char *msg =
            "failed to include file. Please check whether the path exists or this program has access right.";
        if (location != NULL)
        {
            SPLC_ERROR(SPLM_ERR_FATAL, *location, msg);
        }
        else
        {
            SPLC_ERROR_NOLOC(SPLM_ERR_FATAL, msg);
        }
        return -1;
    }
    SPLC_FDIAG("the included file has been identified: %s", filename);

    util_yy_buffer_node node = (util_yy_buffer_node)malloc(sizeof(util_yy_buffer_node_struct));
    node->type = 0;
    node->fid = splc_buffer_node_cnt++;
    node->buffername = filename;
    node->file = new_file;
    node->content = NULL;
    node->real_yy_buffer = yy_create_buffer(new_file, YY_BUF_SIZE);
    node->location = (location != NULL) ? *location : SPLC_ROOT_LOC;
    node->yylineno = yylineno;
    node->yycolno = yycolno;
    node->next = splc_buffer_node_stack;
    splc_buffer_node_stack = node;

    splc_all_buffer_nodes =
        (util_yy_buffer_node *)realloc(splc_all_buffer_nodes, splc_buffer_node_cnt * sizeof(util_yy_buffer_node));
    SPLC_ALLOC_PTR_CHECK(splc_all_buffer_nodes, "out of memory when creating new buffer node array");
    splc_all_buffer_nodes[splc_buffer_node_cnt - 1] = node;

    yy_switch_to_buffer(node->real_yy_buffer);
    yynewfile = 1;
    yylineno = 1;
    yycolno = 1;

    splc_buffer_node_stack_size += 1;
    return 0;
}

static int _builtin_splc_push_char_buffer(const char *restrict _macroname, const splc_loc *const location,
                                          const char *content)
{
    SPLC_ASSERT(_macroname != NULL && content != NULL);
    SPLC_FDIAG("substituting macro: %s", _macroname);

    if (splc_buffer_node_stack_size >= 1000)
    {
        SPLC_FFAIL("on expansion of macro \033[1m%s:\033[0m maximum recursion depth met: %zu", _macroname, splc_buffer_node_stack_size);
    }

    util_yy_buffer_node node = (util_yy_buffer_node)malloc(sizeof(util_yy_buffer_node_struct));
    node->type = 1;
    node->fid = splc_buffer_node_cnt++;
    node->buffername = strdup(_macroname);
    node->file = NULL;
    node->content = strdup(content);
    SPLC_ALLOC_PTR_CHECK(node->content, "failed to copy buffer char array");
    node->real_yy_buffer = yy_scan_string(content);
    node->location = (location != NULL) ? *location : SPLC_ROOT_LOC;
    node->yylineno = yylineno;
    node->yycolno = yycolno;
    node->next = splc_buffer_node_stack;
    splc_buffer_node_stack = node;

    splc_all_buffer_nodes =
        (util_yy_buffer_node *)realloc(splc_all_buffer_nodes, splc_buffer_node_cnt * sizeof(util_yy_buffer_node));
    SPLC_ALLOC_PTR_CHECK(splc_all_buffer_nodes, "out of memory when creating new buffer node array");
    splc_all_buffer_nodes[splc_buffer_node_cnt - 1] = node;

    yy_switch_to_buffer(node->real_yy_buffer);
    yynewfile = 1;
    yylineno = 1;
    yycolno = 1;

    splc_buffer_node_stack_size += 1;
    return 0;
}

static int _builtin_splc_exit_file_buffer()
{
    util_yy_buffer_node tmp = splc_buffer_node_stack;

    yy_delete_buffer(tmp->real_yy_buffer);
    fclose(tmp->file);

    splc_buffer_node_stack = tmp->next;
    if (splc_buffer_node_stack != NULL)
    {
        yy_switch_to_buffer(splc_buffer_node_stack->real_yy_buffer);
        yynewfile = 0;
        /* Handle yylloc */
        yylloc.first_line = tmp->location.linebegin;
        yylloc.first_column = tmp->location.colbegin;
        yylloc.last_line = tmp->location.lineend;
        yylloc.last_column = tmp->location.colend;
        yylineno = tmp->yylineno;
        yycolno = tmp->yycolno;
    }

    splc_buffer_node_stack_size -= 1;
    return splc_buffer_node_stack == NULL;
}

static int _builtin_splc_exit_char_buffer()
{
    util_yy_buffer_node tmp = splc_buffer_node_stack;

    yy_delete_buffer(tmp->real_yy_buffer);

    splc_buffer_node_stack = tmp->next;
    if (splc_buffer_node_stack != NULL)
    {
        yy_switch_to_buffer(splc_buffer_node_stack->real_yy_buffer);
        yynewfile = 0;
        /* Handle yylloc */
        yylloc.first_line = tmp->location.linebegin;
        yylloc.first_column = tmp->location.colbegin;
        yylloc.last_line = tmp->location.lineend;
        yylloc.last_column = tmp->location.colend;
        yylineno = tmp->yylineno;
        yycolno = tmp->yycolno;
    }

    splc_buffer_node_stack_size -= 1;
    return splc_buffer_node_stack == NULL;
}

static int _builtin_splc_exit_buffer()
{
    if (splc_buffer_node_stack == NULL)
    {
        return 1;
    }

    SPLC_ASSERT(SPLC_IS_BUFFER_FILE(splc_buffer_node_stack) || SPLC_IS_BUFFER_CHAR_ARRAY(splc_buffer_node_stack));

    if (SPLC_IS_BUFFER_FILE(splc_buffer_node_stack))
    {
        return _builtin_splc_exit_file_buffer();
    }
    else if (SPLC_IS_BUFFER_CHAR_ARRAY(splc_buffer_node_stack))
    {
        return _builtin_splc_exit_char_buffer();
    }
    return 1;
}

int splc_enter_root(const char *restrict _filename)
{
    return _builtin_splc_enter_file_buffer(_filename, NULL);
}

int splc_push_file_buffer(const char *restrict _filename, const splc_loc location)
{
    return _builtin_splc_enter_file_buffer(_filename, &location);
}

int splc_push_char_buffer(const char *restrict _macroname, const splc_loc location, const char *content)
{
    return _builtin_splc_push_char_buffer(_macroname, &location, content);
}

int splc_pop_buffer()
{
    return _builtin_splc_exit_buffer();
}

const char *const splc_get_buffer_node_name(int fid)
{
    return splc_all_buffer_nodes[fid]->buffername;
}

void update_error(int val)
{
    err_count += val;
}

void update_warning(int val)
{
    warn_count += val;
}