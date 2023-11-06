%code requires {
    #include "splcdef.h"
    #include "lut.h"
    #include "ast.h"
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
%token TYPE_VOID TYPE_INT TYPE_FLOAT TYPE_CHAR STR
%token STRUCT UNION ID TYPE 
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

%precedence COMP_STMT_RC
%precedence COMP_STMT_LC

/* %destructor { destruct_node($$); } <> */

%%
/* Entire translation unit */
translation-unit: 
      external-declaration-list {
        root = $$ = create_parent_node(SPLT_TRANS_UNIT, 1, $1);
      }
    | { root = $$ = NULL; }
    ;

/* External definition list: Recursive definition */
external-declaration-list: 
      external-declaration { $$ = create_parent_node(SPLT_EXT_DECLTN_LIST, 1, $1); }
    | external-declaration-list external-declaration { $$ = add_child($1, $2); }
    ;

/* External definition list: A single unit of one of {variables, structs, functions}. */
external-declaration: 
      SEMI { $$ = create_parent_node(SPLT_EXT_DECLTN, 1, $1); }
    | declaration { $$ = create_parent_node(SPLT_EXT_DECLTN, 1, $1); }
    
    | type-specifier SEMI { $$ = create_parent_node(SPLT_EXT_DECLTN, 2, $1, $2); } // Allowing structure definitions

    | type-specifier function-declarator compound-statement { $$ = create_parent_node(SPLT_EXT_DECLTN, 3, $1, $2, $3); }
    | type-specifier function-declarator SEMI { $$ = create_parent_node(SPLT_EXT_DECLTN, 3, $1, $2, $3); }

    | function-declarator compound-statement { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "function is missing a specifier"); $$ = create_parent_node(SPLT_EXT_DECLTN, 0); yyerrok; } 
    | type-specifier type-specifier function-declarator compound-statement { splcerror(SPLC_ERR_B, $2->location, "function has mutliple specifiers"); $$ = create_parent_node(SPLT_EXT_DECLTN, 0); yyerrok; }

    | type-specifier type-specifier error { splcerror(SPLC_ERR_B, $2->location, "two or more data types in declaration specifiers"); $$ = create_parent_node(SPLT_EXT_DECLTN, 0); yyerrok; }
    | type-specifier type-specifier function-declarator error { splcerror(SPLC_ERR_B, $2->location, "function has mutliple specifiers"); $$ = create_parent_node(SPLT_EXT_DECLTN, 0); yyerrok; }
    | type-specifier function-declarator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($3), "missing valid function body. Did you forget to put '{'?"); $$ = create_parent_node(SPLT_EXT_DECLTN, 0); yyerrok; }
    ;

type-specifier: 
      builtin-type-specifier { $$ = create_parent_node(SPLT_TYPE_SPEC, 1, $1); }
    /* | identifier { $$ = create_parent_node(SPLT_TYPE_SPEC, 1, $1); } */
    | struct-specifier { $$ = create_parent_node(SPLT_TYPE_SPEC, 1, $1); }
    ;
      
type-name:
      builtin-type-specifier { $$ = create_parent_node(SPLT_TYPENAME, 1, $1); }
    | builtin-type-specifier abstract-declarator { $$ = create_parent_node(SPLT_TYPENAME, 2, $1, $2); }
    ;

builtin-type-specifier:
      TYPE_VOID
    | TYPE_INT
    | TYPE_FLOAT
    | TYPE_CHAR
    ;

abstract-declarator:
      pointer { $$ = create_parent_node(SPLT_ABS_DEC, 1, $1); }
    | pointer direct-abstract-declarator { $$ = create_parent_node(SPLT_ABS_DEC, 2, $1, $2); }
    ;

