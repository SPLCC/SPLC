/* This file defines tokens of AST. */
#ifndef SPLDEFS_H
#define SPLDEFS_H

#include <stddef.h>
#include <stdio.h>

/* SPL location struct for tracking purposes */
typedef struct spl_loc
{
    int fid;                 /* file ID */
    int linebegin, colbegin; /* beginning line, beginning column */
    int lineend, colend;     /* ending line, ending column */
} spl_loc;

static const spl_loc spl_loc_root = {.fid = -1, .linebegin = 0, .colbegin = 0, .lineend = 0, .colend = 0};

typedef enum spl_token_type spl_token_t;

enum spl_token_type
{
    /* Nonterminals */
    AST_NULL,             /* uninitialized type */
    AST_PROGRAM,          /* entire program */
    AST_EXT_DEF_LIST,     /* external definition list */
    AST_EXT_DEF,          /* a single external definition: variable/struct/function */
    AST_EXT_DEC_LIST,     /* variable list */
    AST_SPECIFIER,        /* specifier */
    AST_STRUCT_SPECIFIER, /* struct specifier */
    AST_VAR_DEC,          /* variable declaration (single/array) */
    AST_FUNC_DEC,         /* function declaration in the form of `foo(type1 var1, type2 var2, ...) */
    AST_VAR_LIST,         /* variable list (parameter list) */
    AST_PARAM_DEC,        /* single parameter declaration */
    AST_COMP_STMT,        /* compound statement */
    AST_STMT_LIST,        /* list of statements */
    AST_STMT,             /* a single statement */
    AST_FOR_LOOP_BODY,    /* body of a for loop */

    /* Nonterminals: local definition */
    AST_DEF_LIST, /* list of definitions */
    AST_DEF,      /* base for a single definition: list of variable declaration */
    AST_DEC_LIST, /* list of variable declaration */
    AST_DEC,      /* single declaration unit */

    AST_EXP,  /* expressions */
    AST_ARGS, /* argument list */

    /* Terminals: Punctuations */
    AST_SEMI,  /* semicolon */
    AST_COMMA, /* comma */

    /* Terminals: Operators */
    AST_ASSIGN, /* assign */

    AST_AND,         /* and */
    AST_OR,          /* or */
    AST_BITWISE_AND, /* bitwise and */
    AST_BITWISE_OR,  /* bitwise or */

    AST_LT, /* less than */
    AST_LE, /* less than or equal to */
    AST_GT, /* greater than */
    AST_GE, /* greater than or equal to */
    AST_NE, /* not equal to */
    AST_EQ, /* equal to */

    AST_PLUS,  /* plus */
    AST_MINUS, /* minus */
    AST_MUL,   /* multiplication operator */
    AST_DIV,   /* division operator */

    AST_LC,  /* left curly bracket */
    AST_RC,  /* right curly bracket */
    AST_LP,  /* left parenthesis */
    AST_RP,  /* right parenthesis */
    AST_NOT, /* not operator */
    AST_DOT, /* dot - member access */
    AST_LSB, /* left square bracket */
    AST_RSB, /* right square bracket */

    /* Terminal: Keywords */
    AST_STRUCT, /* keyword: struct */
    AST_IF,     /* keyword: if */
    AST_ELSE,   /* keyword: else */
    AST_WHILE,  /* keyword: while */
    AST_FOR,    /* keyword: for */
    AST_RETURN, /* keyword: while */

    /* Terminals: Constant Expressions */
    AST_BUILTIN_TYPE, /* keyword: built-in type */
    AST_ID,           /* id literal */
    AST_INT,          /* integer literal */
    AST_FLOAT,        /* float literal scientific notation allowed */
    AST_CHAR,         /* character literal */
    AST_STREXP,       /* parent of the string expression */
    AST_STR,          /* string literal */

    /* Nonterminals: Macro Expressions */
    AST_MACRO_MNTPT, /* AST: macro mount point */
    AST_MACRO_ID,    /* macro ID */
};

const char *get_spl_token_string(spl_token_t type);

/* The type of the entry in LUT for semantic analysis. */
typedef enum spl_entry_type spl_entry_t;

enum spl_entry_type
{
    SPL_MACRO_VAR,
    SPL_MACRO_FUNC,
    SPL_STRUCT_DEC,
    SPL_TYPEDEF,
    SPL_VAR,
    SPL_FUNC,
};

struct lut_entry_struct;
typedef struct lut_entry_struct *lut_entry;

struct lut_table_struct;
typedef struct lut_table_struct *lut_table;

struct ast_node_struct;
typedef struct ast_node_struct *ast_node;

/* Flex & Bison */
struct YYLTYPE;
typedef struct YYLTYPE YYLTYPE;

struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern int yycolno;

/* Only set this to 1 if entering a new file and needs recounting. This will trigger the parser to restart counting. */
extern int yynewfile;

/* Parsing and error tracking */

typedef struct util_file_node_struct *util_file_node;

