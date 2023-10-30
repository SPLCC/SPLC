/* This file defines tokens of AST. */
#ifndef SPLDEFS_H
#define SPLDEFS_H

#include <stddef.h>
#include <stdio.h>

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
    int linebegin, colbegin;
    int lineend, colend;
    int yylineno;
    int yycolno;
    struct util_file_node_struct *next;
} util_file_node_struct;

extern int spl_file_counter;

/* All previously appeared files will be stored there. They will be indexed using their IDs. */
extern util_file_node *spl_all_file_nodes;

/* The root of linked list files */
extern util_file_node spl_cur_file_node;

extern ast_node root;

extern int err_flag;

/* When modifying, free its content first, and then make a copy and assign to it */
extern char *spl_cur_filename;

extern FILE *spl_cur_file;

extern YY_BUFFER_STATE spl_cur_buffer;

extern const char *progname;

#endif