direct-abstract-declarator:
      LP abstract-declarator RP { $$ = create_parent_node(SPLT_DIR_ABS_DEC, 3, $1, $2, $3); }
    | direct-abstract-declarator LSB assignment-expression RSB { $$ = create_parent_node(SPLT_DIR_ABS_DEC, 4, $1, $2, $3, $4); }
    | direct-abstract-declarator LSB RSB { $$ = create_parent_node(SPLT_DIR_ABS_DEC, 3, $1, $2, $3); }
    | direct-abstract-declarator LSB error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected ']' here"); $$ = create_parent_node(SPLT_DIR_ABS_DEC, 2, $1, $2); yyerrok; }
    | direct-abstract-declarator RSB { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($2), "expected '[' here"); $$ = create_parent_node(SPLT_DIR_ABS_DEC, 2, $1, $2); yyerrok; } 
    ;

/* Specify a structure */
struct-specifier: 
      STRUCT identifier LC declaration-list RC { $$ = create_parent_node(SPLT_STRUCT_SPECIFIER, 5, $1, $2, $3, $4, $5); }
    | STRUCT identifier { $$ = create_parent_node(SPLT_STRUCT_SPECIFIER, 2, $1, $2); }
    ;

/* Single variable declaration */
declarator: 
      pointer direct-declarator { $$ = create_parent_node(SPLT_DEC, 2, $1, $2); }
    | direct-declarator { $$ = create_parent_node(SPLT_DEC, 1, $1); }
    ;

direct-declarator:
      identifier { $$ = create_parent_node(SPLT_DIR_DEC, 1, $1); }
    | direct-declarator LSB assignment-expression RSB { $$ = create_parent_node(SPLT_DIR_DEC, 4, $1, $2, $3, $4); }
    | direct-declarator LSB RSB { $$ = create_parent_node(SPLT_DIR_DEC, 3, $1, $2, $3); }

    | direct-declarator LSB assignment-expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "missing closing bracket ']'"); $$ = create_parent_node(SPLT_DIR_DEC, 0); yyerrok; } 
    /* | direct-declarator error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '[' here"); $$ = create_parent_node(SPLT_DIR_DEC, 0); yyerrok; }  */
    | direct-declarator RSB { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($2), "expected '[' here"); $$ = create_parent_node(SPLT_DIR_DEC, 0); yyerrok; } 
    ;

pointer:
      ASTRK { $$ = create_parent_node(SPLT_PTR, 1, $1); }
    | pointer ASTRK { $$ = create_parent_node(SPLT_PTR, 2, $1, $2); }
    ;

/* Definition: List of definitions. Recursive definition. */
declaration-list: 
      declaration { $$ = create_parent_node(SPLT_DECLTN_LIST, 1, $1); }
    | declaration-list declaration { $$ = add_child($1, $2); }
    ;

/* Definition: Base */
declaration: 
      type-specifier init-declarator-list SEMI { $$ = add_child($1, $2); }
    | type-specifier init-declarator-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "missing semicolon ';'"); $$ = add_child($1, $2); yyerrok; }
    ;

direct-declaration:
      type-specifier init-declarator-list { $$ = create_parent_node(SPLT_DECLTN, 2, $1, $2); }
    ;

/* Definition: Declaration of multiple variable.  */ 
init-declarator-list: 
      init-declarator { $$ = create_parent_node(SPLT_INIT_DEC_LIST, 1, $1); }
    | init-declarator COMMA init-declarator-list { $$ = create_parent_node(SPLT_INIT_DEC_LIST, 3, $1, $2, $3); }

    | init-declarator COMMA { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expected declarator"); $$ = create_parent_node(SPLT_INIT_DEC_LIST, 2, $1, $2); yyerrok; }
    | COMMA init-declarator-list { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected declarator"); $$ = create_parent_node(SPLT_INIT_DEC_LIST, 2, $1, $2); yyerrok; }
    | COMMA { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expected declarator"); $$ = create_parent_node(SPLT_INIT_DEC_LIST, 1, $1); yyerrok; }
    ;

/* Definition: Single declaration unit. */
init-declarator: 
      declarator { $$ = create_parent_node(SPLT_INIT_DEC, 1, $1); }
    | declarator ASSIGN assignment-expression { $$ = create_parent_node(SPLT_INIT_DEC, 3, $1, $2, $3); }
    | declarator ASSIGN error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "invalid initialization"); $$ = create_parent_node(SPLT_INIT_DEC, 0); yyerrok; }
    ;

/* Function: Function name and body. */
function-declarator: 
      direct-function-declarator { $$ = create_parent_node(SPLT_FUNC_DEC, 1, $1); }
    | pointer direct-function-declarator { $$ = create_parent_node(SPLT_FUNC_DEC, 2, $1, $2); }
    ;

direct-function-declarator:
      direct-declarator LP variable-list RP { $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 4, $1, $2, $3, $4); }
    | direct-declarator LP RP { $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 3, $1, $2, $3); }

    | direct-declarator LP error RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "invalid parameter declaration ')'"); $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 0); yyerrok; }
    | direct-declarator LP variable-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 0); yyerrok; }
    | direct-declarator LP error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 0); yyerrok; }

    | LP variable-list RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing identifier for function"); $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 3, $1, $2, $3); yyerrok; }
    | LP RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing identifier for function"); $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 2, $1, $2); yyerrok; }
    ;


