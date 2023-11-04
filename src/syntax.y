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

%token INT FLOAT CHAR STR
%token STRUCT ID TYPE
%token IF ELSE WHILE FOR
%token RETURN
%token SEMI COMMA 
%token ASSIGN 
%token AND OR BW_AND BW_OR
%token LT LE GT GE NE EQ PLUS MINUS MUL DIV
%token LC RC LP RP NOT DOT LSB RSB

%precedence THEN
%precedence ELSE

%right ASSIGN
%left BW_OR
%left BW_AND
%left OR
%left AND
%left LT LE GT GE NE EQ 
%left PLUS MINUS
%left MUL DIV
%right UPLUS UMINUS
%right NOT PRE_PLUS PRE_MINUS
%left LP RP LSB RSB DOT POST_PLUS POST_MINUS

%%
/* Entire program */
translation-unit: 
      external-definition-list {
        root = $$ = create_parent_node(AST_PROGRAM, 1, $1);
      }
    | { root = $$ = NULL; }
    ;

/* External definition list: Recursive definition */
external-definition-list: 
      external-definition { $$ = create_parent_node(AST_EXT_DEF_LIST, 1, $1); }
    | external-definition-list external-definition { $$ = create_parent_node(AST_EXT_DEF_LIST, 2, $1, $2); }
    ;

/* External definition list: A single unit of one of {variables, structs, functions}. */
external-definition: 
      SEMI { $$ = create_parent_node(AST_EXT_DEF, 1, $1); }
    | specifier external-declaration-list SEMI { $$ = create_parent_node(AST_EXT_DEF, 3, $1, $2, $3); }
    | specifier specifier external-declaration-list SEMI { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@2), "two or more data types in declaration specifiers"); $$ = create_parent_node(AST_FUNC_DEC, 0); yyerrok; }
    | external-declaration-list SEMI { splcwarn(SPLC_YY2LOC_CF_1_PNT_F(@1), "declaration is missing a specifier and will default to int"); $$ = create_parent_node(AST_EXT_DEF, 0); yyerrok; }
    | specifier SEMI { $$ = create_parent_node(AST_EXT_DEF, 2, $1, $2); } // Allowing structure definitions
    | function-declarator compound-statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "function is missing a specifier"); $$ = create_parent_node(AST_EXT_DEF, 0); yyerrok; } 
    | specifier function-declarator compound-statement { $$ = create_parent_node(AST_EXT_DEF, 3, $1, $2, $3); }
    | specifier function-declarator SEMI { $$ = create_parent_node(AST_EXT_DEF, 3, $1, $2, $3); }
    | specifier error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@2), "missing valid identifier"); $$ = create_parent_node(AST_EXT_DEF, 0); yyerrok; }
    ;

/* External declaration list: Multiple variable definition of a single type. */
external-declaration-list: 
      variable-declarator { $$ = create_parent_node(AST_EXT_DEC_LIST, 1, $1); }
    | variable-declarator COMMA external-declaration-list { $$ = create_parent_node(AST_EXT_DEC_LIST, 3, $1, $2, $3); }
    ;

specifier: 
      TYPE { $$ = create_parent_node(AST_SPECIFIER, 1, $1); }
    | struct-specifier { $$ = create_parent_node(AST_SPECIFIER, 1, $1); }
    ;

/* Specify a structure */
struct-specifier: 
      STRUCT ID LC definition-list RC { $$ = create_parent_node(AST_STRUCT_SPECIFIER, 5, $1, $2, $3, $4, $5); }
    | STRUCT ID { $$ = create_parent_node(AST_STRUCT_SPECIFIER, 2, $1, $2); }
    ;

