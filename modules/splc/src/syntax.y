%language "c++"
%code requires {}
%define parse.error detailed
/* %define api.value.type {} */
%locations
%define api.location.file "include/ast/location.hpp"
%define api.location.include {<ast/location.hpp>}

%token LTR_INT LTR_FLOAT LTR_CHAR
%token TYPE_VOID TYPE_INT TYPE_SIGNED TYPE_UNSIGNED TYPE_LONG TYPE_FLOAT TYPE_CHAR STR_UNIT
%token AUTO EXTERN REGISTER STATIC TYPEDEF
%token KWD_CONST RESTRICT VOLATILE
%token ELLIPSIS
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

/* %destructor {} <> */

/* Reference: https://learn.microsoft.com/en-us/cpp/c-language/ */
/* Many of the C language features are not implemented here. */

%%
/* Entire translation unit */
translation-unit: 
      external-declaration-list {}
    | {}
    ;

/* External definition list: Recursive definition */
external-declaration-list: 
      external-declaration {}
    | external-declaration-list external-declaration {}
    ;

/* External definition list: A single unit of one of {}. */
external-declaration: 
      SEMI {}
    | declaration {}
    | function-definition {}
    ;

declaration-specifiers:
      storage-class-specifier {}
    | type-specifier {}
    | type-qualifier {}
    | function-specifier {}
    | declaration-specifiers type-specifier {}
    | declaration-specifiers storage-class-specifier {}
    | declaration-specifiers type-qualifier {}
    | declaration-specifiers function-specifier {}
    ;

storage-class-specifier:
      AUTO {}
    | EXTERN {}
    | REGISTER {}
    | STATIC {}
    | TYPEDEF {}
    ;

specifier-qualifier-list:
      type-specifier {}
    | type-qualifier {}
    | specifier-qualifier-list type-specifier {}
    | specifier-qualifier-list type-qualifier {}
    ;

type-specifier: 
      builtin-type-specifier {}
    /* | identifier {} */
    | struct-or-union-specifier {}
    | enum-specifier {}
    | TYPEDEF_NAME {}
    ;

function-specifier:
      INLINE {}
    ;

type-qualifier:
      KWD_CONST {}
    | RESTRICT {}
    | VOLATILE {}
    ;

type-name:
      specifier-qualifier-list {}
    | specifier-qualifier-list abstract-declarator {}
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
      pointer {}
    | pointer direct-abstract-declarator {}
    ;

direct-abstract-declarator:
      LP abstract-declarator RP {}
    | direct-abstract-declarator LSB assignment-expression RSB {}
    | direct-abstract-declarator LSB RSB {}
    | direct-abstract-declarator LSB error {}
    | direct-abstract-declarator RSB {} 
    ;

/* Specify a structure */
struct-or-union-specifier: 
      struct-or-union identifier {}
    | struct-or-union struct-declaration-body {}
    | struct-or-union identifier struct-declaration-body {}
    ;

struct-or-union:
      KWD_STRUCT
    | KWD_UNION
    ;

struct-declaration-body:
      LC RC {}
    | LC struct-declaration-list RC {}

    | LC error {}
    | LC struct-declaration-list error {}
    ;

struct-declaration-list:
      struct-declaration {}
    | struct-declaration-list struct-declaration {}
    ;

struct-declaration:
      specifier-qualifier-list SEMI {}
    | specifier-qualifier-list struct-declarator-list SEMI {}

    | specifier-qualifier-list error {}
    | specifier-qualifier-list struct-declarator-list error {}
    ;

struct-declarator-list:
      struct-declarator {}
    | struct-declarator-list COMMA struct-declarator {}

    | struct-declarator-list COMMA error {}
    ;

struct-declarator:
      declarator {}
    | COLON constant-expression {}
    | declarator COLON constant-expression {}

    | COLON error {}
    | declarator COLON error {}
    ;

enum-specifier:
      KWD_ENUM identifier {}
    | KWD_ENUM enumerator-body {}
    | KWD_ENUM identifier enumerator-body {}
    
    | KWD_ENUM error {}
    ;

