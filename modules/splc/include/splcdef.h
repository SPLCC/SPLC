/* This file defines tokens of AST. */
#ifndef SPLCDEF_H
#define SPLCDEF_H

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

#define SPLT_EXPR_OFFSET 0x00000080
#define SPLT_STMT_OFFSET 0x00000100
#define SPLT_KEYWORD_OFFSET 0x00000200
#define SPLT_PUNCTUATOR_OFFSET 0x00000400
#define SPLT_OPERATOR_OFFSET 0x00000800
#define SPLT_BUILTIN_TYPE_OFFSET 0x00001000
#define SPLT_ID_OFFSET 0x00002000
#define SPLT_MACRO_MNTPT_OFFSET 0x00004000
#define SPLT_LITERAL_OFFSET 0x00008000

typedef enum splc_token_type splc_token_t;

enum splc_token_type
{
    /* Nonterminals */
    SPLT_NULL = 0x00000000,  /* uninitialized type */
    SPLT_TRANS_UNIT,         /* translation unit */
    SPLT_EXT_DECLTN_LIST,    /* external declaration list */
    SPLT_EXT_DECLTN,         /* a single declaration definition: variable/struct/function */
    SPLT_DECLTN_SPEC,        /* declaration specifier */
    SPLT_TYPE_SPEC,          /* type specifier */
    SPLT_STRG_SPEC,          /* storage class specifier */
    SPLT_TYPE_QUAL_LIST,     /* type qualifier list */
    SPLT_TYPE_QUAL,          /* type qualifier */
    SPLT_FUNC_SPEC,          /* function specifier */
    SPLT_SPEC_QUAL_LIST,     /* specifier-qualifier-list */
    SPLT_TYPENAME,           /* typename */
    SPLT_STRUCT_UNION_SPEC,  /* struct or union specifier */
    SPLT_STRUCT_DECLTN_BODY, /* struct declaration body */
    SPLT_STRUCT_DECLTN_LIST, /* struct declaration list */
    SPLT_STRUCT_DECLTN,      /* struct declaration */
    SPLT_STRUCT_DEC_LIST,    /* struct declarator list */
    SPLT_STRUCT_DEC,         /* struct declarator */
    SPLT_ENUM_SPEC,          /* enum specifier */
    SPLT_ENUM_BODY,          /* enum body */
    SPLT_ENUM_LIST,          /* enum list */
    SPLT_ENUM,               /* a single enum */
    SPLT_DEC,                /* variable declaration (pointer interface) */
    SPLT_DIR_DEC,            /* variable declaration (single/array) */
    SPLT_PTR,                /* pointer */
    SPLT_FUNC_DEF,           /* function definition */
    SPLT_FUNC_DEC,           /* function declarator in the form of `*foo(type1 var1, type2 var2, ...) */
    SPLT_DIR_FUNC_DEC,       /* direct function declarator in the form of `foo(type1 var1, type2 var2, ...) */

    SPLT_DECLTN_LIST,   /* list of declarations */
    SPLT_DECLTN,        /* wrapper for declaration with SEMI */
    SPLT_DIR_DECLTN,    /* declaration */
    SPLT_INIT_DEC_LIST, /* list of variable declaration */
    SPLT_INIT_DEC,      /* single declarator, one of `var`s in `type var1, var2` */
    SPLT_INIT,          /* initializer */
    SPLT_INIT_LIST,     /* initializer list */
    SPLT_DESGTN,        /* designation */
    SPLT_DESG_LIST,     /* designator list */
    SPLT_DESG,          /* designator */
    SPLT_ABS_DEC,       /* typename */
    SPLT_DIR_ABS_DEC,   /* direct abstract declarator */

    SPLT_PARAM_TYPE_LIST, /* parameter-type-list which allows VAR_ARG dots */
    SPLT_PARAM_LIST,      /* parameter list */
    SPLT_PARAM_DEC,       /* single parameter declaration */

    SPLT_STMT = SPLT_STMT_OFFSET, /* single statement */
    SPLT_GEN_STMT_LIST,           /* general statement list */
    SPLT_COMP_STMT,               /* compound statement */
    SPLT_EXPR_STMT,               /* expression statement */
    SPLT_SEL_STMT,                /* conditional statement */
    SPLT_ITER_STMT,               /* iteration statement */
    SPLT_FOR_LOOP_BODY,           /* body of a for loop */
    SPLT_LABELED_STMT,            /* labeled statement */
    SPLT_JUMP_STMT,               /* jump statement */
    // SPLT_STMT_LIST,            /* list of statements */

