%option yylineno
%{
    // #include "syntax.tab.h"
    #include <stdlib.h>
    #include <string.h>
    
    int check_hex(char* s);
    void errprintf(char* msg);

    // TODO 返回ID的identifier  CHAR：value   TYPE:具体type
    // TODO 剩下的无法匹配的字符目前返回ID
%}

%%

"int"|"float"|"char"   { return TYPE;   }
"struct"               { return STRUCT; }
"if"                   { return IF;     }
"else"                 { return ELSE;   }
"while"                { return WHILE;  }
"return"               { return RETURN; }

[0-9]+\.[0-9]+ { yylval = atof(yytext); return FLOAT; }

[0-9]+ {
    if (yytext[0] == '0' && yyleng > 1) {
        errprintf("Illegal decimal integer starting with 0");
    }
    yylval = atoi(yytext);
    return INT;
}


0[xX][0-9a-zA-Z]+ {
    // skip 0x
    if (!check_hex(yytext + 2)) {
        errprintf("Illegal hex-form integer");
    } else if (yyleng > 3 && yytext[2] == '0') {
        errprintf("Illegal hexadecimal integer starting with 0");
    }
    yylval = strtol(yytext, NULL, 16);
    return INT;
}
'\\x[0-9a-zA-Z]{2}' {
    yytext++; 
    yytext[4] = '\0'; 
    yytext[0]='0';
    if(!check_hex(yytext + 2)) {
        errprintf("Illegal hex-form character");
    }

    return CHAR;

}

[a-zA-Z_][a-zA-Z0-9_]* { return ID; }

[0-9][a-zA-Z0-9_]* {
        errprintf("Illegal identifier starting with digits");
        return ID;
}

"<=" { return LE; }
">"  { return GT; }
">=" { return GE; }
"!=" { return NE; }
"==" { return EQ; }
"."  { return DOT; }
";"  { return SEMI; }
","  { return COMMA; }
"="  { return ASSIGN; }
"<"  { return LT; }
"+"  { return PLUS; }
"-"  { return MINUS; }
"*"  { return MUL; }
"/"  { return DIV; }
"&&" { return AND; }
"||" { return OR; }
"!"  { return NOT; }
"("  { return LP; }
")"  { return RP; }
"["  { return LB; }
"]"  { return RB; }
"{"  { return LC; }
"}"  { return RC; }

'.' { return CHAR; }

[ \n\r\t] {}

. { fprintf(stderr, "Error type A at line %d: Mysterious lexeme %s\n", yylineno, yytext); return ID; }
%%

int check_hex(char* s)
    {
        int n = strlen(s);
        for(int i = 0; i < n; i++)
            if((s[i]<'0'||s[i]>'9')&&(s[i]<'a'||s[i]>'f')&&(s[i]<'A'||s[i]>'Z')) return 0;
        return 1;
    }

void errprintf(char* msg) {
    fprintf(stderr, "Error type A at line %d: %s `%s`\n", yylineno, msg, yytext);
}