/* List of variables names */
variable-list: 
      parameter-declaration { $$ = create_parent_node(SPLT_VAR_LIST, 1, $1); }
    | variable-list COMMA parameter-declaration { $$ = create_parent_node(SPLT_VAR_LIST, 3, $1, $2, $3); }

    | variable-list COMMA { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expected parameter delcaration"); $$ = create_parent_node(SPLT_PARAM_DEC, 2, $1, $2); yyerrok; }
    | COMMA { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expected parameter delcaration"); $$ = create_parent_node(SPLT_PARAM_DEC, 1, $1); yyerrok; }
    ;

/* Parameter declaration */ 
parameter-declaration: 
      type-specifier declarator { $$ = create_parent_node(SPLT_PARAM_DEC, 2, $1, $2); }

    | declarator { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing specifier"); $$ = create_parent_node(SPLT_PARAM_DEC, 1, $1); yyerrok; }
    | type-specifier { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@1), "missing specifier"); $$ = create_parent_node(SPLT_PARAM_DEC, 1, $1); yyerrok; }
    ;

/* Compound statement: A new scope. */
compound-statement: 
      /* LC general-statement-list RC */
      LC general-statement-list RC { $$ = create_parent_node(SPLT_COMP_STMT, 3, $1, $2, $3); }
    | LC general-statement-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "missing closing bracket '}' "); $$ = create_parent_node(SPLT_COMP_STMT, 0); yyerrok; }
    /* | error RC { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing opening bracket '{' "); $$ = create_parent_node(SPLT_COMP_STMT, 0); yyerrok; } */

      /* LC RC */
    | LC RC { $$ = create_parent_node(SPLT_COMP_STMT, 2, $1, $2); }
    | LC error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "missing closing bracket '}' "); $$ = create_parent_node(SPLT_COMP_STMT, 0); yyerrok; }
    ;

/* wrapper for C99 standard for statements */
general-statement-list: 
      statement { $$ = create_parent_node(SPLT_GEN_STMT_LIST, 1, $1); }
    | declaration { $$ = create_parent_node(SPLT_GEN_STMT_LIST, 1, $1); }
    | general-statement-list statement { $$ = add_child($1, $2); }
    | general-statement-list declaration { $$ = add_child($1, $2); }
    ;

/* Statement: List of statements. Recursive definition. */
/* statement-list: 
      statement { $$ = create_parent_node(SPLT_STMT_LIST, 1, $1); }
    | statement-list statement { $$ = create_parent_node(SPLT_STMT_LIST, 2, $1, $2); } 
    ; */

/* Statement: A single statement. */
statement: 
      SEMI { $$ = create_parent_node(SPLT_STMT, 1, $1); }
    | compound-statement { $$ = create_parent_node(SPLT_STMT, 1, $1); }
    | expression-statement { $$ = create_parent_node(SPLT_STMT, 1, $1); }
    | selection-statement { $$ = create_parent_node(SPLT_STMT, 1, $1); }
    | iteration-statement { $$ = create_parent_node(SPLT_STMT, 1, $1); }
    | labeled-statement { $$ = create_parent_node(SPLT_STMT, 1, $1); }
    | jump-statement { $$ = create_parent_node(SPLT_STMT, 1, $1); }
    ;