    SPLT_EXPR = SPLT_EXPR_OFFSET, /* expression */
    SPLT_PRIM_EXPR,               /* primary expression */
    SPLT_CALL_EXPR,         /* function invocation expression */
    SPLT_POSTFIX_EXPR,            /* postfix expression */
    SPLT_UNARY_EXPR,              /* unary expression */
    SPLT_CAST_EXPR,               /* cast expression */
    SPLT_MUL_EXPR,                /* multiplicative expression */
    SPLT_ADD_EXPR,                /* additive expression */
    SPLT_SHIFT_EXPR,              /* shift expression */
    SPLT_REL_EXPR,                /* relational expression */
    SPLT_EQ_EXPR,                 /* equality expression */
    SPLT_BW_AND_EXPR,             /* bitwise AND expression */
    SPLT_BW_XOR_EXPR,             /* bitwise XOR expression */
    SPLT_BW_OR_EXPR,              /* bitwise OR expression */
    SPLT_AND_EXPR,                /* logical AND expression */
    SPLT_OR_EXPR,                 /* logical OR expression */
    SPLT_COND_EXPR,               /* conditional expression, using "?:" */
    SPLT_ASSIGN_EXPR,             /* assignment expression */
    SPLT_CONST_EXPR,              /* constant expression */
    SPLT_INIT_EXPR,               /* initialization expression, permitted in for-loop */

    // SPLT_CONST_EXPR, /* constant expression */
    SPLT_ARG_LIST, /* argument list */

    /* Terminal: Keywords */
    SPLT_KWD_STRUCT = SPLT_KEYWORD_OFFSET, /* keyword: struct */
    SPLT_KWD_ENUM,                         /* keyword: enum */
    SPLT_KWD_UNION,                        /* keyword: union */

    SPLT_AUTO,     /* keyword: auto */
    SPLT_EXTERN,   /* keyword: extern */
    SPLT_REGISTER, /* keyword: register */
    SPLT_STATIC,   /* keyword: static */
    SPLT_TYPEDEF,  /* keyword: typedef */

    SPLT_KWD_CONST, /* keyword: const */
    SPLT_RESTRICT,  /* keyword: restrict */
    SPLT_VOLATILE,  /* keyword: volatile */

    SPLT_ELLIPSIS, /* keyword: dots for var args */

    SPLT_INLINE, /* keyword: inline */

    SPLT_WHILE, /* keyword: while */
    SPLT_FOR,   /* keyword: for */
    SPLT_DO,    /* keyword: do */

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
    SPLT_LC = SPLT_PUNCTUATOR_OFFSET, /* left curly bracket */
    SPLT_RC,                          /* right curly bracket */
    SPLT_LP,                          /* left parenthesis */
    SPLT_RP,                          /* right parenthesis */

    SPLT_SEMI,  /* semicolon */
    SPLT_COMMA, /* comma */

    SPLT_QM = SPLT_OPERATOR_OFFSET, /* question mark */
    SPLT_COLON,                     /* colon */

    SPLT_LSB, /* left square bracket */
    SPLT_RSB, /* right square bracket */

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
    SPLT_TYPE_VOID = SPLT_BUILTIN_TYPE_OFFSET, /* type: void */
    SPLT_TYPE_INT,                             /* type: int */
    SPLT_TYPE_SIGNED,                          /* type: signed */
    SPLT_TYPE_UNSIGNED,                        /* type: unsigned */
    SPLT_TYPE_LONG,                            /* type: long */
    SPLT_TYPE_FLOAT,                           /* type: float */
    SPLT_TYPE_CHAR,                            /* type: char */
    
    SPLT_ID = SPLT_ID_OFFSET, /* id literal */
    SPLT_TYPEDEF_NAME,        /* type name through typedef declaration */

    /* Nonterminals: Macro Expressions */
    SPLT_MACRO_MNTPT = SPLT_MACRO_MNTPT_OFFSET, /* AST: macro mount point */
    SPLT_MACRO_ID,                              /* macro ID */
    SPLT_MACRO_DEFINE,                          /* macro: define */
    SPLT_MACRO_IFDEF,                           /* macro: ifdef */
    SPLT_MACRO_IFNDEF,                          /* macro: ifndef */
    SPLT_MACRO_ENDIF,                           /* macro: endif */