enumerator-body:
      LC RC {}
    | LC enumerator-list RC {}
    | LC enumerator-list COMMA RC {}

    | LC error {}
    | LC enumerator-list error {}
    ;

enumerator-list:
      enumerator {}
    | enumerator-list COMMA enumerator {}

    | COMMA enumerator {}
    ;

enumerator:
      enumeration-constant {}
    | enumeration-constant ASSIGN constant-expression {}

    | enumeration-constant ASSIGN error {}
    ;

enumeration-constant:
      identifier
    ;

/* Single variable declaration */
declarator: 
      pointer direct-declarator {}
    | direct-declarator {}
    ;

direct-declarator:
      identifier {}
    | LP declarator RP {}
    | direct-declarator LSB assignment-expression RSB {}
    | direct-declarator LSB RSB {}

    | direct-declarator LSB assignment-expression error {} 
    /* | direct-declarator error {}  */
    | direct-declarator RSB {} 
    ;

pointer:
      ASTRK {}
    | ASTRK type-qualifier-list {}
    | pointer ASTRK {}
    | pointer ASTRK type-qualifier-list {}
    ;

type-qualifier-list:
      type-qualifier {} 
    | type-qualifier-list type-qualifier {}
    ;

/* Definition: List of definitions. Recursive definition. */
/* declaration-list: 
      declaration {}
    | declaration-list declaration {}
    ; */

/* Definition: Base */
declaration: 
      direct-declaration SEMI {}
    | direct-declaration error {}
    ;

direct-declaration:
      declaration-specifiers {}
    | declaration-specifiers init-declarator-list {}
    ;

/* Definition: Declaration of multiple variable.  */ 
init-declarator-list: 
      init-declarator {}
    | init-declarator COMMA init-declarator-list {}

    | init-declarator COMMA {}
    | COMMA init-declarator-list {}
    | COMMA {}
    ;

/* Definition: Single declaration unit. */
init-declarator: 
      declarator {}
    | declarator ASSIGN initializer {}
    | declarator ASSIGN error {}
    ;

initializer:
      assignment-expression {}
    | LC initializer-list RC {}
    | LC initializer-list COMMA RC {}
    | LC initializer-list error {}
    ;

initializer-list:
      initializer {}
    | designation initializer {}
    | initializer-list COMMA designation initializer {}
    | initializer-list COMMA initializer {}

    | designation error {}
    | initializer-list COMMA error {}
    ;

designation:
      designator-list ASSIGN {}
    ;

designator-list:
      designator {}
    | designator-list designator {}
    ;

designator:
      LSB constant-expression RSB {}
    | DOT identifier {}

    | LSB constant-expression error {}
    | DOT error {}
    ;

function-definition:
      declaration-specifiers function-declarator compound-statement {}
    | function-declarator compound-statement {} 
    | declaration-specifiers function-declarator SEMI {}

    | declaration-specifiers function-declarator error {}
    ;

/* Function: Function name and body. */
function-declarator: 
      direct-function-declarator {}
    | pointer direct-function-declarator {}
    ;

direct-function-declarator:
      direct-declarator-for-function LP parameter-type-list RP {}
    /* | direct-declarator-for-function LP RP {} */

    /* | direct-declarator-for-function LP error {} */
    | direct-declarator-for-function LP parameter-type-list error {}
    /* | direct-declarator-for-function LP error {} */

    | LP parameter-type-list RP {}
    /* | LP RP {} */
    ;

direct-declarator-for-function:
      identifier {}
    ;

/* List of variables names */
parameter-type-list: 
      {}
    | parameter-list {}
    | parameter-list COMMA ELLIPSIS {}
    ;

parameter-list:
      parameter-declaration {}
    | parameter-list COMMA parameter-declaration {}

    | parameter-list COMMA error {}
    | COMMA {}
    ;

/* Parameter declaration */ 
parameter-declaration: 
      declaration-specifiers declarator {}
    | declaration-specifiers abstract-declarator {}
    | declaration-specifiers {}

    /* | error {} */
    ;

