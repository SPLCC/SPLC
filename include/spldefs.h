/* This file defines tokens of AST. */
#ifndef SPLDEFS_H
#define SPLDEFS_H

typedef enum spltype spltype_t;

enum spltype {
    PROGRAM,
    EXT_DEF_LIST,
    EXT_DEF,
    EXT_DEC_LIST,
    SPECIFIER,
    STRUCT_SPECIFIER,
    VAR_DEC, /* variable declaration */
    FUN_DEC, /* function declaration in the form of `foo(type1 var1, type2 var2, ...) */
    VAR_LIST, /* variable list (parameter list) */
    PARAM_DEC, /* single parameter declaration */
};

const char* get_spltype_string(spltype_t type);

#endif