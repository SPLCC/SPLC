%option yylineno
%{

    #include "syntax.tab.h"
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>

    // TODO: define YY_USER_ACTION

    int err_flag = 0;
    
    // TODO 剩下的无法匹配的字符目前返回ID
%}

%%

"int"|"float"|"char"   {
    yylval = createLeafNode("TYPE"); 
    yylval->type = 1;
    yylval->string_val = strdup(yytext);
    return TYPE;   
}
"struct"               { yylval = createLeafNode("STRUCT"); return STRUCT; }
"if"                   { yylval = createLeafNode("IF"); return IF;     }
"else"                 { yylval = createLeafNode("ELSE"); return ELSE;   }
"while"                { yylval = createLeafNode("WHILE"); return WHILE;  }
"return"               { yylval = createLeafNode("RETURN"); return RETURN; }

[0-9]+\.[0-9]+ {
    yylval = createLeafNode("FLOAT");
    yylval->type = 3;
    yylval->float_val = atof(yytext);
    return FLOAT; 
}

[0-9]+ {
    if (yytext[0] == '0' && yyleng > 1) {
        errA("Illegal decimal integer starting with 0");
    }
    yylval = createLeafNode("INT");
    yylval->type = 2;
    yylval->int_val = atoi(yytext);
    return INT;
}


0[xX][0-9a-zA-Z]+ {
    // skip 0x
    if (!check_hex(yytext + 2)) {
        errA("Illegal hex-form integer");
    } else if (yyleng > 3 && yytext[2] == '0') {
        errA("Illegal hexadecimal integer starting with 0");
    }
    yylval = createLeafNode("INT");
    yylval->type = 2;
    yylval->int_val = strtol(yytext, NULL, 16);
    return INT;
}
'\\x[0-9a-zA-Z]*' {
    yylval = createLeafNode("CHAR");
    yylval->type = 1;
    yylval->string_val = strdup(yytext);

    yytext++;
    if(!check_hex(yytext + 2)) {
        errA("Illegal hex-form character");
    }
    return CHAR;

}

[a-zA-Z_][a-zA-Z0-9_]* { 
        yylval = createLeafNode("ID");
        yylval->type = 1;
        yylval->string_val = strdup(yytext);
        return ID;
    }

[0-9][a-zA-Z0-9_]* {
        yylval = createLeafNode("ID");
        yylval->type = 1;
        yylval->string_val = strdup(yytext);
        errA("Illegal identifier starting with digits");
        return ID;
}

'.' {
        yylval = createLeafNode("CHAR");
        yylval->type = 1;
        yylval->string_val = strdup(yytext);
        return CHAR; 
    }

"<=" { yylval = createLeafNode("LE"); return LE; }
">"  { yylval = createLeafNode("GT"); return GT; }
">=" { yylval = createLeafNode("GE"); return GE; }
"!=" { yylval = createLeafNode("NE"); return NE; }
"==" { yylval = createLeafNode("EQ"); return EQ; }
"."  { yylval = createLeafNode("DOT"); return DOT; }
";"  { yylval = createLeafNode("SEMI"); return SEMI; }
","  { yylval = createLeafNode("COMMA"); return COMMA; }
"="  { yylval = createLeafNode("ASSIGN"); return ASSIGN; }
"<"  { yylval = createLeafNode("LT"); return LT; }
"+"  { yylval = createLeafNode("PLUS"); return PLUS; }
"-"  { yylval = createLeafNode("MINUS"); return MINUS; }
"*"  { yylval = createLeafNode("MUL"); return MUL; }
"/"  { yylval = createLeafNode("DIV"); return DIV; }

"&&" { yylval = createLeafNode("AND"); return AND; }
"||" { yylval = createLeafNode("OR"); return OR; }
"&"  { 
    errA("Mysterious lexeme");
    yylval = createLeafNode("AND"); 
    return AND; 
}
"|"  {
    errA("Mysterious lexeme");
    yylval = createLeafNode("OR"); 
    return OR; 
}

"!"  { yylval = createLeafNode("NOT"); return NOT; }
"("  { yylval = createLeafNode("LP"); return LP; }
")"  { yylval = createLeafNode("RP"); return RP; }
"["  { yylval = createLeafNode("LB"); return LB; }
"]"  { yylval = createLeafNode("RB"); return RB; }
"{"  { yylval = createLeafNode("LC"); return LC; }
"}"  { yylval = createLeafNode("RC"); return RC; }



"\n" { yycolno = 1; }
[ \r\t] {}

. { 
    yylval = createLeafNode("ID");
    yylval->type = 1;
    yylval->string_val = strdup(yytext);
    errA("Mysterious lexeme");
    return ID; 
}
%%

int check_hex(char* s)
    {
        int n = strlen(s);
        if (n == 0) return 0; // cannot be empty

        for(int i = 0; i < n; i++)
            if(!isxdigit(s[i])) return 0;
        return 1;
    }

void errA(char* msg) {
    fprintf(stdout, "Error type A at line %d: %s `%s`\n", yylineno, msg, yytext);
    err_flag = 1;
    // printf("Error type A at line %d: %s `%s`\n", yylineno, msg, yytext);
}