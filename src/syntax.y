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
%token TYPE_INT TYPE_FLOAT TYPE_CHAR STR
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

%%
/* Entire translation unit */
translation-unit: 
      external-definition-list {
        root = $$ = create_parent_node(SPLT_TRANS_UNIT, 1, $1);
      }
    | { root = $$ = NULL; }
    ;

/* External definition list: Recursive definition */
external-definition-list: 
      external-definition { $$ = create_parent_node(SPLT_EXT_DEF_LIST, 1, $1); }
    | external-definition-list external-definition { $$ = create_parent_node(SPLT_EXT_DEF_LIST, 2, $1, $2); }
    ;

/* External definition list: A single unit of one of {variables, structs, functions}. */
external-definition: 
      SEMI { $$ = create_parent_node(SPLT_EXT_DEF, 1, $1); }
    | type-specifier external-declaration-list SEMI { $$ = create_parent_node(SPLT_EXT_DEF, 3, $1, $2, $3); }
    | type-specifier type-specifier external-declaration-list SEMI { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@2), "two or more data types in declaration specifiers"); $$ = create_parent_node(SPLT_FUNC_DEC, 0); yyerrok; }
    | external-declaration-list SEMI { splcwarn(SPLC_YY2LOC_CF_1_PNT_F(@1), "declaration is missing a specifier and will default to int"); $$ = create_parent_node(SPLT_EXT_DEF, 0); yyerrok; }
    
    | type-specifier SEMI { $$ = create_parent_node(SPLT_EXT_DEF, 2, $1, $2); } // Allowing structure definitions
    
    | function-declarator compound-statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "function is missing a specifier"); $$ = create_parent_node(SPLT_EXT_DEF, 0); yyerrok; } 
    | type-specifier function-declarator compound-statement { $$ = create_parent_node(SPLT_EXT_DEF, 3, $1, $2, $3); }
    | type-specifier function-declarator SEMI { $$ = create_parent_node(SPLT_EXT_DEF, 3, $1, $2, $3); }
    
    | type-specifier error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@2), "missing valid identifier"); $$ = create_parent_node(SPLT_EXT_DEF, 0); yyerrok; }
    ;

/* External declaration list: Multiple variable definition of a single type. */
external-declaration-list: 
      variable-declarator { $$ = create_parent_node(SPLT_EXT_DEC_LIST, 1, $1); }
    | external-declaration-list COMMA variable-declarator { $$ = create_parent_node(SPLT_EXT_DEC_LIST, 3, $1, $2, $3); }
    ;

type-specifier: 
      type-name { $$ = create_parent_node(SPLT_TYPE_SPEC, 1, $1); }
    /* | identifier { $$ = create_parent_node(SPLT_TYPE_SPEC, 1, $1); } */
    | struct-specifier { $$ = create_parent_node(SPLT_TYPE_SPEC, 1, $1); }
    ;

type-name:
      TYPE_INT
    | TYPE_FLOAT
    | TYPE_CHAR
    ;

/* Specify a structure */
struct-specifier: 
      STRUCT identifier LC definition-list RC { $$ = create_parent_node(SPLT_STRUCT_SPECIFIER, 5, $1, $2, $3, $4, $5); }
    | STRUCT identifier { $$ = create_parent_node(SPLT_STRUCT_SPECIFIER, 2, $1, $2); }
    ;

/* Single variable declaration */
variable-declarator: 
      pointer direct-declarator { $$ = create_parent_node(SPLT_DIR_DEC, 2, $1, $2); }
    | direct-declarator { $$ = create_parent_node(SPLT_DIR_DEC, 1, $1); }
    ;

