%{
    #include "lex.yy.c"
    #include "treenode.h"
    void yyerror(const char*);
    void errB(const char*);
%}

%token INT FLOAT CHAR
%token ID TYPE 
%token STRUCT IF WHILE RETURN 
%token SEMI COMMA LC RC

%nonassoc LOWER_ELSE
%nonassoc ELSE

%right ASSIGN
%left OR
%left AND
%left LT LE GT GE NE EQ 
%left PLUS MINUS
%left MUL DIV
%right NOT
%left LP RP LB RB DOT


%%
/* high-level definition */
Program : ExtDefList { 
            $$ = createNode("Program", @$.first_line, 1, $1); 
            if (!err_flag) printParseTree($$, 0); 
        }
        ;

ExtDefList : ExtDef ExtDefList {$$ = createNode("ExtDefList", @$.first_line, 2, $1, $2);}
           | 
           {
               $$ = NULL;
           }
           ;

ExtDef : Specifier ExtDecList SEMI { $$ = createNode("ExtDef", @$.first_line, 3, $1, $2, $3);}
       | Specifier SEMI { $$ = createNode("ExtDef", @$.first_line, 2, $1, $2);}
       | Specifier FunDec CompSt { $$ = createNode("ExtDef", @$.first_line, 3, $1, $2, $3);}
       ;

ExtDecList : VarDec { $$ = createNode("ExtDecList", @$.first_line, 1, $1);}
           | VarDec COMMA ExtDecList { $$ = createNode("ExtDecList", @$.first_line, 3, $1, $2, $3);}
           ;

/* specifier */
Specifier : TYPE { $$ = createNode("Specifier", @$.first_line, 1, $1);}
          | StructSpecifier { $$ = createNode("Specifier", @$.first_line, 1, $1);}
          ;

StructSpecifier : STRUCT ID LC DefList RC { $$ = createNode("StructSpecifier", @$.first_line, 5, $1, $2, $3, $4, $5);}
                | STRUCT ID { $$ = createNode("StructSpecifier", @$.first_line, 2, $1, $2);}
                ;

/* declarator */
VarDec : ID { $$ = createNode("VarDec", @$.first_line, 1, $1);}
       | VarDec LB INT RB { $$ = createNode("VarDec", @$.first_line, 4, $1, $2, $3, $4);}
       ;

FunDec : ID LP VarList RP { $$ = createNode("FunDec", @$.first_line, 4, $1, $2, $3, $4);}
       | ID LP RP { $$ = createNode("FunDec", @$.first_line, 3, $1, $2, $3);}
       ;

VarList : ParamDec COMMA VarList { $$ = createNode("VarList", @$.first_line, 3, $1, $2, $3);}
        | ParamDec { $$ = createNode("VarList", @$.first_line, 1, $1);}
        ;

ParamDec : Specifier VarDec { $$ = createNode("ParamDec", @$.first_line, 2, $1, $2);}
         ;

/* statement */
CompSt : LC DefList StmtList RC {
            $$ = createNode("CompSt", @$.first_line, 4, $1, $2, $3, $4);
        }
       ;

StmtList : Stmt StmtList {$$ = createNode("StmtList", @$.first_line, 2, $1, $2);}
         |
         {
            $$ = NULL;
         }
         ;

Stmt : Exp SEMI {$$ = createNode("Stmt", @$.first_line, 2, $1, $2);}
     | CompSt {$$ = createNode("Stmt", @$.first_line, 1, $1);}
     | RETURN Exp SEMI {$$ = createNode("Stmt", @$.first_line, 3, $1, $2, $3);}
     | IF LP Exp RP Stmt %prec LOWER_ELSE {$$ = createNode("Stmt", @$.first_line, 5, $1, $2, $3, $4, $5);}
     | IF LP Exp RP Stmt ELSE Stmt {$$ = createNode("Stmt", @$.first_line, 7, $1, $2, $3, $4, $5, $6, $7);}
     | WHILE LP Exp RP Stmt {$$ = createNode("Stmt", @$.first_line, 5, $1, $2, $3, $4, $5);}
     | RETURN Exp error { errB("Missing semicolon ';'"); yyerrok;} // error
     ;

/* local definition */
DefList : Def DefList { $$ = createNode("DefList", @$.first_line, 2, $1, $2);}
        | 
        {
            $$ = NULL;
        }
        ;

Def : Specifier DecList SEMI { $$ = createNode("Def", @$.first_line, 3, $1, $2, $3);}
    ;

DecList : Dec { $$ = createNode("DecList", @$.first_line, 1, $1);}
        | Dec COMMA DecList { $$ = createNode("DecList", @$.first_line, 3, $1, $2, $3);}
        ;

Dec : VarDec { $$ = createNode("Dec", @$.first_line, 1, $1);}
    | VarDec ASSIGN Exp { $$ = createNode("Dec", @$.first_line, 3, $1, $2, $3);}
    ;

/* Expression */
Exp : Exp ASSIGN Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp AND Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp OR Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp LT Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp LE Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp GT Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp GE Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp NE Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp EQ Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp PLUS Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp MINUS Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp MUL Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp DIV Exp { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | LP Exp RP { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | MINUS Exp { $$ = createNode("Exp", @$.first_line, 2, $1, $2);}
    | NOT Exp { $$ = createNode("Exp", @$.first_line, 2, $1, $2);}
    | ID LP Args RP { $$ = createNode("Exp", @$.first_line, 4, $1, $2, $3, $4);}
    | ID LP RP { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | Exp LB Exp RB { $$ = createNode("Exp", @$.first_line, 4, $1, $2, $3, $4);}
    | Exp DOT ID { $$ = createNode("Exp", @$.first_line, 3, $1, $2, $3);}
    | ID { $$ = createNode("Exp", @$.first_line, 1, $1);}
    | INT { $$ = createNode("Exp", @$.first_line, 1, $1);}
    | FLOAT { $$ = createNode("Exp", @$.first_line, 1, $1);}
    | CHAR { $$ = createNode("Exp", @$.first_line, 1, $1);}
    | ID LP Args error // error recovery
    ;

Args : Exp COMMA Args { $$ = createNode("Args", @$.first_line, 3, $1, $2, $3);}
     | Exp { $$  = createNode("Args", @$.first_line, 1, $1);}
     ;

%%
void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

void errB(const char *s) {
    fprintf(stdout, "Error type B at line %d: %s\n", yylineno, s);
    err_flag = 1;
}

int main() {
    yyparse();
}