/* Single variable declaration */
variable-declarator: 
      ID { $$ = create_parent_node(AST_VAR_DEC, 1, $1); }
    | variable-declarator LSB INT RSB { $$ = create_parent_node(AST_VAR_DEC, 4, $1, $2, $3, $4); }
    | variable-declarator LSB INT error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing closing bracket ']'"); $$ = create_parent_node(AST_FUNC_DEC, 0); yyerrok; } 
    | variable-declarator INT RSB { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '[' here"); $$ = create_parent_node(AST_FUNC_DEC, 0); yyerrok; } 
    | variable-declarator RSB { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '[' here"); $$ = create_parent_node(AST_FUNC_DEC, 0); yyerrok; } 
    ;

/* Function: Function name and body. */
function-declarator: 
      ID LP variable-list RP { $$ = create_parent_node(AST_FUNC_DEC, 4, $1, $2, $3, $4); }
    | ID LP error RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "invalid parameter declaration ')'"); $$ = create_parent_node(AST_FUNC_DEC, 0); yyerrok; }
    | ID error RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(AST_FUNC_DEC, 0); yyerrok; }
    | ID LP variable-list error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = create_parent_node(AST_FUNC_DEC, 0); yyerrok; }
    | ID LP RP { $$ = create_parent_node(AST_FUNC_DEC, 3, $1, $2, $3); }
    | ID LP error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing closing parenthesis ')'"); $$ = create_parent_node(AST_FUNC_DEC, 0); yyerrok; }
    ;

/* List of variables names */
variable-list: 
      parameter-declaration COMMA variable-list { $$ = create_parent_node(AST_VAR_LIST, 3, $1, $2, $3); }
    | parameter-declaration { $$ = create_parent_node(AST_VAR_LIST, 1, $1); }
    ;

/* Parameter declaration */ 
parameter-declaration: 
      specifier variable-declarator { $$ = create_parent_node(AST_PARAM_DEC, 2, $1, $2); }
    ;

/* Compound statement: A new scope. */
compound-statement: 
      LC RC { $$ = create_parent_node(AST_COMP_STMT, 0); }
    | LC definition-list statement-list RC { $$ = create_parent_node(AST_COMP_STMT, 4, $1, $2, $3, $4); }
    | LC definition-list RC { $$ = create_parent_node(AST_COMP_STMT, 3, $1, $2, $3); }
    | LC statement-list RC { $$ = create_parent_node(AST_COMP_STMT, 3, $1, $2, $3); }
    | LC definition-list statement-list error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing brace '}'"); $$ = create_parent_node(AST_COMP_STMT, 0); yyerrok; }
    | LC definition-list statement-list definition-list error RC { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "cannot interleave definitions with statements. "); $$ = create_parent_node(AST_COMP_STMT, 0); }
    | LC statement-list definition-list statement-list error RC { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "cannot interleave definitions with statements. "); $$ = create_parent_node(AST_COMP_STMT, 0); }
    ;

/* Statement: List of statements. Recursive definition. */
statement-list: 
      statement { $$ = create_parent_node(AST_STMT_LIST, 1, $1); }
    | statement-list statement { $$ = create_parent_node(AST_STMT_LIST, 2, $1, $2); } 
    ;

/* Statement: A single statement. */
statement: 
      SEMI { $$ = create_parent_node(AST_STMT, 1, $1); }
    | expression SEMI { $$ = create_parent_node(AST_STMT, 2, $1, $2); }
    | expression error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@1), "missing semicolon ';'"); $$ = create_parent_node(AST_STMT, 0); yyerrok; }

    | compound-statement { $$ = create_parent_node(AST_STMT, 1, $1); }
    | return-statement { $$ = create_parent_node(AST_STMT, 1, $1); }
    | conditional-statement { $$ = create_parent_node(AST_STMT, 1, $1); }
    | iteration-statement { $$ = create_parent_node(AST_STMT, 1, $1); }
    ;

return-statement:
      RETURN expression SEMI { $$ = create_parent_node(AST_RETURN_STMT, 3, $1, $2, $3); }
    | RETURN expression error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing semicolon ';'"); $$ = create_parent_node(AST_RETURN_STMT, 2, $1, $2); yyerrok; }
    | RETURN SEMI { $$ = create_parent_node(AST_RETURN_STMT, 2, $1, $2); }
    | RETURN error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@1), "missing expression in return statement"); $$ = create_parent_node(AST_RETURN_STMT, 2, $1, $2); yyerrok; }
    ;

