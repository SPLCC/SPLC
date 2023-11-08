%code requires {
    #include "splcdef.h"
    #include "ast.h"
    #include "lut.h"
    #include "semantics.h"
    #include "utils.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    void yyerror(const char*);
}
%define parse.error detailed
%define api.value.type { ast_node }
%locations

%token LTR_INT LTR_FLOAT LTR_CHAR
%token TYPE_VOID TYPE_INT TYPE_SIGNED TYPE_UNSIGNED TYPE_LONG TYPE_FLOAT TYPE_CHAR STR_UNIT
%token AUTO EXTERN REGISTER STATIC TYPEDEF
%token KWD_CONST RESTRICT VOLATILE
%token PARAM_DOTS
%token INLINE
%token KWD_ENUM KWD_STRUCT KWD_UNION
%token ID TYPEDEF_NAME 
%token WHILE FOR DO
%token IF ELSE SWITCH DEFAULT CASE 
%token GOTO CONTINUE BREAK RETURN
%token SEMI COMMA QM COLON
/* assignment operators */
%token ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN PLUS_ASSIGN MINUS_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN BW_AND_ASSIGN BW_XOR_ASSIGN BW_OR_ASSIGN

/* conditional/bitwise operators */
%token LSHIFT RSHIFT
%token BW_AND BW_OR BW_NOT BW_XOR
%token AND OR NOT

/* other operators */
%token SIZEOF
%token LT LE GT GE NE EQ 
%token PLUS MINUS DPLUS DMINUS ASTRK DIV MOD
%token LC RC LP RP LSB RSB 
%token DOT RARROW

%precedence THEN
%precedence ELSE

%left COMMA
%right ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN PLUS_ASSIGN MINUS_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN BW_AND_ASSIGN BW_XOR_ASSIGN BW_OR_ASSIGN
%right QM COLON
%left OR
%left AND
%left BW_OR
%left BW_XOR
%left BW_AND
%left LT LE GT GE NE EQ 
%left PLUS MINUS
%left ASTRK DIV MOD
%right UPLUS UMINUS
%right NOT BW_NOT DPLUS DMINUS DEREF ADDROF SIZEOF
%left LP RP LSB RSB DOT POST_INCR POST_DECR

/* %destructor { destruct_node($$); } <> */

/* Reference: https://learn.microsoft.com/en-us/cpp/c-language/ */
/* Many of the C language features are not implemented here. */

%%
/* Entire translation unit */
translation-unit: 
      external-declaration-list {
        current_trans_unit->root = $$ = ast_create_parent_node(SPLT_TRANS_UNIT, 1, $1);
      }
    | { current_trans_unit->root = $$ = NULL; }
    ;

/* External definition list: Recursive definition */
external-declaration-list: 
      external-declaration { $$ = ast_create_parent_node(SPLT_EXT_DECLTN_LIST, 1, $1); }
    | external-declaration-list external-declaration { $$ = ast_add_child($1, $2); }
    ;

/* External definition list: A single unit of one of {variables, structs, functions}. */
external-declaration: 
      SEMI { $$ = ast_create_parent_node(SPLT_EXT_DECLTN, 1, $1); }
    | declaration { $$ = ast_create_parent_node(SPLT_EXT_DECLTN, 1, $1); }
    | function-definition { $$ = ast_create_parent_node(SPLT_EXT_DECLTN, 1, $1); }
    ;

declaration-specifiers:
      storage-class-specifier { $$ = ast_create_parent_node(SPLT_DECLTN_SPEC, 1, $1); }
    | type-specifier { $$ = ast_create_parent_node(SPLT_DECLTN_SPEC, 1, $1); }
    | type-qualifier { $$ = ast_create_parent_node(SPLT_DECLTN_SPEC, 1, $1); }
    | function-specifier { $$ = ast_create_parent_node(SPLT_DECLTN_SPEC, 1, $1); }
    | declaration-specifiers type-specifier { $$ = ast_add_child($1, $2); }
    | declaration-specifiers storage-class-specifier { $$ = ast_add_child($1, $2); }
    | declaration-specifiers type-qualifier { $$ = ast_add_child($1, $2); }
    | declaration-specifiers function-specifier { $$ = ast_add_child($1, $2); }
    ;

storage-class-specifier:
      AUTO { $$ = ast_create_parent_node(SPLT_STRG_SPEC, 1, $1); }
    | EXTERN { $$ = ast_create_parent_node(SPLT_STRG_SPEC, 1, $1); }
    | REGISTER { $$ = ast_create_parent_node(SPLT_STRG_SPEC, 1, $1); }
    | STATIC { $$ = ast_create_parent_node(SPLT_STRG_SPEC, 1, $1); }
    | TYPEDEF { $$ = ast_create_parent_node(SPLT_STRG_SPEC, 1, $1); }
    ;

specifier-qualifier-list:
      type-specifier { $$ = ast_create_parent_node(SPLT_SPEC_QUAL_LIST, 1, $1); }
    | type-qualifier { $$ = ast_create_parent_node(SPLT_SPEC_QUAL_LIST, 1, $1); }
    | specifier-qualifier-list type-specifier { $$ = ast_add_child($1, $2); }
    | specifier-qualifier-list type-qualifier { $$ = ast_add_child($1, $2); }
    ;