/* Compound statement: A new scope. */
compound-statement: 
      /* LC general-statement-list RC */
      LC general-statement-list RC {}
    | LC general-statement-list error {}
    /* | error RC { SPLC_MSG(SPLM_ERR_SYN_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing opening bracket '{} */

      /* LC RC */
    | LC RC {}
    | LC error {}
    ;

/* wrapper for C99 standard for statements */
general-statement-list: 
      statement {}
    | declaration {}
    | general-statement-list statement {}
    | general-statement-list declaration {}
    ;

/* Statement: List of statements. Recursive definition. */
/* statement-list: 
      statement {}
    | statement-list statement {} 
    ; */

/* Statement: A single statement. */
statement: 
      SEMI {}
    | compound-statement {}
    | expression-statement {}
    | selection-statement {}
    | iteration-statement {}
    | labeled-statement {}
    | jump-statement {}

    /* | error SEMI {} */
    ;

expression-statement:
      expression SEMI {}
    | expression error {}
    ;

selection-statement:
      IF LP expression RP statement %prec THEN {}

    | IF error RP statement %prec THEN {}
    | IF LP RP statement %prec THEN {}
    | IF LP expression RP error %prec THEN {}
    | IF LP RP error %prec THEN {}
    
    | IF LP expression RP statement ELSE statement %prec ELSE {}

    | IF error RP statement ELSE statement %prec ELSE {}
    | IF LP expression RP statement ELSE error %prec ELSE {}
    | IF LP RP statement ELSE statement %prec ELSE {}
    | IF LP RP statement ELSE error %prec ELSE {}
    | IF LP expression error %prec ELSE {}
    | ELSE statement {}

    | SWITCH LP expression RP statement {}
    /* | SWITCH LP expression statement {} */
    | SWITCH error RP statement {}
    ;

labeled-statement:
      identifier COLON statement {}
    | CASE constant-expression COLON statement {}
    | DEFAULT COLON statement {}

    | COLON statement {}
    ;

jump-statement:
      GOTO identifier SEMI {}
    | CONTINUE SEMI {}
    | BREAK SEMI {}
    | RETURN expression SEMI {}
    | RETURN expression error {}
    | RETURN SEMI {}

    | RETURN error {}
    ;

iteration-statement:
      WHILE LP expression RP statement {}
    | WHILE error RP statement {}
    | WHILE LP expression RP error {}
    | WHILE LP expression error {}
    
    | DO statement WHILE LP expression RP SEMI {}
    | DO statement WHILE LP error SEMI {}

    | FOR LP for-loop-body RP statement {}
    | FOR LP for-loop-body RP error {}
    | FOR LP for-loop-body error {}
    ;

for-loop-body: // TODO: add constant expressions 
      initialization-expression SEMI expression SEMI expression {}

    | SEMI expression SEMI expression {} 
    | initialization-expression SEMI expression SEMI {}
    | initialization-expression SEMI SEMI expression {}

    | SEMI expression SEMI {}
    | SEMI SEMI expression {}
    /* | definition SEMI {} */
    | initialization-expression SEMI SEMI {}
    
    | SEMI SEMI {}
    ;

constant-expression: 
      conditional-expression {}
    ;

constant:
      LTR_INT {}
    | LTR_FLOAT {}
    | LTR_CHAR {}
    ;

primary-expression:
      identifier
    | constant {}
    | string-literal {}
    | LP expression RP {}
    | LP expression error {}
    /* | LP expression {} */
    ;

postfix-expression:
      primary-expression
    | postfix-expression LSB expression RSB {}
    | postfix-expression LP argument-list RP {}
    /* | postfix-expression LP RP {} */
    | postfix-expression member-access-operator identifier {}
    | postfix-expression DPLUS {}
    | postfix-expression DMINUS {}
    | LP type-name RP LC initializer-list RC {}
    | LP type-name RP LC initializer-list COMMA RC {}

    | postfix-expression LSB expression error {}
    | postfix-expression LP argument-list error {}
    | postfix-expression member-access-operator {}
    | RARROW identifier {}
    | LP type-name RP LC initializer-list error {}
    ;

