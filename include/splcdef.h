/* This file defines tokens of AST. */
#ifndef SPLDEFS_H
#define SPLDEFS_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* SPL location struct for tracking purposes */
typedef struct splc_loc
{
    int fid;                 /* file ID */
    int linebegin, colbegin; /* beginning line, beginning column */
    int lineend, colend;     /* ending line, ending column */
} splc_loc;

static const splc_loc splc_invalid_loc = {.fid = -1, .linebegin = 0, .colbegin = 0, .lineend = 0, .colend = 0};

static const splc_loc splc_root_loc = {.fid = -1, .linebegin = 0, .colbegin = 0, .lineend = 0, .colend = 0};

/* Convert a location to string. The caller is responsible for freeing this string. */
char *splc_loc2str(splc_loc location);

splc_loc splc_concat_loc(splc_loc l, splc_loc r);

typedef enum splc_token_type splc_token_t;

enum splc_token_type
{
    /* Nonterminals */
    SPLT_NULL,             /* uninitialized type */
    SPLT_TRANS_UNIT,       /* translation unit */
    SPLT_EXT_DEF_LIST,     /* external definition list */
    SPLT_EXT_DEF,          /* a single external definition: variable/struct/function */
    SPLT_EXT_DEC_LIST,     /* variable list */
    SPLT_TYPE_SPEC,        /* specifier */
    SPLT_STRUCT_SPECIFIER, /* struct specifier */
    SPLT_VAR_DEC,          /* variable declaration (pointer interface) */
    SPLT_DIR_DEC,          /* variable declaration (single/array) */
    SPLT_PTR,              /* pointer */
    SPLT_FUNC_DEC,         /* function declaration in the form of `foo(type1 var1, type2 var2, ...) */
    SPLT_DIR_FUNC_DEC,     /* direct function declaration in the form of `foo(type1 var1, type2 var2, ...) */

    SPLT_VAR_LIST,  /* variable list (parameter list) */
    SPLT_PARAM_DEC, /* single parameter declaration */

    SPLT_COMP_STMT,     /* compound statement */
    SPLT_STMT_LIST,     /* list of statements */
    SPLT_STMT,          /* single statement */
    SPLT_EXPR_STMT,     /* expression statement */
    SPLT_SEL_STMT,      /* conditional statement */
    SPLT_ITER_STMT,     /* iteration statement */
    SPLT_FOR_LOOP_BODY, /* body of a for loop */
    SPLT_LABELED_STMT,  /* labeled statement */
    SPLT_JUMP_STMT,     /* jump statement */

    SPLT_DEF_LIST, /* list of definitions */
    SPLT_DEF,      /* base for a single definition: list of variable declaration */
    SPLT_DEC_LIST, /* list of variable declaration */
    SPLT_DEC,      /* single declarator, one of `var`s in `type var1, var2` */

    SPLT_PRIM_EXPR,   /* primary expression */
    SPLT_POSTF_EXPR,  /* postfix expression */
    SPLT_UNARY_EXPR,  /* unary expression */
    SPLT_CAST_EXPR,   /* unary expression */
    SPLT_MUL_EXPR,    /* multiplicative expression */
    SPLT_ADD_EXPR,    /* additive expression */
    SPLT_SHIFT_EXPR,  /* shift expression */
    SPLT_REL_EXPR,    /* relational expression */
    SPLT_EQ_EXPR,     /* equality expression */
    SPLT_BW_AND_EXPR, /* bitwise AND expression */
    SPLT_BW_XOR_EXPR, /* bitwise XOR expression */
    SPLT_BW_OR_EXPR,  /* bitwise OR expression */
    SPLT_AND_EXPR,    /* logical AND expression */
    SPLT_OR_EXPR,     /* logical OR expression */
    SPLT_COND_EXPR,   /* conditional expression, using "?:" */
    SPLT_ASSIGN_EXPR, /* assignment expression */
    SPLT_EXPR,        /* expression */
    SPLT_CONST_EXPR,  /* constant expression */

    // SPLT_CONST_EXPR, /* constant expression */
    SPLT_ARG_LIST, /* argument list */

    /* Terminal: Keywords */
    SPLT_STRUCT,  /* keyword: struct */
    SPLT_UNION,   /* keyword: union */

    SPLT_WHILE,   /* keyword: while */
    SPLT_FOR,     /* keyword: for */
    SPLT_DO,      /* keyword: do */

    SPLT_IF,      /* keyword: if */
    SPLT_ELSE,    /* keyword: else */
    SPLT_SWITCH,  /* keyword: switch */
    SPLT_DEFAULT, /* keyword: default */
    SPLT_CASE,    /* keyword: case */

    SPLT_GOTO,     /* keyword: goto */
    SPLT_CONTINUE, /* keyword: continue */
    SPLT_BREAK,    /* keyword: break */
    SPLT_RETURN,   /* keyword: return */