conditional-statement:
      IF LP expression RP statement %prec THEN { $$ = create_parent_node(AST_COND_STMT, 5, $1, $2, $3, $4, $5); }
    | IF error RP statement %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(AST_COND_STMT, 0); yyerrok; }
    | IF LP expression RP error %prec THEN { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@4), "if requires at least one statement to be executed"); $$ = create_parent_node(AST_COND_STMT, 0); yyerrok; }
    
    | IF LP expression RP statement ELSE statement %prec ELSE { $$ = create_parent_node(AST_COND_STMT, 7, $1, $2, $3, $4, $5, $6, $7); }
    | IF error RP statement ELSE statement %prec ELSE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(AST_COND_STMT, 0); yyerrok; }
    | IF LP expression RP statement ELSE error %prec ELSE { $$ = create_parent_node(AST_COND_STMT, 7, $1, $2, $3, $4, $5, $6, $7); }
    | IF LP expression error %prec ELSE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = create_parent_node(AST_COND_STMT, 0); yyerrok; }
    | ELSE statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "dangling else is not allowed."); $$ = create_parent_node(AST_COND_STMT, 2, $1, $2); yyerrok; }
    ;

iteration-statement:
      WHILE LP expression RP statement { $$ = create_parent_node(AST_ITER_STMT, 5, $1, $2, $3, $4, $5); }
    | WHILE error RP statement { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_F(@2), "expected '(' here"); $$ = create_parent_node(AST_ITER_STMT, 0); yyerrok; }
    | WHILE LP expression RP error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@4), "while loop requires at least one statement to be executed"); $$ = create_parent_node(AST_ITER_STMT, 0); yyerrok; }
    | WHILE LP expression error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = create_parent_node(AST_ITER_STMT, 0); yyerrok;  }

    | FOR LP for-loop-body RP statement { $$ = create_parent_node(AST_ITER_STMT, 5, $1, $2, $3, $4, $5); }
    | FOR LP for-loop-body RP error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@4), "for loop requires at least one statement to be executed"); $$ = create_parent_node(AST_ITER_STMT, 0); yyerrok; }
    | FOR LP for-loop-body error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = create_parent_node(AST_ITER_STMT, 0); yyerrok; }
    ;

for-loop-body: 
      definition expression SEMI expression { $$ = create_parent_node(AST_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }

    | SEMI expression SEMI expression { $$ = create_parent_node(AST_FOR_LOOP_BODY, 4, $1, $2, $3, $4); }
    | definition expression SEMI { $$ = create_parent_node(AST_FOR_LOOP_BODY, 3, $1, $2, $3); }
    | definition SEMI expression { $$ = create_parent_node(AST_FOR_LOOP_BODY, 3, $1, $2, $3); }

    | SEMI expression SEMI { $$ = create_parent_node(AST_FOR_LOOP_BODY, 3, $1, $2, $3); }
    | SEMI SEMI expression { $$ = create_parent_node(AST_FOR_LOOP_BODY, 3, $1, $2, $3); }
    | definition SEMI { $$ = create_parent_node(AST_FOR_LOOP_BODY, 2, $1, $2); }
    
    | SEMI SEMI { $$ = create_parent_node(AST_FOR_LOOP_BODY, 2, $1, $2); }
    ;

/* Definition: List of definitions. Recursive definition. */
definition-list: 
      definition { $$ = create_parent_node(AST_DEF_LIST, 1, $1); }
    | definition-list definition { $$ = create_parent_node(AST_DEF_LIST, 2, $1, $2); }
    ;

/* Definition: Base */
definition: 
      specifier declaration-list SEMI { $$ = create_parent_node(AST_DEF, 3, $1, $2, $3); }
    | specifier declaration-list error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing semicolon ';'"); $$ = create_parent_node(AST_DEF, 0); yyerrok; }
    ;

/* Definition: Declaration of multiple variable.  */ 
declaration-list: 
      declarator { $$ = create_parent_node(AST_DEC_LIST, 1, $1); }
    | declaration-list COMMA declarator { $$ = create_parent_node(AST_DEC_LIST, 3, $1, $2, $3); }
    ;

/* Definition: Single declaration unit. */
declarator: 
      variable-declarator { $$ = create_parent_node(AST_DEC, 1, $1); }
    | variable-declarator LP RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_2_PNT_FL(@1, @3), "function definition not allowed here."); $$ = create_parent_node(AST_DEC, 0); yyerrok; } 
    | variable-declarator LP expression RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_2_PNT_FL(@1, @4), "function definition not allowed here."); $$ = create_parent_node(AST_DEC, 0); yyerrok; } 
    | variable-declarator ASSIGN expression { $$ = create_parent_node(AST_DEC, 3, $1, $2, $3); }
    | variable-declarator ASSIGN error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "invalid initialization"); $$ = create_parent_node(AST_DEC, 0); yyerrok; }
    ;