type-specifier: 
      builtin-type-specifier { $$ = ast_create_parent_node(SPLT_TYPE_SPEC, 1, $1); }
    /* | identifier { $$ = ast_create_parent_node(SPLT_TYPE_SPEC, 1, $1); } */
    | struct-or-union-specifier { $$ = ast_create_parent_node(SPLT_TYPE_SPEC, 1, $1); }
    | enum-specifier { $$ = ast_create_parent_node(SPLT_TYPE_SPEC, 1, $1); }
    | TYPEDEF_NAME { $$ = ast_create_parent_node(SPLT_TYPE_SPEC, 1, $1); }
    ;

function-specifier:
      INLINE { $$ = ast_create_parent_node(SPLT_FUNC_SPEC, 1, $1); }
    ;

type-qualifier:
      KWD_CONST { $$ = ast_create_parent_node(SPLT_TYPE_QUAL, 1, $1); }
    | RESTRICT { $$ = ast_create_parent_node(SPLT_TYPE_QUAL, 1, $1); }
    | VOLATILE { $$ = ast_create_parent_node(SPLT_TYPE_QUAL, 1, $1); }
    ;

type-name:
      specifier-qualifier-list { $$ = ast_create_parent_node(SPLT_TYPENAME, 1, $1); }
    | specifier-qualifier-list abstract-declarator { $$ = ast_create_parent_node(SPLT_TYPENAME, 2, $1, $2); }
    ;

builtin-type-specifier:
      TYPE_VOID
    | TYPE_INT
    | TYPE_FLOAT
    | TYPE_CHAR
    | TYPE_SIGNED
    | TYPE_UNSIGNED
    | TYPE_LONG 
    ;

abstract-declarator:
      pointer { $$ = ast_create_parent_node(SPLT_ABS_DEC, 1, $1); }
    | pointer direct-abstract-declarator { $$ = ast_create_parent_node(SPLT_ABS_DEC, 2, $1, $2); }
    ;

direct-abstract-declarator:
      LP abstract-declarator RP { $$ = ast_create_parent_node(SPLT_DIR_ABS_DEC, 3, $1, $2, $3); }
    | direct-abstract-declarator LSB assignment-expression RSB { $$ = ast_add_children($1, 3, $2, $3, $4); }
    | direct-abstract-declarator LSB RSB { $$ = ast_add_children($1, 2, $2, $3); }
    | direct-abstract-declarator LSB error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect ']' here"); $$ = ast_add_children($1, 1, $2); yyerrok; }
    | direct-abstract-declarator RSB { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($2), "expect '[' here"); $$ = ast_add_children($1, 1, $2); yyerrok; } 
    ;

/* Specify a structure */
struct-or-union-specifier: 
      struct-or-union identifier { $$ = ast_create_parent_node(SPLT_STRUCT_UNION_SPEC, 2, $1, $2); }
    | struct-or-union LC RC { $$ = ast_create_parent_node(SPLT_STRUCT_UNION_SPEC, 3, $1, $2, $3); }
    | struct-or-union identifier LC RC { $$ = ast_create_parent_node(SPLT_STRUCT_UNION_SPEC, 4, $1, $2, $3, $4); }
    | struct-or-union LC struct-declaration-list RC { $$ = ast_create_parent_node(SPLT_STRUCT_UNION_SPEC, 4, $1, $2, $3, $4); }
    | struct-or-union identifier LC struct-declaration-list RC { $$ = ast_create_parent_node(SPLT_STRUCT_UNION_SPEC, 5, $1, $2, $3, $4, $5); }

    | struct-or-union LC error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect token '}'"); $$ = ast_create_parent_node(SPLT_STRUCT_UNION_SPEC, 2, $1, $2); yyerrok; }
    | struct-or-union identifier LC error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expect token '}'"); $$ = ast_create_parent_node(SPLT_STRUCT_UNION_SPEC, 3, $1, $2, $3); yyerrok; }
    | struct-or-union LC struct-declaration-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expect token '}'"); $$ = ast_create_parent_node(SPLT_STRUCT_UNION_SPEC, 3, $1, $2, $3); yyerrok; }
    | struct-or-union identifier LC struct-declaration-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($4), "expect token '}'"); $$ = ast_create_parent_node(SPLT_STRUCT_UNION_SPEC, 4, $1, $2, $3, $4); yyerrok; }
    ;

struct-or-union:
      KWD_STRUCT
    | KWD_UNION
    ;

struct-declaration-list:
      struct-declaration { $$ = ast_create_parent_node(SPLT_STRUCT_DECLTN_LIST, 1, $1); }
    | struct-declaration-list struct-declaration { $$ = ast_add_child($1, $2); }
    ;

struct-declaration:
      specifier-qualifier-list SEMI { $$ = ast_create_parent_node(SPLT_STRUCT_DECLTN, 2, $1, $2); }
    | specifier-qualifier-list struct-declarator-list SEMI { $$ = ast_create_parent_node(SPLT_STRUCT_DECLTN, 2, $1, $2); }

    | specifier-qualifier-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "expect ';' here"); $$ = ast_create_parent_node(SPLT_STRUCT_DECLTN, 1, $1); yyerrok; }
    | specifier-qualifier-list struct-declarator-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect ';' here"); $$ = ast_create_parent_node(SPLT_STRUCT_DECLTN, 2, $1, $2); yyerrok; }
    ;

