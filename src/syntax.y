%code requires {
    #include "spldef.h"
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
%token IF THEN ELSE WHILE FOR
%token RETURN
%token SEMI COMMA 
%token ASSIGN 
%token AND OR BITWISE_AND BITWISE_OR
%token LT LE GT GE NE EQ PLUS MINUS MUL DIV
%token LC RC LP RP NOT DOT LSB RSB

%precedence THEN
%precedence ELSE

%right ASSIGN
%left BITWISE_OR
%left BITWISE_AND
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
Program: ExtDefList {
        root = $$ = create_parent_node(AST_PROGRAM, @$.first_line, 1, $1);
        if (!err_flag)
            print_ast($$);
      }
    ;

/* External definition list: Recursive definition */
ExtDefList: ExtDef ExtDefList { $$ = create_parent_node(AST_EXT_DEF_LIST, @$.first_line, 2, $1, $2); }
    /* | Stmt ExtDefList{ splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "statement is not allowed at global scope"); $$ = create_parent_node(AST_EXT_DEF_LIST, @$.first_line, 2, $1, $2); yyerrok; } */
    | { $$ = NULL; }
    ;

/* External definition list: A single unit of one of {variables, structs, functions}. */
ExtDef: Specifier ExtDecList SEMI { $$ = create_parent_node(AST_EXT_DEF, @$.first_line, 3, $1, $2, $3); }
    | Specifier Specifier ExtDecList SEMI { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.last_line, @2.last_column, "two or more data types in declaration specifiers"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; }
    | ExtDecList SEMI { splwarn(@1.first_line, @1.first_column, @1.first_line, @1.first_column, "declaration is missing a specifier and will default to int"); $$ = create_parent_node(AST_EXT_DEF, @$.first_line, 0); yyerrok; }
    | Specifier SEMI { $$ = create_parent_node(AST_EXT_DEF, @$.first_line, 2, $1, $2); } // Allowing structure definitions
    | FuncDec CompStmt { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.first_line, @1.first_column, "function is missing a specifier"); $$ = create_parent_node(AST_EXT_DEF, @$.first_line, 0); yyerrok; } 
    /* | FuncDec SEMI { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.first_line, @1.first_column, "function is missing a specifier"); $$ = create_parent_node(AST_EXT_DEF, @$.first_line, 0); yyerrok; }  */
    | Specifier FuncDec CompStmt { $$ = create_parent_node(AST_EXT_DEF, @$.first_line, 3, $1, $2, $3); }
    | Specifier FuncDec SEMI { $$ = create_parent_node(AST_EXT_DEF, @$.first_line, 3, $1, $2, $3); }
    /* | Specifier FuncDec error { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.last_line, @2.last_column, "invalid function body"); $$ = create_parent_node(AST_EXT_DEF, @$.first_line, 0); yyerrok; } */
    | Specifier error { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.last_line, @2.last_column, "missing valid identifier"); $$ = create_parent_node(AST_EXT_DEF, @$.first_line, 0); yyerrok; }
    ;

/* External declaration list: Multiple variable definition of a single type. */
ExtDecList: VarDec { $$ = create_parent_node(AST_EXT_DEC_LIST, @$.first_line, 1, $1); }
    | VarDec COMMA ExtDecList { $$ = create_parent_node(AST_EXT_DEC_LIST, @$.first_line, 3, $1, $2, $3); }
    ;

Specifier: TYPE { $$ = create_parent_node(AST_SPECIFIER, @$.first_line, 1, $1); }
    | StructSpecifier { $$ = create_parent_node(AST_SPECIFIER, @$.first_line, 1, $1); }
    ;

/* Specify a structure */
StructSpecifier: STRUCT ID LC DefList RC { $$ = create_parent_node(AST_STRUCT_SPECIFIER, @$.first_line, 5, $1, $2, $3, $4, $5); }
    | STRUCT ID { $$ = create_parent_node(AST_STRUCT_SPECIFIER, @$.first_line, 2, $1, $2); }
    ;