expression-statement:
      expression SEMI { $$ = create_parent_node(SPLT_EXPR_STMT, 2, $1, $2); }
    | expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "missing semicolon ';'"); $$ = create_parent_node(SPLT_EXPR_STMT, 0); yyerrok; }
    ;

selection-statement:
      IF LP expression RP statement %prec THEN { $$ = create_parent_node(SPLT_SEL_STMT, 5, $1, $2, $3, $4, $5); }

    | IF error RP statement %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(SPLT_SEL_STMT, 3, $1, $3, $4); yyerrok; }
    | IF LP RP statement %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expected an expression"); $$ = create_parent_node(SPLT_SEL_STMT, 4, $1, $2, $3, $4); yyerrok; }
    | IF LP expression RP error %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@4), "if requires at least one statement to be executed"); $$ = create_parent_node(SPLT_SEL_STMT, 4, $1, $2, $3, $4); yyerrok; }
    | IF LP RP error %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expected an expression"); $$ = create_parent_node(SPLT_SEL_STMT, 3, $1, $2, $3); yyerrok; }
    
    | IF LP expression RP statement ELSE statement %prec ELSE { $$ = create_parent_node(SPLT_SEL_STMT, 7, $1, $2, $3, $4, $5, $6, $7); }

    | IF error RP statement ELSE statement %prec ELSE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(SPLT_SEL_STMT, 0); yyerrok; }
    | IF LP expression RP statement ELSE error %prec ELSE { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($6), "expected a statement"); $$ = create_parent_node(SPLT_SEL_STMT, 6, $1, $2, $3, $4, $5, $6); yyerrok; }
    | IF LP RP statement ELSE statement %prec ELSE { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an expression"); $$ = create_parent_node(SPLT_SEL_STMT, 6, $1, $2, $3, $4, $5, $6); yyerrok; }
    | IF LP RP statement ELSE error %prec ELSE { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expected expression and statement"); $$ = create_parent_node(SPLT_SEL_STMT, 5, $1, $2, $3, $4, $5); yyerrok; }
    | IF LP expression error %prec ELSE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_SEL_STMT, 0); yyerrok; }
    | ELSE statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "dangling else is not allowed"); $$ = create_parent_node(SPLT_SEL_STMT, 2, $1, $2); yyerrok; }

    | SWITCH LP expression RP statement { $$ = create_parent_node(SPLT_SEL_STMT, 4, $1, $2, $3, $4); }
    /* | SWITCH LP expression statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing opening parenthese '('"); $$ = create_parent_node(SPLT_SEL_STMT, 0); yyerrok; } */
    | SWITCH error RP statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "missing opening parenthese '('"); $$ = create_parent_node(SPLT_SEL_STMT, 0); yyerrok; }
    ;

labeled-statement:
      identifier COLON statement { $$ = create_parent_node(SPLT_LABELED_STMT, 3, $1, $2, $3); }
    | CASE constant-expression COLON statement { $$ = create_parent_node(SPLT_LABELED_STMT, 3, $1, $2, $3); }
    | DEFAULT COLON statement { $$ = create_parent_node(SPLT_LABELED_STMT, 3, $1, $2, $3); }

    | COLON statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing a label"); $$ = create_parent_node(SPLT_LABELED_STMT, 0); yyerrok; }
    ;

jump-statement:
      GOTO identifier SEMI { $$ = create_parent_node(SPLT_JUMP_STMT, 3, $1, $2, $3); }
    | CONTINUE SEMI { $$ = create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); }
    | BREAK SEMI { $$ = create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); }
    | RETURN expression SEMI { $$ = create_parent_node(SPLT_JUMP_STMT, 3, $1, $2, $3); }
    | RETURN expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "missing semicolon ';'"); $$ = create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); yyerrok; }
    | RETURN SEMI { $$ = create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); }

    | RETURN error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "missing expression in return statement"); $$ = create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); yyerrok; }
    ;