struct-declarator-list:
      struct-declarator { $$ = ast_create_parent_node(SPLT_STRUCT_DEC_LIST, 1, $1); }
    | struct-declarator-list COMMA struct-declarator { $$ = ast_add_children($1, 2, $2, $3); }

    | struct-declarator-list COMMA error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect struct declarator here");; $$ = ast_add_child($1, $2); yyerrok; }
    ;

struct-declarator:
      declarator { $$ = ast_create_parent_node(SPLT_STRUCT_DEC, 1, $1); }
    | COLON constant-expression { $$ = ast_create_parent_node(SPLT_STRUCT_DEC, 2, $1, $2); }
    | declarator COLON constant-expression { $$ = ast_create_parent_node(SPLT_STRUCT_DEC, 3, $1, $2, $3); }

    | COLON error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "expect expression here"); $$ = ast_create_parent_node(SPLT_STRUCT_DEC, 1, $1); yyerrok; }
    | declarator COLON error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect expression here"); $$ = ast_create_parent_node(SPLT_STRUCT_DEC, 2, $1, $2); yyerrok; }
    ;

enum-specifier:
      KWD_ENUM identifier { $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 2, $1, $2); }
    | KWD_ENUM LC RC { $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 3, $1, $2, $3); }
    | KWD_ENUM identifier LC RC { $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 4, $1, $2, $3, $4); }
    | KWD_ENUM LC enumerator-list RC { $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 4, $1, $2, $3, $4); }
    | KWD_ENUM identifier LC enumerator-list RC { $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 5, $1, $2, $3, $4, $5); }
    | KWD_ENUM LC enumerator-list COMMA RC { $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 5, $1, $2, $3, $4, $5); }
    | KWD_ENUM identifier LC enumerator-list COMMA RC { $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 6, $1, $2, $3, $4, $5, $6); }
    
    | KWD_ENUM error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "expect identifier here"); $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 1, $1); yyerrok; }
    | KWD_ENUM LC error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect token ',' or '}'"); $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 2, $1, $2); yyerrok; }
    | KWD_ENUM identifier LC error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expect token ',' or '}'"); $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 3, $1, $2, $3); yyerrok; }
    | KWD_ENUM LC enumerator-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expect token ',' or '}'"); $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 3, $1, $2, $3); yyerrok; }
    | KWD_ENUM identifier LC enumerator-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($4), "expect token ',' or '}'"); $$ = ast_create_parent_node(SPLT_ENUM_SPEC, 4, $1, $2, $3, $4); yyerrok; }
    ;

enumerator-list:
      enumerator { $$ = ast_create_parent_node(SPLT_ENUM_LIST, 1, $1); }
    | enumerator-list COMMA enumerator { $$ = ast_add_children($1, 2, $2, $3); }

    | COMMA enumerator { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expect identifier here ");; $$ = ast_create_parent_node(SPLT_ENUM_LIST, 2, $1, $2); yyerrok; }
    ;

enumerator:
      enumeration-constant { $$ = ast_create_parent_node(SPLT_ENUM, 1, $1); }
    | enumeration-constant ASSIGN constant-expression { $$ = ast_create_parent_node(SPLT_ENUM, 3, $1, $2, $3); }

    | enumeration-constant ASSIGN error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect a constant expression"); $$ = ast_create_parent_node(SPLT_ENUM, 2, $1, $2); yyerrok; }
    ;

enumeration-constant:
      identifier
    ;

/* Single variable declaration */
declarator: 
      pointer direct-declarator { $$ = ast_create_parent_node(SPLT_DEC, 2, $1, $2); }
    | direct-declarator { $$ = ast_create_parent_node(SPLT_DEC, 1, $1); }
    ;

direct-declarator:
      identifier { $$ = ast_create_parent_node(SPLT_DIR_DEC, 1, $1); }
    | LP declarator RP { $$ = ast_create_parent_node(SPLT_DIR_DEC, 3, $1, $2, $3); }
    | direct-declarator LSB assignment-expression RSB { $$ = ast_add_children($1, 3, $2, $3, $4); }
    | direct-declarator LSB RSB { $$ = ast_add_children($1, 2, $2, $3); }

    | direct-declarator LSB assignment-expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "missing closing bracket ']'"); $$ = ast_add_children($1, 2, $2, $3); yyerrok; } 
    /* | direct-declarator error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expect '[' here"); $$ = ast_create_parent_node(SPLT_DIR_DEC, 0); yyerrok; }  */
    | direct-declarator RSB { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($2), "expect '[' here"); $$ = ast_add_children($1, 1, $2); yyerrok; } 
    ;

