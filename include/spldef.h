/* This file defines tokens of AST. */
#ifndef SPLDEFS_H
#define SPLDEFS_H

#include <stddef.h>

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
    AST_RETURN, /* keyword: while */

    /* Terminals: Constant Expressions */
    AST_BUILTIN_TYPE, /* keyword: built-in type */
    AST_ID,
    AST_INT,
    AST_FLOAT,
    AST_CHAR,
};

const char *get_spl_token_string(spl_token_t type);

/* The type of the entry in LUT for semantic analysis. */
typedef enum spl_entry_type spl_entry_t;

enum spl_entry_type
{
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

extern int err_flag;

#endif