typedef struct util_file_node_struct
{
    int fid;                     /* File ID */
    char *filename;              /* name of the file. Must be freed when freeing this struct */
    FILE *file;                  /* file descriptor. Will be closed after splc finished reading this file */
    YY_BUFFER_STATE file_buffer; /* YY_BUFFER_STATE for flex. Will be closed after splc finished reading this file */
    spl_loc location;            /* Location */
    int yylineno;
    int yycolno;
    struct util_file_node_struct *next;
} util_file_node_struct;

extern int spl_include_dir_cnt; /* Number of include directories. */

/* List of include directories. Each entry ends with the directory
   separator, so that they can be directly concatenated. This array must
   end with `NULL`. */
extern const char **spl_include_dirs;

extern int spl_src_file_cnt; /* Number of source files. */

extern const char **spl_src_files; /* Source files */

extern int spl_file_node_cnt; /* How many files have splc encountered */

/* All previously appeared files will be stored there. They will be indexed using their IDs. */
extern util_file_node *spl_all_file_nodes;

/* The root of linked list files. The root marks the previous file. */
extern util_file_node spl_file_node_stack;

/* The root of AST parsed from call to `yyparse()` */
extern ast_node root;

extern int err_flag;

extern const char *progname;

/* Macros */

/* Make a spl_loc struct with specific fid, lines and columns */
#define SPL_MAKE_SPLLOC(_fid, _linebegin, _colbegin, _lineend, _colend)                                                \
    (spl_loc)                                                                                                          \
    {                                                                                                                  \
        .fid = (_fid), .linebegin = (_linebegin), .colbegin = (_colbegin), .lineend = (_lineend), .colend = (_colend)  \
    }

/* Make a spl_loc struct from current file, by specifying line and column numbers */
#define SPL_MAKE_SPLLOC_CF(_linebegin, _colbegin, _lineend, _colend)                                                   \
    (spl_loc)                                                                                                          \
    {                                                                                                                  \
        .fid = spl_file_node_stack->fid, .linebegin = (_linebegin), .colbegin = (_colbegin), .lineend = (_lineend),    \
        .colend = (_colend)                                                                                            \
    }

#define SPL_IS_SPLLOC_ROOT(_loc) ((_loc).fid == -1)

#define SPL_UNPACK_YYLLOC(x) x.first_line, x.first_column, x.last_line, x.last_column
/* Make a spl_loc struct directly from current file and yylloc */
#define YY2SPLLOC_CF_D(x) SPL_MAKE_SPLLOC_CF(x.first_line, x.first_column, x.last_line, x.last_column)
/* Make a spl_loc struct explicitly with single point from line number and column number */
#define YY2SPLLOC_CF_1_PNT_E(_lineno, _colno) SPL_MAKE_SPLLOC_CF((_lineno), (_colno), (_lineno), (_colno))

/* Make a spl_loc struct from _loc by capturing the entire interval */
#define YY2SPLLOC_CF_1_PNT_I(_loc)                                                                                     \
    SPL_MAKE_SPLLOC_CF((_loc).first_line, (_loc).first_column, (_loc).last_line, (_loc).last_column)
/* Make a spl_loc struct from _loc by capturing the first point */
#define YY2SPLLOC_CF_1_PNT_F(_loc)                                                                                     \
    SPL_MAKE_SPLLOC_CF((_loc).first_line, (_loc).first_column, (_loc).first_line, (_loc).first_column)
/* Make a spl_loc struct from _loc by capturing the last point */
#define YY2SPLLOC_CF_1_PNT_L(_loc)                                                                                     \
    SPL_MAKE_SPLLOC_CF((_loc).last_line, (_loc).last_column, (_loc).last_line, (_loc).last_column)

/* Make a spl_loc struct from l1, l2 by capturing the interval. F - first_line/column, L - last_line/column */
#define YY2SPLLOC_CF_2_PNT_FL(l1, l2)                                                                                  \
    SPL_MAKE_SPLLOC_CF((l1).first_line, (l1).first_column, (l2).last_line, (l2).last_column)
/* Make a spl_loc struct from l1, l2 by capturing the interval. F - first_line/column, L - last_line/column */
#define YY2SPLLOC_CF_2_PNT_FF(l1, l2)                                                                                  \
    SPL_MAKE_SPLLOC_CF((l1).first_line, (l1).first_column, (l2).first_line, (l2).first_column)
/* Make a spl_loc struct from l1, l2 by capturing the interval. F - first_line/column, L - last_line/column */
#define YY2SPLLOC_CF_2_PNT_LF(l1, l2)                                                                                  \
    SPL_MAKE_SPLLOC_CF((l1).last_line, (l1).last_column, (l2).first_line, (l2).first_column)
/* Make a spl_loc struct from l1, l2 by capturing the interval. F - first_line/column, L - last_line/column */
#define YY2SPLLOC_CF_2_PNT_LL(l1, l2)                                                                                  \
    SPL_MAKE_SPLLOC_CF((l1).last_line, (l1).last_column, (l2).last_line, (l2).last_column)

#endif