pointer:
      ASTRK { $$ = ast_create_parent_node(SPLT_PTR, 1, $1); }
    | ASTRK type-qualifier-list { $$ = ast_create_parent_node(SPLT_PTR, 2, $1, $2); }
    | pointer ASTRK { $$ = ast_add_child($1, $2); }
    | pointer ASTRK type-qualifier-list { $$ = ast_add_children($1, 2, $2, $3); }
    ;

type-qualifier-list:
      type-qualifier { $$ = ast_create_parent_node(SPLT_TYPE_QUAL_LIST, 1, $1); } 
    | type-qualifier-list type-qualifier { $$ = ast_add_child($1, $2); }
    ;

/* Definition: List of definitions. Recursive definition. */
/* declaration-list: 
      declaration { $$ = ast_create_parent_node(SPLT_DECLTN_LIST, 1, $1); }
    | declaration-list declaration { $$ = ast_add_child($1, $2); }
    ; */

/* Definition: Base */
declaration: 
      direct-declaration SEMI { $$ = ast_create_parent_node(SPLT_DECLTN, 2, $1, $2); }
    | direct-declaration error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "missing semicolon ';'"); $$ = ast_create_parent_node(SPLT_DECLTN, 1, $1); yyerrok; }
    ;

direct-declaration:
      declaration-specifiers { $$ = ast_create_parent_node(SPLT_DIR_DECLTN, 1, $1); }
    | declaration-specifiers init-declarator-list { 
          $$ = ast_create_parent_node(SPLT_DIR_DECLTN, 2, $1, $2); 
          // EXPERIMENTAL
          sem_register_typedef($$);
      }
    ;

/* Definition: Declaration of multiple variable.  */ 
init-declarator-list: 
      init-declarator { $$ = ast_create_parent_node(SPLT_INIT_DEC_LIST, 1, $1); }
    | init-declarator COMMA init-declarator-list { $$ = ast_create_parent_node(SPLT_INIT_DEC_LIST, 3, $1, $2, $3); }

    | init-declarator COMMA { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expect declarator"); $$ = ast_create_parent_node(SPLT_INIT_DEC_LIST, 2, $1, $2); yyerrok; }
    | COMMA init-declarator-list { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expect declarator"); $$ = ast_create_parent_node(SPLT_INIT_DEC_LIST, 2, $1, $2); yyerrok; }
    | COMMA { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expect declarator"); $$ = ast_create_parent_node(SPLT_INIT_DEC_LIST, 1, $1); yyerrok; }
    ;

/* Definition: Single declaration unit. */
init-declarator: 
      declarator { $$ = ast_create_parent_node(SPLT_INIT_DEC, 1, $1); }
    | declarator ASSIGN initializer { $$ = ast_create_parent_node(SPLT_INIT_DEC, 3, $1, $2, $3); }
    | declarator ASSIGN error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "invalid initialization"); $$ = ast_create_parent_node(SPLT_INIT_DEC, 0); yyerrok; }
    ;

initializer:
      assignment-expression { $$ = ast_create_parent_node(SPLT_INIT, 1, $1); }
    | LC initializer-list RC { $$ = ast_create_parent_node(SPLT_INIT, 3, $1, $2, $3); }
    | LC initializer-list COMMA RC { $$ = ast_create_parent_node(SPLT_INIT, 4, $1, $2, $3, $4); }
    | LC initializer-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect token '}' or ','"); $$ = ast_create_parent_node(SPLT_INIT, 2, $1, $2); }
    ;

initializer-list:
      initializer { $$ = ast_create_parent_node(SPLT_INIT_LIST, 1, $1); }
    | designation initializer { $$ = ast_create_parent_node(SPLT_INIT_LIST, 2, $1, $2); }
    | initializer-list COMMA designation initializer { $$ = ast_add_children($1, 3, $2, $3, $4); }
    | initializer-list COMMA initializer { $$ = ast_add_children($1, 2, $2, $3); }

    | designation error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "expect initializer"); $$ = ast_create_parent_node(SPLT_INIT_LIST, 1, $1); yyerrok; }
    | initializer-list COMMA error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect initializer"); $$ = ast_create_parent_node(SPLT_INIT_LIST, 2, $1, $2); }
    ;

designation:
      designator-list ASSIGN { $$ = ast_create_parent_node(SPLT_DESGTN, 2, $1, $2); }
    ;

designator-list:
      designator { $$ = ast_create_parent_node(SPLT_DESG_LIST, 1, $1); }
    | designator-list designator { $$ = ast_create_parent_node(SPLT_DESG_LIST, 2, $1, $2); }
    ;

designator:
      LSB constant-expression RSB { $$ = ast_create_parent_node(SPLT_DESG, 3, $1, $2, $3); }
    | DOT identifier { $$ = ast_create_parent_node(SPLT_DESG, 2, $1, $2); }

    | LSB constant-expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect ']'"); $$ = ast_create_parent_node(SPLT_DESG, 2, $1, $2); yyerrok; }
    | DOT error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "expect valid identifier"); $$ = ast_create_parent_node(SPLT_DESG, 1, $1); yyerrok; }
    ;

