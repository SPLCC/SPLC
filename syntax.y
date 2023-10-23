%{
    #include "lex.yy.c"
    void yyerror(const char*);
%}

%token INT
%token FLOAT
%token CHAR
%token ID
%token TYPE
%token STRUCT
%token IF
%token ELSE
%token WHILE
%token RETURN
%token DOT
%token SEMI
%token COMMA
%token ASSIGN
%token LT
%token LE
%token GT
%token GE
%token NE
%token EQ
%token PLUS
%token MINUS
%token MUL
%token DIV
%token AND
%token OR
%token NOT
%token LP
%token RP
%token LB
%token RB
%token LC
%token RC


%%
stmt : INT {printf("INT");}
     | FLOAT {printf("FLOAT");}
     | CHAR {printf("CHAR");}
     ;
%%
void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}
int main() {
    yyparse();
}