direct-declarator:
      identifier { $$ = create_parent_node(SPLT_VAR_DEC, 1, $1); }
    | direct-declarator LSB LTR_INT RSB { $$ = create_parent_node(SPLT_VAR_DEC, 4, $1, $2, $3, $4); }
    | direct-declarator LSB RSB { $$ = create_parent_node(SPLT_VAR_DEC, 3, $1, $2, $3); }
    | direct-declarator LSB LTR_INT error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing closing bracket ']'"); $$ = create_parent_node(SPLT_FUNC_DEC, 0); yyerrok; } 
    | direct-declarator LTR_INT RSB { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '[' here"); $$ = create_parent_node(SPLT_FUNC_DEC, 0); yyerrok; } 
    | direct-declarator RSB { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '[' here"); $$ = create_parent_node(SPLT_FUNC_DEC, 0); yyerrok; } 
    ;

pointer:
      ASTRK { $$ = create_parent_node(SPLT_PTR, 1, $1); }
    | pointer ASTRK { $$ = create_parent_node(SPLT_PTR, 2, $1, $2); }
    ;

/* Function: Function name and body. */
function-declarator: 
      direct-function-declarator { $$ = create_parent_node(SPLT_FUNC_DEC, 1, $1); }
    | pointer direct-function-declarator { $$ = create_parent_node(SPLT_FUNC_DEC, 2, $1, $2); }
    ;

direct-function-declarator:
      identifier LP variable-list RP { $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 4, $1, $2, $3, $4); }
    | identifier LP error RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "invalid parameter declaration ')'"); $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 0); yyerrok; }
    | identifier error RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 0); yyerrok; }
    | identifier LP variable-list error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 0); yyerrok; }
    | identifier LP RP { $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 3, $1, $2, $3); }
    | identifier LP error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_DIR_FUNC_DEC, 0); yyerrok; }
    ;


/* List of variables names */
variable-list: 
      parameter-declaration { $$ = create_parent_node(SPLT_VAR_LIST, 1, $1); }
    | variable-list COMMA parameter-declaration { $$ = create_parent_node(SPLT_VAR_LIST, 3, $1, $2, $3); }
    ;

/* Parameter declaration */ 
parameter-declaration: 
      type-specifier variable-declarator { $$ = create_parent_node(SPLT_PARAM_DEC, 2, $1, $2); }
    ;

/* Compound statement: A new scope. */
compound-statement: 
      LC definition-list statement-list RC { $$ = create_parent_node(SPLT_COMP_STMT, 4, $1, $2, $3, $4); }
    | LC definition-list RC { $$ = create_parent_node(SPLT_COMP_STMT, 3, $1, $2, $3); }
    | LC statement-list RC { $$ = create_parent_node(SPLT_COMP_STMT, 3, $1, $2, $3); }
    | LC RC { $$ = create_parent_node(SPLT_COMP_STMT, 0); }

    | LC definition-list statement-list error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing brace '}'"); $$ = create_parent_node(SPLT_COMP_STMT, 0); yyerrok; }
    | LC definition-list statement-list definition-list error RC { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "cannot interleave definitions with statements. "); $$ = create_parent_node(SPLT_COMP_STMT, 0); }
    | LC statement-list definition-list statement-list error RC { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "cannot interleave definitions with statements. "); $$ = create_parent_node(SPLT_COMP_STMT, 0); }
    ;

/* Statement: List of statements. Recursive definition. */
statement-list: 
      statement { $$ = create_parent_node(SPLT_STMT_LIST, 1, $1); }
    | statement-list statement { $$ = create_parent_node(SPLT_STMT_LIST, 2, $1, $2); } 
    ;

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
    | expression error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@1), "missing semicolon ';'"); $$ = create_parent_node(SPLT_EXPR_STMT, 0); yyerrok; }
    ;

selection-statement:
      IF LP expression RP statement %prec THEN { $$ = create_parent_node(SPLT_SEL_STMT, 5, $1, $2, $3, $4, $5); }
    | IF error RP statement %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(SPLT_SEL_STMT, 0); yyerrok; }
    | IF LP expression RP error %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@4), "if requires at least one statement to be executed"); $$ = create_parent_node(SPLT_SEL_STMT, 0); yyerrok; }
    
    | IF LP expression RP statement ELSE statement %prec ELSE { $$ = create_parent_node(SPLT_SEL_STMT, 7, $1, $2, $3, $4, $5, $6, $7); }
    | IF error RP statement ELSE statement %prec ELSE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(SPLT_SEL_STMT, 0); yyerrok; }
    | IF LP expression RP statement ELSE error %prec ELSE { $$ = create_parent_node(SPLT_SEL_STMT, 7, $1, $2, $3, $4, $5, $6, $7); }
    | IF LP expression error %prec ELSE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_SEL_STMT, 0); yyerrok; }
    | ELSE statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "dangling else is not allowed."); $$ = create_parent_node(SPLT_SEL_STMT, 2, $1, $2); yyerrok; }
    
    | SWITCH LP expression RP statement {}
    | SWITCH error RP statement {}
    ;