function-definition:
      declaration-specifiers function-declarator compound-statement { 
          $$ = ast_create_parent_node(SPLT_FUNC_DEF, 3, $1, $2, $3); 
          // EXPERIMENTAL
          sem_register_typedef($$);
      }
    | function-declarator compound-statement { 
          splcwarn(SPLC_AST_GET_STARTLOC($1), "function is missing a specifier and will default to 'int'"); 
          $$ = ast_create_parent_node(SPLT_FUNC_DEF, 2, $1, $2); 
          // EXPERIMENTAL
          sem_register_typedef($$);
      } 
    | declaration-specifiers function-declarator SEMI { 
          $$ = ast_create_parent_node(SPLT_FUNC_DEF, 3, $1, $2, $3); 
          // EXPERIMENTAL
          sem_register_typedef($$);
      }

    | declaration-specifiers function-declarator error { 
          splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($3), "missing valid function body. Did you forget to put '{'?"); 
          $$ = ast_create_parent_node(SPLT_FUNC_DEF, 2, $1, $2);
          // EXPERIMENTAL
          sem_register_typedef($$);
          yyerrok; 
      }
    ;

/* Function: Function name and body. */
function-declarator: 
      direct-function-declarator { $$ = ast_create_parent_node(SPLT_FUNC_DEC, 1, $1); }
    | pointer direct-function-declarator { $$ = ast_create_parent_node(SPLT_FUNC_DEC, 2, $1, $2); }
    ;

direct-function-declarator:
      direct-declarator LP parameter-type-list RP { $$ = ast_create_parent_node(SPLT_DIR_FUNC_DEC, 4, $1, $2, $3, $4); }
    | direct-declarator LP RP { $$ = ast_create_parent_node(SPLT_DIR_FUNC_DEC, 3, $1, $2, $3); }

    | direct-declarator LP error RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "invalid parameter declaration ')'"); $$ = ast_create_parent_node(SPLT_DIR_FUNC_DEC, 0); yyerrok; }
    | direct-declarator LP parameter-type-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "missing closing parenthesis ')'"); $$ = ast_create_parent_node(SPLT_DIR_FUNC_DEC, 0); yyerrok; }
    | direct-declarator LP error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "missing closing parenthesis ')'"); $$ = ast_create_parent_node(SPLT_DIR_FUNC_DEC, 0); yyerrok; }

    | LP parameter-type-list RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing identifier for function"); $$ = ast_create_parent_node(SPLT_DIR_FUNC_DEC, 3, $1, $2, $3); yyerrok; }
    | LP RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing identifier for function"); $$ = ast_create_parent_node(SPLT_DIR_FUNC_DEC, 2, $1, $2); yyerrok; }
    ;

/* List of variables names */
parameter-type-list: 
      parameter-list { $$ = ast_create_parent_node(SPLT_PARAM_TYPE_LIST, 1, $1); }
    | parameter-list COMMA PARAM_DOTS { $$ = ast_create_parent_node(SPLT_PARAM_TYPE_LIST, 3, $1, $2, $3); }
    ;

parameter-list:
      parameter-declaration { $$ = ast_create_parent_node(SPLT_PARAM_LIST, 1, $1); }
    | parameter-list COMMA parameter-declaration { $$ = ast_add_children($1, 2, $2, $3); }

    | parameter-list COMMA { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expect parameter delcaration"); $$ = ast_add_child($1, $2); yyerrok; }
    | COMMA { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expect parameter delcaration"); $$ = ast_create_parent_node(SPLT_PARAM_LIST, 1, $1); yyerrok; }
    ;

/* Parameter declaration */ 
parameter-declaration: 
      declaration-specifiers declarator { $$ = ast_create_parent_node(SPLT_PARAM_DEC, 2, $1, $2); }
    | declaration-specifiers abstract-declarator { $$ = ast_create_parent_node(SPLT_PARAM_DEC, 2, $1, $2); }
    | declaration-specifiers { $$ = ast_create_parent_node(SPLT_PARAM_DEC, 1, $1); }

    /* | error { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "missing specifier"); $$ = ast_create_parent_node(SPLT_PARAM_DEC, 0); yyerrok; } */
    ;

/* Compound statement: A new scope. */
compound-statement: 
      /* LC general-statement-list RC */
      LC general-statement-list RC { $$ = ast_create_parent_node(SPLT_COMP_STMT, 3, $1, $2, $3); }
    | LC general-statement-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "missing closing bracket '}' "); $$ = ast_create_parent_node(SPLT_COMP_STMT, 0); yyerrok; }
    /* | error RC { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing opening bracket '{' "); $$ = ast_create_parent_node(SPLT_COMP_STMT, 0); yyerrok; } */

      /* LC RC */
    | LC RC { $$ = ast_create_parent_node(SPLT_COMP_STMT, 2, $1, $2); }
    | LC error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "missing closing bracket '}' "); $$ = ast_create_parent_node(SPLT_COMP_STMT, 0); yyerrok; }
    ;

/* wrapper for C99 standard for statements */
general-statement-list: 
      statement { $$ = ast_create_parent_node(SPLT_GEN_STMT_LIST, 1, $1); }
    | declaration { $$ = ast_create_parent_node(SPLT_GEN_STMT_LIST, 1, $1); }
    | general-statement-list statement { $$ = ast_add_child($1, $2); }
    | general-statement-list declaration { $$ = ast_add_child($1, $2); }
    ;