    /* Terminals: Punctuators */
    SPLT_LC,  /* left curly bracket */
    SPLT_RC,  /* right curly bracket */
    SPLT_LP,  /* left parenthesis */
    SPLT_RP,  /* right parenthesis */
    SPLT_LSB, /* left square bracket */
    SPLT_RSB, /* right square bracket */

    SPLT_SEMI,  /* semicolon */
    SPLT_COMMA, /* comma */

    SPLT_QM,    /* question mark */
    SPLT_COLON, /* colon */

    /* Assignment Operators */
    SPLT_ASSIGN,        /* assign */
    SPLT_MUL_ASSIGN,    /* multiply and assign */
    SPLT_DIV_ASSIGN,    /* divide and assign */
    SPLT_MOD_ASSIGN,    /* mode and assign */
    SPLT_PLUS_ASSIGN,   /* plus and assign */
    SPLT_MINUS_ASSIGN,  /* minus and assign */
    SPLT_LSHIFT_ASSIGN, /* left shift and assign */
    SPLT_RSHIFT_ASSIGN, /* right shift and assign */
    SPLT_BW_AND_ASSIGN, /* bitwise AND and assign */
    SPLT_BW_XOR_ASSIGN, /* bitwise XOR and assign */
    SPLT_BW_OR_ASSIGN,  /* bitwise OR assign */

    SPLT_LSHIFT, /* right shift */
    SPLT_RSHIFT, /* left shift */
    SPLT_BW_AND, /* bitwise and */
    SPLT_BW_OR,  /* bitwise or */
    SPLT_BW_XOR, /* bitwise xor */
    SPLT_BW_NOT, /* not operator */
    SPLT_AND,    /* and */
    SPLT_OR,     /* or */
    SPLT_NOT,    /* not operator */

    SPLT_SIZEOF, /* sizeof */

    SPLT_LT, /* less than */
    SPLT_LE, /* less than or equal to */
    SPLT_GT, /* greater than */
    SPLT_GE, /* greater than or equal to */
    SPLT_NE, /* not equal to */
    SPLT_EQ, /* equal to */

    SPLT_DPLUS,  /* double plus */
    SPLT_DMINUS, /* double minus */
    SPLT_PLUS,   /* plus */
    SPLT_MINUS,  /* minus */
    SPLT_ASTRK,  /* asterisk operator */
    SPLT_DIV,    /* division operator */
    SPLT_MOD,    /* mod operator */

    SPLT_DOT,    /* dot - member access */
    SPLT_RARROW, /* right arrow - member access */

    /* Terminals: Constant Expressions */
    SPLT_TYPE_INT,      /* type: int */
    SPLT_TYPE_FLOAT,    /* type: float */
    SPLT_TYPE_CHAR,     /* type: char */
    // SPLT_TYPE_UNSIGNED, /* type: int */
    // SPLT_TYPE_SIGNED,   /* type: int */
    // SPLT_TYPE_LONG,     /* type: int */
    SPLT_ID,            /* id literal */
    SPLT_CONSTANT,      /* constants */

    /* Nonterminals: Macro Expressions */
    SPLT_MACRO_MNTPT,  /* AST: macro mount point */
    SPLT_MACRO_ID,     /* macro ID */
    SPLT_MACRO_DEFINE, /* macro: define */
    SPLT_MACRO_IFDEF,  /* macro: ifdef */
    SPLT_MACRO_IFNDEF, /* macro: ifndef */
    SPLT_MACRO_ENDIF,  /* macro: endif */

    /* Terminals: Literals */
    SPLT_LTR_INT,   /* integer literal */
    SPLT_LTR_FLOAT, /* float literal scientific notation allowed */
    SPLT_LTR_CHAR,  /* character literal */
    SPLT_LTR_STR,   /* string literal */
    SPLT_STR,       /* basic string unit */
};

#define SPLT_IS_BUILTIN_TYPE(x) ((x) == SPLT_TYPE_INT || (x) == SPLT_TYPE_FLOAT || (x) == SPLT_TYPE_CHAR)

/* Convert a token to string. The caller shall not free this string. */
const char *splc_token2str(splc_token_t type);

/* Get the color code of this token type to color the output in console. */
const char *splc_get_token_color_code(splc_token_t type);

/* The type of the entry in LUT for semantic analysis. */
typedef enum splc_entry_type splc_entry_t;

enum splc_entry_type
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

extern YYLTYPE yylloc;

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
    splc_loc location;           /* Location */
    int yylineno;
    int yycolno;
    struct util_file_node_struct *next;
} util_file_node_struct;

/* Passed splc arguments */

extern int splc_incl_dir_cnt; /* Number of include directories. */

/* List of include directories. Each entry ends with the directory
   separator, so that they can be directly concatenated. This array must
   end with `NULL`. */
extern const char **splc_incl_dirs;