/* Single variable declaration */
VarDec: ID { $$ = create_parent_node(AST_VAR_DEC, @$.first_line, 1, $1); }
    /* | Specifier { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "two or more data types in declaration specifiers"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; } */
    | VarDec LSB INT RSB { $$ = create_parent_node(AST_VAR_DEC, @$.first_line, 4, $1, $2, $3, $4); }
    | VarDec LSB INT error { splerror(SPLC_ERR_B, @2.last_line, @2.last_column, @2.last_line, @2.last_column, "missing closing bracket ']'"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; } 
    | VarDec INT RSB { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.first_line, @2.first_column, "expected '[' here"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; } 
    | VarDec RSB { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.first_line, @2.first_column, "expected '[' here"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; } 
    /* | VarDec error { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.last_line, @2.last_column, "unrecognized following content"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; } */
    ;

/* Function: Function name and body. */
FuncDec: ID LP VarList RP { $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 4, $1, $2, $3, $4); }
    | ID LP error RP { splerror(SPLC_ERR_B, @3.first_line, @3.first_column, @3.last_line, @3.last_column, "invalid parameter declaration ')'"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; }
    | ID error RP { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.first_line, @2.first_column, "expected '(' here"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; }
    /* | ID LP VarList { splerror(SPLC_ERR_B, @3.last_line, @3.last_column, @3.last_line, @3.last_column, "missing closing parenthesis ')'"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; } */
    | ID LP VarList error { splerror(SPLC_ERR_B, @3.last_line, @3.last_column, @3.last_line, @3.last_column, "missing closing parenthesis ')'"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; }
    | ID LP RP { $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 3, $1, $2, $3); }
    | ID LP error { splerror(SPLC_ERR_B, @2.last_line, @2.last_column, @2.last_line, @2.last_column, "missing closing parenthesis ')'"); $$ = create_parent_node(AST_FUNC_DEC, @$.first_line, 0); yyerrok; }
    ;

/* List of variables names */
VarList: ParamDec COMMA VarList { $$ = create_parent_node(AST_VAR_LIST, @$.first_line, 3, $1, $2, $3); }
    | ParamDec { $$ = create_parent_node(AST_VAR_LIST, @$.first_line, 1, $1); }
    ;

/* Parameter declaration */ 
ParamDec: Specifier VarDec { $$ = create_parent_node(AST_PARAM_DEC, @$.first_line, 2, $1, $2); }
    ;

/* Compound statement: A new scope. */
CompStmt: LC RC { $$ = create_parent_node(AST_COMP_STMT, @$.first_line, 0); }
    | LC DefList StmtList RC { $$ = create_parent_node(AST_COMP_STMT, @$.first_line, 4, $1, $2, $3, $4); }
    | LC DefList RC { $$ = create_parent_node(AST_COMP_STMT, @$.first_line, 3, $1, $2, $3); }
    | LC StmtList RC { $$ = create_parent_node(AST_COMP_STMT, @$.first_line, 3, $1, $2, $3); }
    | LC DefList StmtList error { splerror(SPLC_ERR_B, @3.last_line, @3.last_column, @3.last_line, @3.last_column, "missing closing brace '}'"); $$ = create_parent_node(AST_COMP_STMT, @$.first_line, 0); yyerrok; }
    | LC DefList StmtList DefList error RC { splerror(SPLC_ERR_B, @3.first_line, @3.first_column, @3.last_line, @3.last_column, "cannot interleave definitions with statements. "); $$ = create_parent_node(AST_COMP_STMT, @$.first_line, 0); }
    | LC StmtList DefList StmtList error RC { splerror(SPLC_ERR_B, @3.first_line, @3.first_column, @3.last_line, @3.last_column, "cannot interleave definitions with statements. "); $$ = create_parent_node(AST_COMP_STMT, @$.first_line, 0); }
    ;

/* Statement: List of statements. Recursive definition. */
StmtList: Stmt StmtList { $$ = create_parent_node(AST_STMT_LIST, @$.first_line, 2, $1, $2); }
    | Stmt { $$ = create_parent_node(AST_STMT_LIST, @$.first_line, 1, $1); }
    ;

/* Statement: A single statement. */
Stmt: SEMI { $$ = create_parent_node(AST_STMT, @$.first_line, 1, $1); }
    | Exp SEMI { $$ = create_parent_node(AST_STMT, @$.first_line, 2, $1, $2); }
    | Exp error { splerror(SPLC_ERR_B, @1.last_line, @1.last_column, @1.last_line, @1.last_column, "missing semicolon ';'"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok; }

    | CompStmt { $$ = create_parent_node(AST_STMT, @$.first_line, 1, $1); }

    | RETURN Exp SEMI { $$ = create_parent_node(AST_STMT, @$.first_line, 3, $1, $2, $3); }
    | RETURN Exp error { splerror(SPLC_ERR_B, @2.last_line, @2.last_column, @2.last_line, @2.last_column, "missing semicolon ';'"); $$ = create_parent_node(AST_STMT, @$.first_line, 2, $1, $2); yyerrok; }

    | IF LP Exp RP Stmt %prec THEN { $$ = create_parent_node(AST_STMT, @$.first_line, 5, $1, $2, $3, $4, $5); }
    | IF error RP Stmt %prec THEN { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.first_line, @2.first_column, "expected '(' here"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok; }
    | IF LP Exp RP error { splerror(SPLC_ERR_B, @4.last_line, @4.last_column, @4.last_line, @4.last_column, "if requires at least one statement to be executed"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok; }
    | IF LP Exp RP Stmt ELSE Stmt { $$ = create_parent_node(AST_STMT, @$.first_line, 7, $1, $2, $3, $4, $5, $6, $7); }
    | IF error RP Stmt ELSE Stmt { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.first_line, @2.first_column, "expected '(' here"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok; }
    | IF LP Exp RP Stmt ELSE error { $$ = create_parent_node(AST_STMT, @$.first_line, 7, $1, $2, $3, $4, $5, $6, $7); }
    | IF LP Exp error { splerror(SPLC_ERR_B, @3.last_line, @3.last_column, @3.last_line, @3.last_column, "missing closing parenthesis ')'"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok; }
    | ELSE Stmt { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "hanging else is not allowed."); $$ = create_parent_node(AST_STMT, @$.first_line, 2, $1, $2); yyerrok; }

    | WHILE LP Exp RP Stmt { $$ = create_parent_node(AST_STMT, @$.first_line, 5, $1, $2, $3, $4, $5); }
    | WHILE error RP Stmt { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.first_line, @2.first_column, "expected '(' here"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok; }
    | WHILE LP Exp RP error { splerror(SPLC_ERR_B, @4.last_line, @4.last_column, @4.last_line, @4.last_column, "while loop requires at least one statement to be executed"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok; }
    /* | WHILE Exp RP Stmt { splerror(SPLC_ERR_B, @1.last_line, @1.last_column, @1.last_line, @1.last_column, "missing opening parenthesis '('"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok;  } */
    | WHILE LP Exp error { splerror(SPLC_ERR_B, @3.last_line, @3.last_column, @3.last_line, @3.last_column, "missing closing parenthesis ')'"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok;  }

    | FOR LP ForLoopBody RP Stmt { $$ = create_parent_node(AST_STMT, @$.first_line, 5, $1, $2, $3, $4, $5); }
    | FOR LP ForLoopBody RP error { splerror(SPLC_ERR_B, @4.last_line, @4.last_column, @4.last_line, @4.last_column, "for loop requires at least one statement to be executed"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok; }
    | FOR LP ForLoopBody error { splerror(SPLC_ERR_B, @3.last_line, @3.last_column, @3.last_line, @3.last_column, "missing closing parenthesis ')'"); $$ = create_parent_node(AST_STMT, @$.first_line, 0); yyerrok; }
    ;

ForLoopBody: Def Exp SEMI Exp { $$ = create_parent_node(AST_FOR_LOOP_BODY, @$.first_line, 4, $1, $2, $3, $4); }

    | SEMI Exp SEMI Exp { $$ = create_parent_node(AST_FOR_LOOP_BODY, @$.first_line, 4, $1, $2, $3, $4); }
    | Def Exp SEMI { $$ = create_parent_node(AST_FOR_LOOP_BODY, @$.first_line, 3, $1, $2, $3); }
    | Def SEMI Exp { $$ = create_parent_node(AST_FOR_LOOP_BODY, @$.first_line, 3, $1, $2, $3); }

    | SEMI Exp SEMI { $$ = create_parent_node(AST_FOR_LOOP_BODY, @$.first_line, 3, $1, $2, $3); }
    | SEMI SEMI Exp { $$ = create_parent_node(AST_FOR_LOOP_BODY, @$.first_line, 3, $1, $2, $3); }
    | Def SEMI { $$ = create_parent_node(AST_FOR_LOOP_BODY, @$.first_line, 2, $1, $2); }
    
    | SEMI SEMI { $$ = create_parent_node(AST_FOR_LOOP_BODY, @$.first_line, 2, $1, $2); }
    ;

/* Definition: List of definitions. Recursive definition. */
DefList: Def DefList { $$ = create_parent_node(AST_DEF_LIST, @$.first_line, 2, $1, $2); }
    | Def { $$ = create_parent_node(AST_DEF_LIST, @$.first_line, 1, $1); }
    ;

/* Definition: Base */
Def: Specifier DecList SEMI { $$ = create_parent_node(AST_DEF, @$.first_line, 3, $1, $2, $3); }
    | Specifier DecList error { splerror(SPLC_ERR_B, @2.last_line, @2.last_column, @2.last_line, @2.last_column, "missing semicolon ';'"); $$ = create_parent_node(AST_DEF, @$.first_line, 0); yyerrok; }
    ;

/* Definition: Declaration of multiple variable.  */ 
DecList: Dec { $$ = create_parent_node(AST_DEC_LIST, @$.first_line, 1, $1); }
    | Dec COMMA DecList { $$ = create_parent_node(AST_DEC_LIST, @$.first_line, 3, $1, $2, $3); }
    ;

/* Definition: Single declaration unit. */
Dec: VarDec { $$ = create_parent_node(AST_DEC, @$.first_line, 1, $1); }
    | VarDec LP RP { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @3.last_line, @3.last_column, "function definition not allowed here."); $$ = create_parent_node(AST_DEC, @$.first_line, 0); yyerrok; } 
    | VarDec LP Exp RP { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @4.last_line, @4.last_column, "function definition not allowed here."); $$ = create_parent_node(AST_DEC, @$.first_line, 0); yyerrok; } 
    | VarDec ASSIGN Exp { $$ = create_parent_node(AST_DEC, @$.first_line, 3, $1, $2, $3); }
    | VarDec ASSIGN error { splerror(SPLC_ERR_B, @3.first_line, @3.first_column, @3.last_line, @3.last_column, "invalid initialization"); $$ = create_parent_node(AST_DEC, @$.first_line, 0); yyerrok; }
    ;

/* Expression */
Exp: Exp ASSIGN Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | ASSIGN Exp {  splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '='"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | ASSIGN error {  splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '='"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }

    | Exp AND Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp OR Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp BITWISE_AND Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp BITWISE_OR Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | AND Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '&&'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | OR Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '||'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | BITWISE_AND Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '&'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | BITWISE_OR Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '|'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | AND { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '&&'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | OR { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '||'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | BITWISE_AND { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '&'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | BITWISE_OR { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '|'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }

    /* | Exp COMMA Exp {$$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); } */
    /* | COMMA Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before ','"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; } */
    /* | COMMA { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after ','"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; } */

    | Exp LT Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp LE Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp GT Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp GE Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp NE Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp EQ Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | LT Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '<'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | LE Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '<='"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | GT Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '>'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | GE Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '>='"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | NE Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '!='"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | EQ Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '=='"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | LT { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '<'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | LE { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '<='"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | GT { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '>'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | GE { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '>='"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | NE { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '!='"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | EQ { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '=='"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }

    | Exp PLUS Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp MINUS Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp PLUS error { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.last_line, @2.last_column, "expected expression after '+'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | Exp MINUS error { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.last_line, @2.last_column, "expected expression after '-'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }

    | Exp MUL Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | Exp DIV Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | MUL Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '*'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | DIV Exp { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression before '/'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | MUL error { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '*'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | DIV error { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.last_line, @1.last_column, "expected expression after '/'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }

    | LP Exp RP { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | LP Exp error { splerror(SPLC_ERR_B, @2.last_line, @2.last_column, @2.last_line, @2.last_column, "missing closing parenthesis ')'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | LP error RP { splerror(SPLC_ERR_B, @2.first_line, @2.first_column, @2.last_line, @2.last_column, "invalid expression"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    /* | Exp RP { splerror(SPLC_ERR_B, @1.first_line, @1.first_column, @1.first_line, @1.first_column, "expected '(' here"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; } */

    | Exp PLUS PLUS %prec POST_PLUS { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    /* | PLUS PLUS Exp %prec PRE_PLUS { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); } */
    | PLUS Exp %prec UPLUS { $$ = create_parent_node(AST_EXP, @$.first_line, 2, $1, $2); }
    | Exp MINUS MINUS %prec POST_MINUS { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    /* | MINUS MINUS Exp %prec PRE_MINUS { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); } */
    | MINUS Exp %prec UMINUS { $$ = create_parent_node(AST_EXP, @$.first_line, 2, $1, $2); }
    | NOT Exp { $$ = create_parent_node(AST_EXP, @$.first_line, 2, $1, $2); }
    | ID LP RP { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }
    | ID LP Args RP { $$ = create_parent_node(AST_EXP, @$.first_line, 4, $1, $2, $3, $4); }
    | ID LP Args error { splerror(SPLC_ERR_B, @3.last_line, @3.last_column, @3.last_line, @3.last_column, "missing closing parenthesis ')'"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }
    | ID LP error RP { splerror(SPLC_ERR_B, @3.first_line, @3.first_column, @3.last_line, @3.last_column, "invalid argument list"); $$ = create_parent_node(AST_EXP, @$.first_line, 0); yyerrok; }

    | Exp LSB Exp RSB { $$ = create_parent_node(AST_EXP, @$.first_line, 4, $1, $2, $3, $4); }
    | Exp DOT ID { $$ = create_parent_node(AST_EXP, @$.first_line, 3, $1, $2, $3); }

    | ID { $$ = create_parent_node(AST_EXP, @$.first_line, 1, $1); }
    | INT { $$ = create_parent_node(AST_EXP, @$.first_line, 1, $1); }
    | FLOAT { $$ = create_parent_node(AST_EXP, @$.first_line, 1, $1); }
    | CHAR { $$ = create_parent_node(AST_EXP, @$.first_line, 1, $1); }
    | StrExp { $$ = create_parent_node(AST_EXP, @$.first_line, 1, $1); }
    ;

/* String intermediate expression. Allowing concatenation of strings. */
StrExp: STR { $$ = create_parent_node(AST_STREXP, @$.first_line, 1, $1); }
    | STR StrExp { $$ = create_parent_node(AST_STREXP, @$.first_line, 2, $1, $2); }
    ;

/* Argument: List of arguments */
Args: Exp COMMA Args { $$ = create_parent_node(AST_ARGS, @$.first_line, 3, $1, $2, $3); }
    | Exp { $$ = create_parent_node(AST_ARGS, @$.first_line, 1, $1); }
    ;

%%

void yyerror(const char *s) {
    // suppressed
    /* fprintf(stderr, "%s at line %d\n", s, yylloc.first_line); */
}