labeled-statement:
      identifier COLON statement { $$ = create_parent_node(SPLT_LABELED_STMT, 3, $1, $2, $3); }
    | CASE constant-expression COLON statement { $$ = create_parent_node(SPLT_LABELED_STMT, 3, $1, $2, $3); }
    | DEFAULT COLON statement { $$ = create_parent_node(SPLT_LABELED_STMT, 3, $1, $2, $3); }
    | COLON statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing a label"); $$ = create_parent_node(SPLT_SEL_STMT, 0); yyerrok; }
    ;

jump-statement:
      GOTO identifier SEMI { $$ = create_parent_node(SPLT_JUMP_STMT, 3, $1, $2, $3); }
    | CONTINUE SEMI { $$ = create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); }
    | BREAK SEMI { $$ = create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); }
    | RETURN expression SEMI { $$ = create_parent_node(SPLT_JUMP_STMT, 3, $1, $2, $3); }
    | RETURN expression error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing semicolon ';'"); $$ = create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); yyerrok; }
    | RETURN SEMI { $$ = create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); }
    | RETURN error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@1), "missing expression in return statement"); $$ = create_parent_node(SPLT_JUMP_STMT, 2, $1, $2); yyerrok; }
    ;

iteration-statement:
      WHILE LP expression RP statement { $$ = create_parent_node(SPLT_ITER_STMT, 5, $1, $2, $3, $4, $5); }
    | WHILE error RP statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    | WHILE LP expression RP error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@4), "while loop requires at least one statement to be executed"); $$ = create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    | WHILE LP expression error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_ITER_STMT, 0); yyerrok;  }
    
    | DO WHILE LP expression RP SEMI { $$ = create_parent_node(SPLT_ITER_STMT, 6, $1, $2, $3, $4, $5, $6); }

    | FOR LP for-loop-body RP statement { $$ = create_parent_node(SPLT_ITER_STMT, 5, $1, $2, $3, $4, $5); }
    | FOR LP for-loop-body RP error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@4), "for loop requires at least one statement to be executed"); $$ = create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    | FOR LP for-loop-body error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = create_parent_node(SPLT_ITER_STMT, 0); yyerrok; }
    ;

for-loop-body: 
      definition expression SEMI expression { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }
    | expression SEMI expression SEMI expression { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 5, $1, $2, $3, $4, $5); }

    | SEMI expression SEMI expression { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }
    | definition expression SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    | expression SEMI expression SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }
    | definition SEMI expression { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    | expression SEMI SEMI expression { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }

    | SEMI expression SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    | SEMI SEMI expression { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    | definition SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 2, $1, $2); }
    | expression SEMI SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 3, $1, $2, $3); }
    
    | SEMI SEMI { $$ = create_parent_node(SPLT_FOR_LOOP_BODY, 2, $1, $2); }
    ;

/* Definition: List of definitions. Recursive definition. */
definition-list: 
      definition { $$ = create_parent_node(SPLT_DEF_LIST, 1, $1); }
    | definition-list definition { $$ = create_parent_node(SPLT_DEF_LIST, 2, $1, $2); }
    ;

/* Definition: Base */
definition: 
      type-specifier declaration-list SEMI { $$ = create_parent_node(SPLT_DEF, 3, $1, $2, $3); }
    | type-specifier declaration-list error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing semicolon ';'"); $$ = create_parent_node(SPLT_DEF, 0); yyerrok; }
    ;

