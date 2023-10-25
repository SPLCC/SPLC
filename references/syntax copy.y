%{
    #include "lex.yy.c"
    #include "treenode.h"
    void yyerror(const char*);
%}

%token INT FLOAT CHAR ID TYPE STRUCT IF ELSE WHILE RETURN DOT SEMI COMMA ASSIGN LT LE GT GE NE EQ PLUS MINUS MUL DIV AND OR NOT LP RP LB RB LC RC


%%
/* high-level definition */
Program : ExtDefList { $$ = createNode("Program", 1, $1); printParseTree($$, 0); }
        ;

ExtDefList : ExtDef ExtDefList {$$ = createNode("ExtDefList", 2, $1, $2);}
        |
        {
            $$ = NULL;
        }
           ;

ExtDef : Specifier ExtDecList SEMI { $$ = createNode("ExtDef", 3, $1, $2, $3);}
       | Specifier SEMI { $$ = createNode("ExtDef", 2, $1, $2);}
       | Specifier FunDec CompSt { $$ = createNode("ExtDef", 3, $1, $2, $3);}
       ;

ExtDecList : VarDec { $$ = createNode("ExtDecList", 1, $1);}
           | VarDec COMMA ExtDecList { $$ = createNode("ExtDecList", 3, $1, $2, $3);}
           ;

/* specifier */
Specifier : TYPE { $$ = createNode("Specifier", 1, $1);}
          | StructSpecifier { $$ = createNode("Specifier", 1, $1);}
          ;

StructSpecifier : STRUCT ID LC DefList RC { $$ = createNode("StructSpecifier", 5, $1, $2, $3, $4, $5);}
                | STRUCT ID { $$ = createNode("StructSpecifier", 2, $1, $2);}
                ;

/* declarator */
VarDec : ID { $$ = createNode("VarDec", 1, $1);}
       | VarDec LB INT RB { $$ = createNode("VarDec", 4, $1, $2, $3, $4);}
       ;

FunDec : ID LP VarList RP { $$ = createNode("FunDec", 4, $1, $2, $3, $4);}
       | ID LP RP { $$ = createNode("FunDec", 3, $1, $2, $3);}
       ;

VarList : ParamDec COMMA VarList { $$ = createNode("VarList", 3, $1, $2, $3);}
        | ParamDec { $$ = createNode("VarList", 1, $1);}
        ;

ParamDec : Specifier VarDec {$$ = createNode("ParamDec", 2, $1, $2);}
         ;

/* statement */
CompSt : LC DefList StmtList RC {
     $$ = createNode("CompSt", 4, $1, $2, $3, $4);
    }
       ;

StmtList : Stmt StmtList {$$ = createNode("StmtList", 2, $1, $2);}
         |
         {
            $$ = NULL;
         }
         ;

Stmt : Exp SEMI {$$ = createNode("Stmt", 2, $1, $2);}
     | CompSt {$$ = createNode("Stmt", 1, $1);}
     | RETURN Exp SEMI {$$ = createNode("Stmt", 3, $1, $2, $3);}
     | IF LP Exp RP Stmt {$$ = createNode("Stmt", 5, $1, $2, $3, $4, $5);}
     | IF LP Exp RP Stmt ELSE Stmt {$$ = createNode("Stmt", 7, $1, $2, $3, $4, $5, $6, $7);}
     | WHILE LP Exp RP Stmt {$$ = createNode("Stmt", 5, $1, $2, $3, $4, $5);}
     ;

/* local definition */
DefList : Def DefList { $$ = createNode("DefList", 2, $1, $2);}
        | 
        {
            $$ = NULL;
        }
        ;

Def : Specifier DecList SEMI { $$ = createNode("Def", 3, $1, $2, $3);}
    ;

DecList : Dec { $$ = createNode("DecList", 1, $1);}
        | Dec COMMA DecList { $$ = createNode("DecList", 3, $1, $2, $3);}
        ;

Dec : VarDec { $$ = createNode("Dec", 1, $1);}
    | VarDec ASSIGN Exp { $$ = createNode("Dec", 3, $1, $2, $3);}
    ;

/* Expression */
Exp : Exp ASSIGN Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp AND Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp OR Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp LT Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp LE Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp GT Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp GE Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp NE Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp EQ Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp PLUS Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp MINUS Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp MUL Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp DIV Exp { $$ = createNode("Exp", 3, $1, $2, $3);}
    | LP Exp RP { $$ = createNode("Exp", 3, $1, $2, $3);}
    | MINUS Exp { $$ = createNode("Exp", 2, $1, $2);}
    | NOT Exp { $$ = createNode("Exp", 2, $1, $2);}
    | ID LP Args RP { $$ = createNode("Exp", 4, $1, $2, $3, $4);}
    | ID LP RP { $$ = createNode("Exp", 3, $1, $2, $3);}
    | Exp LB Exp RB { $$ = createNode("Exp", 4, $1, $2, $3, $4);}
    | Exp DOT ID { $$ = createNode("Exp", 3, $1, $2, $3);}
    | ID { $$ = createNode("Exp", 1, $1);}
    | INT { $$ = createNode("Exp", 1, $1);}
    | FLOAT { $$ = createNode("Exp", 1, $1);}
    | CHAR { $$ = createNode("Exp", 1, $1);}
    ;

Args : Exp COMMA Args { $$ = createNode("Args", 3, $1, $2, $3);}
     | Exp { $$  = createNode("Args", 1, $1);}
     ;
%%
void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}
int main() {
    yyparse();
}