    /* Terminals: Literals */
    SPLT_CONSTANT = SPLT_LITERAL_OFFSET, /* constants */
    SPLT_LTR_INT,                        /* integer literal */
    SPLT_LTR_FLOAT,                      /* float literal scientific notation allowed */
    SPLT_LTR_CHAR,                       /* character literal */
    SPLT_LTR_STR,                        /* string literal */
    SPLT_STR_UNIT,                       /* basic string unit, requires free(val) */
};

#define SPLT_IS_EXPR(x) (((x) & SPLT_EXPR_OFFSET) == SPLT_EXPR_OFFSET)
#define SPLT_IS_STMT(x) (((x) & SPLT_STMT_OFFSET) == SPLT_STMT_OFFSET)
#define SPLT_IS_KEYWORD(x) (((x) & SPLT_KEYWORD_OFFSET) == SPLT_KEYWORD_OFFSET)
#define SPLT_IS_PUNCTUATOR(x) (((x) & SPLT_PUNCTUATOR_OFFSET) == SPLT_PUNCTUATOR_OFFSET)
#define SPLT_IS_OPERATOR(x) (((x) & SPLT_OPERATOR_OFFSET) == SPLT_OPERATOR_OFFSET)
#define SPLT_IS_BUILTIN_TYPE(x) (((x) & SPLT_BUILTIN_TYPE_OFFSET) == SPLT_BUILTIN_TYPE_OFFSET)
#define SPLT_IS_ID(x) ((x) == SPLT_ID)
#define SPLT_IS_MACRO(x) (((x) & SPLT_MACRO_MNTPT_OFFSET) == SPLT_MACRO_MNTPT_OFFSET)
#define SPLT_IS_LITERAL(x) (((x) & SPLT_LITERAL_OFFSET) == SPLT_LITERAL_OFFSET)

#define SPLT_IS_VAL_ALLOCATED(x) ((x) == SPLT_STR_UNIT)

/* Convert a token to string. The caller shall not free this string. */
const char *splc_token2str(splc_token_t type);

/* Get the color code of this token type to color the output in console. */
const char *splc_get_token_color_code(splc_token_t type);

/* The type of the entry in LUT for semantic analysis. */
typedef enum splc_entry_type splc_entry_t;

enum splc_entry_type
{
    SPLE_NULL,
    SPLE_FUNC,
    SPLE_STRUCT_DEC,
    SPLE_TYPEDEF,
    SPLE_UNION_DEC,
    SPLE_ENUM_DEC,
    SPLE_VAR,