iteration-statement:
      WHILE LP expression RP statement { $$ = create_parent_node(SPLT_ITER_STMT, 5, $1, $2, $3, $4, $5); }
    | WHILE error RP statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    | WHILE LP expression RP error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($4), "while loop requires at least one statement to be executed"); $$ = create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    | WHILE LP expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_ITER_STMT, 0); yyerrok;  }
    
    | DO WHILE LP expression RP SEMI { $$ = create_parent_node(SPLT_ITER_STMT, 6, $1, $2, $3, $4, $5, $6); }

    | FOR LP for-loop-body RP statement { $$ = create_parent_node(SPLT_ITER_STMT, 5, $1, $2, $3, $4, $5); }
    | FOR LP for-loop-body RP error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($4), "for loop requires at least one statement to be executed"); $$ = create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    | FOR LP for-loop-body error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    ;

for-loop-body: 
      initialization-expression SEMI expression SEMI expression { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 5, $1, $2, $3, $4, $5); }

    | SEMI expression SEMI expression { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }
    | initialization-expression SEMI expression SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }
    | initialization-expression SEMI SEMI expression { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }

    | SEMI expression SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    | SEMI SEMI expression { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    /* | definition SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 2, $1, $2); } */
    | initialization-expression SEMI SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    
    | SEMI SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 2, $1, $2); }
    ;

constant-expression: 
      conditional-expression { $$ = create_parent_node(SPLT_CONST_EXPR, 1, $1); }
    ;

constant:
      LTR_INT { $$ = create_parent_node(SPLT_CONSTANT, 1, $1); }
    | LTR_FLOAT { $$ = create_parent_node(SPLT_CONSTANT, 1, $1); }
    | LTR_CHAR { $$ = create_parent_node(SPLT_CONSTANT, 1, $1); }
    ;