extern int splc_src_file_cnt; /* Number of source files. */

extern const char **splc_src_files; /* Source files */

extern int splc_enable_diag;

extern int splc_enable_colored_ast;

/* splc internal global variables */

extern int splc_file_node_cnt; /* How many files have splc encountered */

/* All previously appeared files will be stored there. They will be indexed using their IDs. */
extern util_file_node *splc_all_file_nodes;

/* The root of linked list files. The root marks the previous file. */
extern util_file_node splc_file_node_stack;

/* The root of AST parsed from call to `yyparse()` */
extern ast_node root;

extern int err_flag;

extern const char *progname;

/* Macros */

/* Make a splc_loc struct with specific fid, lines and columns */
#define SPLC_MAKE_LOC(_fid, _linebegin, _colbegin, _lineend, _colend)                                                  \
    (splc_loc)                                                                                                         \
    {                                                                                                                  \
        .fid = (_fid), .linebegin = (_linebegin), .colbegin = (_colbegin), .lineend = (_lineend), .colend = (_colend)  \
    }

/* Make a splc_loc struct from current file, by specifying line and column numbers */
#define SPLC_MAKE_LOC_CF(_linebegin, _colbegin, _lineend, _colend)                                                     \
    (splc_loc)                                                                                                         \
    {                                                                                                                  \
        .fid = splc_file_node_stack->fid, .linebegin = (_linebegin), .colbegin = (_colbegin), .lineend = (_lineend),   \
        .colend = (_colend)                                                                                            \
    }

#define SPLC_ROOT_LOC (splc_root_loc)

/* Check if the location is root (i.e., assigned from static global constant) */
#define SPLC_IS_LOC_ROOT(_loc) ((_loc).fid == -1)

#define SPLC_INVALID_LOC (splc_invalid_loc)

/* Check if the location is invalid (i.e., assigned from static global constant) */
#define SPLC_IS_LOC_INVALID(_loc) ((_loc).fid == -1)

#define SPLC_UNPACK_YYLLOC(x) x.first_line, x.first_column, x.last_line, x.last_column
/* Make a splc_loc struct directly from current file and yylloc */
#define SPLC_YY2LOC_CF_D(x) SPLC_MAKE_LOC_CF(x.first_line, x.first_column, x.last_line, x.last_column)
/* Make a splc_loc struct explicitly with single point from line number and column number */
#define SPLC_YY2LOC_CF_1_PNT_E(_lineno, _colno) SPLC_MAKE_LOC_CF((_lineno), (_colno), (_lineno), (_colno))

/* Make a splc_loc struct from _loc from current file by capturing the entire interval */
#define SPLC_YY2LOC_CF_1_PNT_I(_loc)                                                                                   \
    SPLC_MAKE_LOC_CF((_loc).first_line, (_loc).first_column, (_loc).last_line, (_loc).last_column)
/* Make a splc_loc struct from _loc from current file by capturing the first point */
#define SPLC_YY2LOC_CF_1_PNT_F(_loc)                                                                                   \
    SPLC_MAKE_LOC_CF((_loc).first_line, (_loc).first_column, (_loc).first_line, (_loc).first_column)
/* Make a splc_loc struct from _loc from current file by capturing the last point */
#define SPLC_YY2LOC_CF_1_PNT_L(_loc)                                                                                   \
    SPLC_MAKE_LOC_CF((_loc).last_line, (_loc).last_column, (_loc).last_line, (_loc).last_column)

/* Make a splc_loc struct from l1, l2 from current file by capturing the interval. F - first_line/column, L -
 * last_line/column */
#define SPLC_YY2LOC_CF_2_PNT_FL(l1, l2)                                                                                \
    SPLC_MAKE_LOC_CF((l1).first_line, (l1).first_column, (l2).last_line, (l2).last_column)
/* Make a splc_loc struct from l1, l2 from current file by capturing the interval. F - first_line/column, L -
 * last_line/column */
#define SPLC_YY2LOC_CF_2_PNT_FF(l1, l2)                                                                                \
    SPLC_MAKE_LOC_CF((l1).first_line, (l1).first_column, (l2).first_line, (l2).first_column)
/* Make a splc_loc struct from l1, l2 from current file by capturing the interval. F - first_line/column, L -
 * last_line/column */
#define SPLC_YY2LOC_CF_2_PNT_LF(l1, l2)                                                                                \
    SPLC_MAKE_LOC_CF((l1).last_line, (l1).last_column, (l2).first_line, (l2).first_column)
/* Make a splc_loc struct from l1, l2 from current file by capturing the interval. F - first_line/column, L -
 * last_line/column */
#define SPLC_YY2LOC_CF_2_PNT_LL(l1, l2)                                                                                \
    SPLC_MAKE_LOC_CF((l1).last_line, (l1).last_column, (l2).last_line, (l2).last_column)

#endif