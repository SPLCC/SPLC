%{
    // #include "syntax.tab.h"
    #include <stdlib.h>
    #include <string.h>
    
    int check_hex(char* s);
    void errprintf(char* msg);
%}

%%

"int"|"float"|"char"    { printf("TYPE "); }
"struct"                { printf("STRUCT "); }
"if"                    { printf("IF "); }
"else"                  { printf("ELSE "); }
"while"                 { printf("WHILE "); }
"return"                { printf("RETURN "); }

[0-9]+ {
    if (yytext[0] == '0' && yyleng > 1) {
        errprintf("Illegal decimal integer starting with 0");
    }
    // return INT;
    printf("INT ");
}


0[xX][0-9a-zA-Z]+ {
    // skip 0x
    if (!check_hex(yytext + 2)) {
        errprintf("Illegal hex-form integer");
    } else if (yyleng > 3 && yytext[2] == '0') {
        errprintf("Illegal hexadecimal integer starting with 0");
    }
    printf("INT ");
}
'\\x[0-9a-zA-Z]{2}' {
    yytext++; 
    yytext[4] = '\0'; 
    yytext[0]='0';
    if(!check_hex(yytext)) {
        errprintf("Illegal hex-form character");
    }
    printf("CHAR %ld ", strtol(yytext, NULL, 16)); 

}

[a-zA-Z_][a-zA-Z0-9_]* { printf("ID %s ",  strdup(yytext)); }

[0-9][a-zA-Z0-9_]* {
        errprintf("Illegal identifier starting with digits");
        printf("ID %s ",  strdup(yytext)); 
    }
"<=" { printf("LE "); }
">" { printf("GT "); }
">=" { printf("GE "); }
"!=" { printf("NE "); }
"==" { printf("EQ "); }

"." { printf("DOT "); }
";" { printf("SEMI "); }
"," { printf("COMMA "); }
"=" { printf("ASSIGN "); }
"<" { printf("LT "); }

"+" { printf("PLUS "); }
"-" { printf("MINUS "); }
"*" { printf("MUL "); }
"/" { printf("DIV "); }
"&&" { printf("AND "); }
"||" { printf("OR "); }
"!" { printf("NOT "); }
"(" { printf("LP "); }
")" { printf("RP "); }
"[" { printf("LB "); }
"]" { printf("RB "); }
"{" { printf("LC "); }
"}" { printf("RC "); }
'.' { printf("CHAR %d ", yytext[1]); }

^[-+]?[0-9]*\.[0-9]+$ { printf("FLOAT %f ", atof(yytext)); }
\n {yylineno++;}
[ \r\t] {}
. { fprintf(stderr, "Error type A at line %d: Mysterious lexeme %s\n", yylineno, yytext); return 1; }
%%

int check_hex(char* s)
    {
        int n = strlen(s);
        for(int i=2;i<n;i++)
          if((s[i]<'0'||s[i]>'9')&&(s[i]<'a'||s[i]>'f')&&(s[i]<'A'||s[i]>'Z')) return 0;
        return 1;
    }

void errprintf(char* msg) {
    fprintf(stderr, "Error type A at line %d: %s `%s`\n", yylineno, msg, yytext);
}