primary-expression:
      identifier
    | constant { $$ = create_parent_node(SPLT_EXPR, 1, $1); }
    | string-literal { $$ = create_parent_node(SPLT_EXPR, 1, $1); }
    | LP expression RP { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | LP expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected ')'"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    /* | LP expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; } */
    ;

postfix-expression:
      primary-expression
    | postfix-expression LSB expression RSB { $$ = create_parent_node(SPLT_EXPR, 4, $1, $2, $3, $4); }
    | postfix-expression LP argument-list RP { $$ = create_parent_node(SPLT_EXPR, 4, $1, $2, $3, $4); }
    | postfix-expression LP RP { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | postfix-expression member-access-operator identifier { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | postfix-expression DPLUS { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | postfix-expression DMINUS { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }

    | postfix-expression LSB expression error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expected ']'"); $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); yyerrok; }
    | postfix-expression LP argument-list error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expected ')'"); $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); yyerrok; }
    | postfix-expression member-access-operator { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expected an identifier"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | member-access-operator identifier { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

member-access-operator:
      DOT
    | RARROW
    ;

unary-expression:
      postfix-expression
    | DPLUS unary-expression { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | DMINUS unary-expression { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | unary-operator cast-expression %prec UPLUS { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | SIZEOF unary-expression { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | SIZEOF LP type-name RP { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | DPLUS error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 1, $1); yyerrok; }
    | DMINUS error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 1, $1); yyerrok; }
    | SIZEOF error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 1, $1); yyerrok; }
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
    | LP type-name RP cast-expression { $$ = create_parent_node(SPLT_CAST_EXPR, 4, $1, $2, $3, $4); }

    | LP type-name RP error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($3), "expected an expression"); $$ = create_parent_node(SPLT_CAST_EXPR, 3, $1, $2, $3); yyerrok; }
    | LP type-name error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected ')'"); $$ = create_parent_node(SPLT_CAST_EXPR, 2, $1, $2); yyerrok; }
    ;

multiplicative-expression:
      cast-expression
    | multiplicative-expression multiplicative-operator cast-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | multiplicative-expression multiplicative-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | division-operator cast-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
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
    | additive-expression additive-operator multiplicative-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | additive-expression additive-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

additive-operator:
      PLUS
    | MINUS
    ;

shift-expression:
      additive-expression
    | shift-expression shift-operator additive-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | shift-expression shift-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | shift-operator additive-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;
  
shift-operator:
      LSHIFT
    | RSHIFT
    ;

relational-expression:
      shift-expression
    | relational-expression relational-operator shift-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | relational-expression relational-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | relational-operator shift-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

relational-operator:
      LT
    | GT
    | LE
    | GE
    ;

equality-expression:
      relational-expression
    | equality-expression equality-operator relational-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | equality-expression equality-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | equality-operator relational-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

equality-operator:
      EQ
    | NE
    ;

BW-AND-expression:
      equality-expression
    | BW-AND-expression BW_AND equality-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | BW-AND-expression BW_AND error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | BW_AND error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

BW-XOR-expression:
      BW-AND-expression
    | BW-XOR-expression BW_XOR BW-AND-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | BW-XOR-expression BW_XOR error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | BW_XOR BW-AND-expression { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

BW-OR-expression:
      BW-XOR-expression
    | BW-OR-expression BW_OR BW-XOR-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | BW-OR-expression BW_OR error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | BW_OR BW-XOR-expression { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

logical-AND-expression:
      BW-OR-expression
    | logical-AND-expression AND BW-OR-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | logical-AND-expression AND error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | AND BW-OR-expression { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

logical-OR-expression:
      logical-AND-expression
    | logical-OR-expression OR logical-AND-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | logical-OR-expression OR error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | OR logical-AND-expression { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;

conditional-expression:
      logical-OR-expression
    | logical-OR-expression QM expression COLON conditional-expression { $$ = create_parent_node(SPLT_EXPR, 5, $1, $2, $3, $4, $5); }

    | logical-OR-expression QM COLON conditional-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "expected an expression"); $$ = create_parent_node(SPLT_EXPR, 4, $1, $2, $3, $4); yyerrok; }
    | logical-OR-expression QM expression COLON { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@4), "expected an expression"); $$ = create_parent_node(SPLT_EXPR, 4, $1, $2, $3, $4); yyerrok; }
    | QM error { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expected an expression"); $$ = create_parent_node(SPLT_EXPR, 1, $1); yyerrok; }
    ;

assignment-expression:
      conditional-expression
    | unary-expression assignment-operator assignment-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | unary-expression assignment-operator error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | assignment-operator assignment-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
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
      assignment-expression
    | expression COMMA assignment-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }

    | expression COMMA error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    | COMMA assignment-expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "expected an operand"); $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); yyerrok; }
    ;
  
initialization-expression:
      expression { $$ = create_parent_node(SPLT_INIT_EXPR, 1, $1); }
    | direct-declaration { $$ = create_parent_node(SPLT_INIT_EXPR, 1, $1); }
    ;

/* Argument: List of arguments */
argument-list: 
      argument-list COMMA assignment-expression { $$ = create_parent_node(SPLT_ARG_LIST, 3, $1, $2, $3); }
    | assignment-expression { $$ = create_parent_node(SPLT_ARG_LIST, 1, $1); }

    | argument-list COMMA error { splcerror(SPLC_ERR_B, SPLC_AST_GET_ENDLOC($2), "expected an operand"); $$ = create_parent_node(SPLT_ARG_LIST, 2, $1, $2); yyerrok; }
    | COMMA error { splcerror(SPLC_ERR_B, SPLC_AST_GET_STARTLOC($1), "expected an operand"); $$ = create_parent_node(SPLT_ARG_LIST, 1, $1); yyerrok; }
    ;

/* String intermediate expression. Allowing concatenation of strings. */
string-literal: 
      STR { $$ = create_parent_node(SPLT_LTR_STR, 1, $1); }
    | string-literal STR { $$ = add_child($1, $2); }
    ;

identifier:
      ID { $$ = $1; }
    ;
%%

void yyerror(const char *s) {
    // suppressed
#ifdef DEBUG
    fprintf(stderr, "%s at line %d\n", s, yylloc.first_line);
#endif
}