/* Expression */
expression: 
      expression ASSIGN expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | ASSIGN expression {  splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '='"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | ASSIGN error {  splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '='"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }

    | expression AND expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression OR expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression BW_AND expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression BW_OR expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | AND expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '&&'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | OR expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '||'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | BW_AND expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '&'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | BW_OR expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '|'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | AND { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '&&'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | OR { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '||'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | BW_AND { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '&'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | BW_OR { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '|'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }

    | expression LT expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression LE expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression GT expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression GE expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression NE expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression EQ expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | LT expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '<'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | LE expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '<='"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | GT expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '>'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | GE expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '>='"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | NE expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '!='"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | EQ expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '=='"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | LT { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '<'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | LE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '<='"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | GT { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '>'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | GE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '>='"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | NE { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '!='"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | EQ { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '=='"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }

    | expression PLUS expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression MINUS expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression PLUS error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@2), "expected expression after '+'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | expression MINUS error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@2), "expected expression after '-'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }

    | expression MUL expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | expression DIV expression { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | MUL expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '*'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | DIV expression { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression before '/'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | MUL error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '*'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | DIV error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@1), "expected expression after '/'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }

    | LP expression RP { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | LP expression error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@2), "missing closing parenthesis ')'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | LP error RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@2), "invalid expression"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }

    | expression PLUS PLUS %prec POST_PLUS { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | PLUS expression %prec UPLUS { $$ = create_parent_node(AST_EXP, 2, $1, $2); }
    | expression MINUS MINUS %prec POST_MINUS { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | MINUS expression %prec UMINUS { $$ = create_parent_node(AST_EXP, 2, $1, $2); }
    | NOT expression { $$ = create_parent_node(AST_EXP, 2, $1, $2); }
    | ID LP RP { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }
    | ID LP argument-list RP { $$ = create_parent_node(AST_EXP, 4, $1, $2, $3, $4); }
    | ID LP argument-list error { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_L(@3), "missing closing parenthesis ')'"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }
    | ID LP error RP { splcerror(SPLC_ERR_B, SPLC_YY2LOC_CF_1_PNT_I(@3), "invalid argument list"); $$ = create_parent_node(AST_EXP, 0); yyerrok; }

    | expression LSB expression RSB { $$ = create_parent_node(AST_EXP, 4, $1, $2, $3, $4); }
    | expression DOT ID { $$ = create_parent_node(AST_EXP, 3, $1, $2, $3); }

    | ID { $$ = create_parent_node(AST_EXP, 1, $1); }
    | INT { $$ = create_parent_node(AST_EXP, 1, $1); }
    | FLOAT { $$ = create_parent_node(AST_EXP, 1, $1); }
    | CHAR { $$ = create_parent_node(AST_EXP, 1, $1); }
    | string-literal { $$ = create_parent_node(AST_EXP, 1, $1); }
    ;
    
/* Argument: List of arguments */
argument-list: 
      argument-list COMMA expression { $$ = create_parent_node(AST_ARGS, 3, $1, $2, $3); }
    | expression { $$ = create_parent_node(AST_ARGS, 1, $1); }
    ;

/* String intermediate expression. Allowing concatenation of strings. */
string-literal: 
      STR { $$ = create_parent_node(AST_STREXP, 1, $1); }
    | STR string-literal { $$ = create_parent_node(AST_STREXP, 2, $1, $2); }
    ;

%%

void yyerror(const char *s) {
    // suppressed
    /* fprintf(stderr, "%s at line %d\n", s, yylloc.first_line); */
}