    SPLE_MACRO_FUNC,
    SPLE_MACRO_VAR,
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

#if defined (__unix__) || defined (unix) || defined (__unix) || \
    defined (__APPLE__) || defined (__MACH__) || \
    defined (__linux) || defined (linux) || defined (__linux__)
#define SYSTEM_PATH_SEPARATOR '/'
#elif defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
#define SYSTEM_PATH_SEPARATOR '\\'
#else
#error "Unidentified platform should support their system path separator here"
#endif

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

/* Translation Unit */
typedef struct splc_trans_unit_struct *splc_trans_unit;

typedef struct splc_trans_unit_struct
{
    lut_table global_symtable; /* symbol table of this translation unit */
    ast_node root;             /* root of this translation unit */

    lut_table *envs; /* environment stack of this translation unit */
    size_t nenvs;    /* current number of environments */            

    int err_count;
    int warn_count;
} splc_trans_unit_struct;

/* create an empty translation unit with all fields initialized to 0/NULL. */
splc_trans_unit splc_create_empty_trans_unit();

/* create an empty translation unit and initialize the symbol table. */
splc_trans_unit splc_create_trans_unit();

/* push an empty symbol table into the environment of this translation unit */
lut_table splc_push_symtable(splc_trans_unit tunit, int scope);

/* pop and return the symbol table at the top of the environment of this translation unit.
   This function manages the internal stack state.
   This function will NOT free the underlying symbol table. */
lut_table splc_pop_symtable(splc_trans_unit tunit);

splc_trans_unit splc_link_trans_units();

#define SPLC_TRANS_UNIT_ENV_TOP(tunit) \
    (tunit)->envs[(tunit)->nenvs - 1]

/* Passed splc arguments */

extern int splc_incl_dir_cnt; /* Number of include directories. */

/* List of include directories. Each entry ends with the directory
   separator, so that they can be directly concatenated. This array must
   end with `NULL`. */
extern const char **splc_incl_dirs;

extern int splc_src_file_cnt; /* Number of source files. */

extern const char **splc_src_files; /* Source files */

#include "splcflag.h"

#define SPLC_OPT_REQUIRE_AST_PREP (splcf_enable_ast_punctuators)

/* splc internal global variables */

extern int splc_file_node_cnt; /* How many files have splc encountered */

/* All previously appeared files will be stored there. They will be indexed using their IDs. */
extern util_file_node *splc_all_file_nodes;

/* The root of linked list files. The root marks the previous file. */
extern util_file_node splc_file_node_stack;

/* Translation units parsed from `yyparse()` on different source files */
extern splc_trans_unit *splc_trans_unit_list;

/* The translation unit that is being parsed currently */
extern splc_trans_unit current_trans_unit;

extern int err_count;

extern int warn_count;

extern const char *progname;

extern const char *progversion;

/* Macros */

#define SPLC_GET_CURRENT_FID (splc_file_node_stack[0].fid)

#define SPLC_ROOT_LOC (splc_root_loc)
/* Check if the location is root (i.e., assigned from static global constant) */
#define SPLC_IS_LOC_ROOT(_loc) ((_loc).fid == -1)

#define SPLC_INVALID_LOC (splc_invalid_loc)
/* Check if the location is invalid (i.e., assigned from static global constant) */
#define SPLC_IS_LOC_INVALID(_loc) ((_loc).fid == -1)

/* Make a splc_loc struct with specific fid, lines and columns */
#define SPLC_MAKE_LOC(_fid, _linebegin, _colbegin, _lineend, _colend)                                                  \
    (splc_loc)                                                                                                         \
    {                                                                                                                  \
        .fid = (_fid), .linebegin = (_linebegin), .colbegin = (_colbegin), .lineend = (_lineend), .colend = (_colend)  \
    }

/* Make a splc_loc struct from current file, by specifying line and column numbers. */
#define SPLC_MAKE_LOC_CF(_linebegin, _colbegin, _lineend, _colend)                                                     \
    (splc_loc)                                                                                                         \
    {                                                                                                                  \
        .fid = splc_file_node_stack->fid, .linebegin = (_linebegin), .colbegin = (_colbegin), .lineend = (_lineend),   \
        .colend = (_colend)                                                                                            \
    }

#define SPLC_UNPACK_YYLLOC(x) x.first_line, x.first_column, x.last_line, x.last_column
/* Make a splc_loc struct directly from current file and yylloc */
#define SPLC_YY2LOC_CF_D(x) SPLC_MAKE_LOC_CF((x).first_line, (x).first_column, (x).last_line, (x).last_column)
/* Make a splc_loc struct explicitly with single point from line number and column number */
#define SPLC_YY2LOC_CF_1_PNT_E(_lineno, _colno) SPLC_MAKE_LOC_CF((_lineno), (_colno), (_lineno), (_colno))

/* Make a splc_loc struct from _loc from current file by capturing the entire interval */
#define SPLC_YY2LOC_CF_1_PNT_I(loc)                                                                                    \
    SPLC_MAKE_LOC_CF((loc).first_line, (loc).first_column, (loc).last_line, (loc).last_column)
/* Make a splc_loc struct from _loc from current file by capturing the first point */
#define SPLC_YY2LOC_CF_1_PNT_F(loc)                                                                                    \
    SPLC_MAKE_LOC_CF((loc).first_line, (loc).first_column, (loc).first_line, (loc).first_column)
/* Make a splc_loc struct from _loc from current file by capturing the last point */
#define SPLC_YY2LOC_CF_1_PNT_L(loc)                                                                                    \
    SPLC_MAKE_LOC_CF((loc).last_line, (loc).last_column, (loc).last_line, (loc).last_column)

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

/* Convert spl_loc to the first/last point of itself */
#define SPLC_SPLLOC_TO_1_PNT_F(loc)                                                                                    \
    SPLC_MAKE_LOC((loc).fid, (loc).first_line, (loc).first_column, (loc).first_line, (loc).first_column)
/* Convert spl_loc to the first/last point of itself */
#define SPLC_SPLLOC_TO_1_PNT_L(loc)                                                                                    \
    SPLC_MAKE_LOC((loc).fid, (loc).last_line, (loc).last_column, (loc).last_line, (loc).last_column)

#endif /* SPLCDEF_H */