/* Statement: List of statements. Recursive definition. */
/* statement-list: 
      statement { $$ = ast_create_parent_node(SPLT_STMT_LIST, 1, $1); }
    | statement-list statement { $$ = ast_create_parent_node(SPLT_STMT_LIST, 2, $1, $2); } 
    ; */

/* Statement: A single statement. */
statement: 
      SEMI { $$ = ast_create_parent_node(SPLT_STMT, 1, $1); }
    | compound-statement { $$ = ast_create_parent_node(SPLT_STMT, 1, $1); }
    | expression-statement { $$ = ast_create_parent_node(SPLT_STMT, 1, $1); }
    | selection-statement { $$ = ast_create_parent_node(SPLT_STMT, 1, $1); }
    | iteration-statement { $$ = ast_create_parent_node(SPLT_STMT, 1, $1); }
    | labeled-statement { $$ = ast_create_parent_node(SPLT_STMT, 1, $1); }
    | jump-statement { $$ = ast_create_parent_node(SPLT_STMT, 1, $1); }

    /* | error SEMI { $$ = ast_create_parent_node(SPLT_STMT, 0); yyerrok; } */
    ;

expression-statement:
      expression SEMI { $$ = ast_create_parent_node(SPLT_EXPR_STMT, 2, $1, $2); }
    | expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "missing semicolon ';'"); $$ = ast_create_parent_node(SPLT_EXPR_STMT, 0); yyerrok; }
    ;

selection-statement:
      IF LP expression RP statement %prec THEN { $$ = ast_create_parent_node(SPLT_SEL_STMT, 5, $1, $2, $3, $4, $5); }

    | IF error RP statement %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expect '(' here"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 3, $1, $3, $4); yyerrok; }
    | IF LP RP statement %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expect an expression"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 4, $1, $2, $3, $4); yyerrok; }
    | IF LP expression RP error %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@4), "if requires at least one statement to be executed"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 4, $1, $2, $3, $4); yyerrok; }
    | IF LP RP error %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expect an expression"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 3, $1, $2, $3); yyerrok; }
    
    | IF LP expression RP statement ELSE statement %prec ELSE { $$ = ast_create_parent_node(SPLT_SEL_STMT, 7, $1, $2, $3, $4, $5, $6, $7); }

    | IF error RP statement ELSE statement %prec ELSE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expect '(' here"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 0); yyerrok; }
    | IF LP expression RP statement ELSE error %prec ELSE { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($6), "expect a statement"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 6, $1, $2, $3, $4, $5, $6); yyerrok; }
    | IF LP RP statement ELSE statement %prec ELSE { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an expression"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 6, $1, $2, $3, $4, $5, $6); yyerrok; }
    | IF LP RP statement ELSE error %prec ELSE { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expect expression and statement"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 5, $1, $2, $3, $4, $5); yyerrok; }
    | IF LP expression error %prec ELSE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 0); yyerrok; }
    | ELSE statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "dangling else is not allowed"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 2, $1, $2); yyerrok; }

    | SWITCH LP expression RP statement { $$ = ast_create_parent_node(SPLT_SEL_STMT, 5, $1, $2, $3, $4, $5); }
    /* | SWITCH LP expression statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing opening parenthese '('"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 0); yyerrok; } */
    | SWITCH error RP statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "missing opening parenthese '('"); $$ = ast_create_parent_node(SPLT_SEL_STMT, 3, $1, $3, $4); yyerrok; }
    ;

labeled-statement:
      identifier COLON statement { $$ = ast_create_parent_node(SPLT_LABELED_STMT, 3, $1, $2, $3); }
    | CASE constant-expression COLON statement { $$ = ast_create_parent_node(SPLT_LABELED_STMT, 3, $1, $2, $3); }
    | DEFAULT COLON statement { $$ = ast_create_parent_node(SPLT_LABELED_STMT, 3, $1, $2, $3); }

    | COLON statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing a label"); $$ = ast_create_parent_node(SPLT_LABELED_STMT, 0); yyerrok; }
    ;

jump-statement:
      GOTO identifier SEMI { $$ = ast_create_parent_node(SPLT_JUMP_STMT, 3, $1, $2, $3); }
    | CONTINUE SEMI { $$ = ast_create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); }
    | BREAK SEMI { $$ = ast_create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); }
    | RETURN expression SEMI { $$ = ast_create_parent_node(SPLT_JUMP_STMT, 3, $1, $2, $3); }
    | RETURN expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "missing semicolon ';'"); $$ = ast_create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); yyerrok; }
    | RETURN SEMI { $$ = ast_create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); }

    | RETURN error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "missing expression in return statement"); $$ = ast_create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); yyerrok; }
    ;