/* Definition: Declaration of multiple variable.  */ 
declaration-list: 
      declarator { $$ = create_parent_node(SPLT_DEC_LIST, 1, $1); }
    | declaration-list COMMA declarator { $$ = create_parent_node(SPLT_DEC_LIST, 3, $1, $2, $3); }
    ;

/* Definition: Single declaration unit. */
declarator: 
      variable-declarator { $$ = create_parent_node(SPLT_DEC, 1, $1); }
    | variable-declarator LP RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_2_PNT_FL(@1, @3), "function definition not allowed here."); $$ = create_parent_node(SPLT_DEC, 0); yyerrok; } 
    | variable-declarator LP expression RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_2_PNT_FL(@1, @4), "function definition not allowed here."); $$ = create_parent_node(SPLT_DEC, 0); yyerrok; } 
    | variable-declarator ASSIGN expression { $$ = create_parent_node(SPLT_DEC, 3, $1, $2, $3); }
    | variable-declarator ASSIGN error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "invalid initialization"); $$ = create_parent_node(SPLT_DEC, 0); yyerrok; }
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
    ;

postfix-expression:
      primary-expression
    | postfix-expression LSB expression RSB { $$ = create_parent_node(SPLT_EXPR, 4, $1, $2, $3, $4); }
    | postfix-expression LP argument-list RP { $$ = create_parent_node(SPLT_EXPR, 4, $1, $2, $3, $4); }
    | postfix-expression LP RP { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | postfix-expression DOT identifier { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | postfix-expression RARROW identifier { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | postfix-expression DPLUS { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | postfix-expression DMINUS { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
    ;

unary-expression:
      postfix-expression
    | DPLUS unary-expression { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | DMINUS unary-expression { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | unary-operator cast-expression %prec UPLUS { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
    | SIZEOF unary-expression { $$ = create_parent_node(SPLT_EXPR, 2, $1, $2); }
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
    ;

multiplicative-expression:
      cast-expression
    | multiplicative-expression ASTRK cast-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | multiplicative-expression DIV cast-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | multiplicative-expression MOD cast-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

additive-expression:
      multiplicative-expression
    | additive-expression PLUS multiplicative-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | additive-expression MINUS multiplicative-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

shift-expression:
      additive-expression
    | shift-expression LSHIFT additive-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | shift-expression RSHIFT additive-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

relational-expression:
      shift-expression
    | relational-expression LT shift-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | relational-expression GT shift-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | relational-expression LE shift-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | relational-expression GE shift-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

equality-expression:
      relational-expression
    | equality-expression EQ relational-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    | equality-expression NE relational-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

BW-AND-expression:
      equality-expression
    | BW-AND-expression BW_AND equality-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

BW-XOR-expression:
      BW-AND-expression
    | BW-XOR-expression BW_XOR BW-AND-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

BW-OR-expression:
      BW-XOR-expression
    | BW-OR-expression BW_OR BW-XOR-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

logical-AND-expression:
      BW-OR-expression
    | logical-AND-expression AND BW-OR-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

logical-OR-expression:
      logical-AND-expression
    | logical-OR-expression OR logical-AND-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

conditional-expression:
      logical-OR-expression
    | logical-OR-expression QM expression COLON conditional-expression { $$ = create_parent_node(SPLT_EXPR, 5, $1, $2, $3, $4, $5); }
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

assignment-expression:
      conditional-expression
    | unary-expression assignment-operator assignment-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

/* expressions */
expression: 
      assignment-expression
    | expression COMMA assignment-expression { $$ = create_parent_node(SPLT_EXPR, 3, $1, $2, $3); }
    ;

/* Argument: List of arguments */
argument-list: 
      argument-list COMMA assignment-expression { $$ = create_parent_node(SPLT_ARG_LIST, 3, $1, $2, $3); }
    | assignment-expression { $$ = create_parent_node(SPLT_ARG_LIST, 1, $1); }
    ;

/* String intermediate expression. Allowing concatenation of strings. */
string-literal: 
      STR { $$ = create_parent_node(SPLT_LTR_STR, 1, $1); }
    | string-literal STR { $$ = create_parent_node(SPLT_LTR_STR, 2, $1, $2); }
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