member-access-operator:
      DOT
    | RARROW
    ;

unary-expression:
      postfix-expression
    | DPLUS unary-expression {}
    | DMINUS unary-expression {}
    | unary-operator cast-expression %prec UPLUS {}
    | SIZEOF unary-expression {}
    | SIZEOF LP type-name RP {}

    | BW_AND error {}
    | ASTRK error {}
    | BW_NOT error {}
    | NOT error {}
    | DPLUS error {}
    | DMINUS error {}
    | SIZEOF error {}
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
    | LP type-name RP cast-expression {}

    | LP type-name RP error {}
    | LP type-name error {}
    ;

multiplicative-expression:
      cast-expression
    | multiplicative-expression multiplicative-operator cast-expression {}

    | multiplicative-expression multiplicative-operator error {}
    | division-operator cast-expression {}
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
    | additive-expression additive-operator multiplicative-expression {}

    | additive-expression additive-operator error {}
    ;

additive-operator:
      PLUS
    | MINUS
    ;

shift-expression:
      additive-expression
    | shift-expression shift-operator additive-expression {}

    | shift-expression shift-operator error {}
    | shift-operator additive-expression {}
    ;
  
shift-operator:
      LSHIFT
    | RSHIFT
    ;

relational-expression:
      shift-expression
    | relational-expression relational-operator shift-expression {}

    | relational-expression relational-operator error {}
    | relational-operator shift-expression {}
    ;

relational-operator:
      LT
    | GT
    | LE
    | GE
    ;

equality-expression:
      relational-expression
    | equality-expression equality-operator relational-expression {}

    | equality-expression equality-operator error {}
    | equality-operator relational-expression {}
    ;

equality-operator:
      EQ
    | NE
    ;

BW-AND-expression:
      equality-expression
    | BW-AND-expression BW_AND equality-expression {}

    | BW-AND-expression BW_AND error {}
    ;

BW-XOR-expression:
      BW-AND-expression
    | BW-XOR-expression BW_XOR BW-AND-expression {}

    | BW-XOR-expression BW_XOR error {}
    | BW_XOR BW-AND-expression {}
    ;

BW-OR-expression:
      BW-XOR-expression
    | BW-OR-expression BW_OR BW-XOR-expression {}

    | BW-OR-expression BW_OR error {}
    | BW_OR BW-XOR-expression {}
    ;

logical-AND-expression:
      BW-OR-expression
    | logical-AND-expression AND BW-OR-expression {}

    | logical-AND-expression AND error {}
    | AND BW-OR-expression {}
    ;

logical-OR-expression:
      logical-AND-expression
    | logical-OR-expression OR logical-AND-expression {}

    | logical-OR-expression OR error {}
    | OR logical-AND-expression {}
    ;

conditional-expression:
      logical-OR-expression {}
    | logical-OR-expression QM expression COLON conditional-expression {}

    | logical-OR-expression QM COLON conditional-expression {}
    | logical-OR-expression QM expression COLON {}
    | QM error {}
    ;

assignment-expression:
      conditional-expression {}
    
    | conditional-expression assignment-operator assignment-expression {}
    | conditional-expression assignment-operator error {}
    | assignment-operator assignment-expression {}
    
    /* | unary-expression assignment-operator assignment-expression {} */
    /* | unary-expression assignment-operator error {} */
    /* | assignment-operator assignment-expression {} */
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
      assignment-expression {}
    | expression COMMA assignment-expression {}

    | expression COMMA error {}
    | COMMA assignment-expression {}
    ;
  
initialization-expression:
      expression {}
    | direct-declaration {}
    ;

/* Argument: List of arguments */
argument-list: 
      {}
    | argument-list COMMA assignment-expression {}
    | assignment-expression {}

    | argument-list COMMA error {}
    /* | error {} */
    ;

/* String intermediate expression. Allowing concatenation of strings. */
string-literal: 
      STR_UNIT {}
    | string-literal STR_UNIT {}
    ;

identifier:
      ID {}
    ;
%%