iteration-statement:
      WHILE LP expression RP statement { $$ = ast_create_parent_node(SPLT_ITER_STMT, 5, $1, $2, $3, $4, $5); }
    | WHILE error RP statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expect '(' here"); $$ = ast_create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    | WHILE LP expression RP error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($4), "while loop requires at least one statement to be executed"); $$ = ast_create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    | WHILE LP expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "missing closing parenthesis ')'"); $$ = ast_create_parent_node(SPLT_ITER_STMT, 0); yyerrok;  }
    
    | DO WHILE LP expression RP SEMI { $$ = ast_create_parent_node(SPLT_ITER_STMT, 6, $1, $2, $3, $4, $5, $6); }

    | FOR LP for-loop-body RP statement { $$ = ast_create_parent_node(SPLT_ITER_STMT, 5, $1, $2, $3, $4, $5); }
    | FOR LP for-loop-body RP error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($4), "for loop requires at least one statement to be executed"); $$ = ast_create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    | FOR LP for-loop-body error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "missing closing parenthesis ')'"); $$ = ast_create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    ;

for-loop-body: 
      initialization-expression SEMI expression SEMI expression { $$ = ast_create_parent_node(SPLT_FOR_LOOP_BODY, 5, $1, $2, $3, $4, $5); }

    | SEMI expression SEMI expression { $$ = ast_create_parent_node(SPLT_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }
    | initialization-expression SEMI expression SEMI { $$ = ast_create_parent_node(SPLT_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }
    | initialization-expression SEMI SEMI expression { $$ = ast_create_parent_node(SPLT_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }

    | SEMI expression SEMI { $$ = ast_create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    | SEMI SEMI expression { $$ = ast_create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    /* | definition SEMI { $$ = ast_create_parent_node(SPLT_FOR_LOOP_BODY, 2, $1, $2); } */
    | initialization-expression SEMI SEMI { $$ = ast_create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    
    | SEMI SEMI { $$ = ast_create_parent_node(SPLT_FOR_LOOP_BODY, 2, $1, $2); }
    ;

constant-expression: 
      conditional-expression { $$ = ast_create_parent_node(SPLT_CONST_EXPR, 1, $1); }
    ;

constant:
      LTR_INT { $$ = ast_create_parent_node(SPLT_CONSTANT, 1, $1); }
    | LTR_FLOAT { $$ = ast_create_parent_node(SPLT_CONSTANT, 1, $1); }
    | LTR_CHAR { $$ = ast_create_parent_node(SPLT_CONSTANT, 1, $1); }
    ;

primary-expression:
      identifier
    | constant { $$ = ast_create_parent_node(SPLT_EXPR, 1, $1); }
    | string-literal { $$ = ast_create_parent_node(SPLT_EXPR, 1, $1); }
    | LP expression RP { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | LP expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect ')'"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    /* | LP expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing closing parenthesis ')'"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; } */
    ;

postfix-expression:
      primary-expression
    | postfix-expression LSB expression RSB { $$ = ast_create_parent_node(SPLT_EXPR, 4, $1, $2, $3, $4); }
    | postfix-expression LP argument-list RP { $$ = ast_create_parent_node(SPLT_EXPR, 4, $1, $2, $3, $4); }
    | postfix-expression LP RP { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | postfix-expression member-access-operator identifier { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | postfix-expression DPLUS { $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | postfix-expression DMINUS { $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | LP type-name RP LC initializer-list RC { $$ = ast_create_parent_node(SPLT_EXPR, 6, $1, $2, $3, $4, $5, $6); }
    | LP type-name RP LC initializer-list COMMA RC { $$ = ast_create_parent_node(SPLT_EXPR, 7, $1, $2, $3, $4, $5, $6, $7); }

    | postfix-expression LSB expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expect ']'"); $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); yyerrok; }
    | postfix-expression LP argument-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expect ')'"); $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); yyerrok; }
    | postfix-expression member-access-operator { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expect an identifier"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | RARROW identifier { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | LP type-name RP LC initializer-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($5), "expect token ',' and '}' "); $$ = ast_create_parent_node(SPLT_EXPR, 5, $1, $2, $3, $4, $5); yyerrok; }
    ;

member-access-operator:
      DOT
    | RARROW
    ;

unary-expression:
      postfix-expression
    | DPLUS unary-expression { $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | DMINUS unary-expression { $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | unary-operator cast-expression %prec UPLUS { $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | SIZEOF unary-expression { $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | SIZEOF LP type-name RP { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | DPLUS error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 1, $1); yyerrok; }
    | DMINUS error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 1, $1); yyerrok; }
    | SIZEOF error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 1, $1); yyerrok; }
    /* | SIZEOF LP unary-expression RP {} */
    ;

unary-operator: /* Take the default behavior, that is, `$$ = $1` */
      BW_AND
    | ASTRK 
    | PLUS
    | MINUS
    | BW_NOT
    | NOT
    ;

cast-expression:
      unary-expression
    | LP type-name RP cast-expression { $$ = ast_create_parent_node(SPLT_CAST_EXPR, 4, $1, $2, $3, $4); }

    | LP type-name RP error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expect an expression"); $$ = ast_create_parent_node(SPLT_CAST_EXPR, 3, $1, $2, $3); yyerrok; }
    | LP type-name error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect ')'"); $$ = ast_create_parent_node(SPLT_CAST_EXPR, 2, $1, $2); yyerrok; }
    ;

multiplicative-expression:
      cast-expression
    | multiplicative-expression multiplicative-operator cast-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | multiplicative-expression multiplicative-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | division-operator cast-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;
  
multiplicative-operator:
      ASTRK
    | division-operator
    ;

division-operator:
      DIV
    | MOD
    ;

additive-expression:
      multiplicative-expression
    | additive-expression additive-operator multiplicative-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | additive-expression additive-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

additive-operator:
      PLUS
    | MINUS
    ;

shift-expression:
      additive-expression
    | shift-expression shift-operator additive-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | shift-expression shift-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | shift-operator additive-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;
  
shift-operator:
      LSHIFT
    | RSHIFT
    ;

relational-expression:
      shift-expression
    | relational-expression relational-operator shift-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | relational-expression relational-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | relational-operator shift-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

relational-operator:
      LT
    | GT
    | LE
    | GE
    ;

equality-expression:
      relational-expression
    | equality-expression equality-operator relational-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | equality-expression equality-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | equality-operator relational-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

equality-operator:
      EQ
    | NE
    ;

BW-AND-expression:
      equality-expression
    | BW-AND-expression BW_AND equality-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | BW-AND-expression BW_AND error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | BW_AND error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

BW-XOR-expression:
      BW-AND-expression
    | BW-XOR-expression BW_XOR BW-AND-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | BW-XOR-expression BW_XOR error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | BW_XOR BW-AND-expression { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

BW-OR-expression:
      BW-XOR-expression
    | BW-OR-expression BW_OR BW-XOR-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | BW-OR-expression BW_OR error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | BW_OR BW-XOR-expression { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

logical-AND-expression:
      BW-OR-expression
    | logical-AND-expression AND BW-OR-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | logical-AND-expression AND error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | AND BW-OR-expression { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

logical-OR-expression:
      logical-AND-expression
    | logical-OR-expression OR logical-AND-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | logical-OR-expression OR error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | OR logical-AND-expression { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

conditional-expression:
      logical-OR-expression { if (!SPLT_IS_EXPR($1->type)) $$ = ast_create_parent_node(SPLT_EXPR, 1, $1); else $$ = $1; }
    | logical-OR-expression QM expression COLON conditional-expression { $$ = ast_create_parent_node(SPLT_EXPR, 5, $1, $2, $3, $4, $5); }

    | logical-OR-expression QM COLON conditional-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expect an expression"); $$ = ast_create_parent_node(SPLT_EXPR, 4, $1, $2, $3, $4); yyerrok; }
    | logical-OR-expression QM expression COLON { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@4), "expect an expression"); $$ = ast_create_parent_node(SPLT_EXPR, 4, $1, $2, $3, $4); yyerrok; }
    | QM error { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expect an expression"); $$ = ast_create_parent_node(SPLT_EXPR, 1, $1); yyerrok; }
    ;

assignment-expression:
      conditional-expression { if (!SPLT_IS_EXPR($1->type)) $$ = ast_create_parent_node(SPLT_EXPR, 1, $1); else $$ = $1; }
    | unary-expression assignment-operator assignment-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | unary-expression assignment-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | assignment-operator assignment-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;
    
assignment-operator: /* Use the default behavior to pass the value */
      ASSIGN 
    | MUL_ASSIGN
    | DIV_ASSIGN
    | MOD_ASSIGN
    | PLUS_ASSIGN
    | MINUS_ASSIGN
    | LSHIFT_ASSIGN
    | RSHIFT_ASSIGN
    | BW_AND_ASSIGN
    | BW_XOR_ASSIGN
    | BW_OR_ASSIGN
    ;

/* expressions */
expression: 
      assignment-expression { if (!SPLT_IS_EXPR($1->type)) $$ = ast_create_parent_node(SPLT_EXPR, 1, $1); else $$ = $1; }
    | expression COMMA assignment-expression { $$ = ast_create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | expression COMMA error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | COMMA assignment-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expect an operand"); $$ = ast_create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;
  
initialization-expression:
      expression { $$ = ast_create_parent_node(SPLT_INIT_EXPR, 1, $1); }
    | direct-declaration { $$ = ast_create_parent_node(SPLT_INIT_EXPR, 1, $1); }
    ;

/* Argument: List of arguments */
argument-list: 
      argument-list COMMA assignment-expression { $$ = ast_add_children($1, 2, $2, $3); }
    | assignment-expression { $$ = ast_create_parent_node(SPLT_ARG_LIST, 1, $1); }

    | argument-list COMMA error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expect an operand"); $$ = ast_create_parent_node(SPLT_ARG_LIST, 2, $1, $2); yyerrok; }
    | COMMA error { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expect an operand"); $$ = ast_create_parent_node(SPLT_ARG_LIST, 1, $1); yyerrok; }
    ;

/* String intermediate expression. Allowing concatenation of strings. */
string-literal: 
      STR_UNIT { $$ = ast_create_parent_node(SPLT_LTR_STR, 1, $1); }
    | string-literal STR_UNIT { $$ = ast_add_child($1, $2); }
    ;

identifier:
      ID { $$ = $1; }
    ;
%%

void yyerror(const char *s) {
    // suppressed
#ifdef DEBUG
    SPLC_FDIAG("%s at line %d", s, yylloc.first_line);
#endif
}
