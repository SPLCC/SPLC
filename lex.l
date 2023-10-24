%option yylineno
%{
    #define YYSTYPE struct TreeNode *
    #include "syntax.tab.h"
    #include "treenode.h"
    #include <stdlib.h>
    #include <string.h>
    
    int check_hex(char* s);
    void errprintf(char* msg);

    // TODO 返回ID的identifier  CHAR：value   TYPE:具体type
    // TODO 剩下的无法匹配的字符目前返回ID
%}

%%

"int"|"float"|"char"   { yylval = createNode("TYPE", 0); return TYPE;   }
"struct"               { yylval = createNode("STRUCT", 0); return STRUCT; }
"if"                   { yylval = createNode("IF", 0); return IF;     }
"else"                 { yylval = createNode("ELSE", 0); return ELSE;   }
"while"                { yylval = createNode("WHILE", 0); return WHILE;  }
"return"               { yylval = createNode("RETURN", 0); return RETURN; }

[0-9]+\.[0-9]+ {
    yylval = createNode("FLOAT", 0);
    yylval->type = 3;
    yylval->float_val = atof(yytext);
    return FLOAT; 
}

[0-9]+ {
    if (yytext[0] == '0' && yyleng > 1) {
        errprintf("Illegal decimal integer starting with 0");
    }
    yylval = createNode("INT", 0);
    yylval->type = 2;
    yylval->int_val = atoi(yytext);
    return INT;
}


0[xX][0-9a-zA-Z]+ {
    // skip 0x
    if (!check_hex(yytext + 2)) {
        errprintf("Illegal hex-form integer");
    } else if (yyleng > 3 && yytext[2] == '0') {
        errprintf("Illegal hexadecimal integer starting with 0");
    }
    yylval = createNode("INT", 0);
    yylval->type = 1;
    yylval->string_val = strdup(yytext);
    return INT;
}
'\\x[0-9a-zA-Z]{2}' {
    yylval = createNode("CHAR", 0);
    yylval->type = 1;
    yylval->string_val = strdup(yytext);

    yytext++; 
    yytext[4] = '\0'; 
    yytext[0]='0';
    if(!check_hex(yytext + 2)) {
        errprintf("Illegal hex-form character");
    }
    return CHAR;

}

[a-zA-Z_][a-zA-Z0-9_]* { 
        yylval = createNode("ID", 0);
        yylval->type = 1;
        yylval->string_val = strdup(yytext);
        return ID;
    }

[0-9][a-zA-Z0-9_]* {
        yylval = createNode("ID", 0);
        yylval->type = 1;
        yylval->string_val = strdup(yytext);
        errprintf("Illegal identifier starting with digits");
        return ID;
}

"<=" { yylval = createNode("LE", 0); return LE; }
">"  { yylval = createNode("GT", 0); return GT; }
">=" { yylval = createNode("GE", 0); return GE; }
"!=" { yylval = createNode("NE", 0); return NE; }
"==" { yylval = createNode("EQ", 0); return EQ; }
"."  { yylval = createNode("DOT", 0); return DOT; }
";"  { yylval = createNode("SEMI", 0); return SEMI; }
","  { yylval = createNode("COMMA", 0); return COMMA; }
"="  { yylval = createNode("ASSIGN", 0); return ASSIGN; }
"<"  { yylval = createNode("LT", 0); return LT; }
"+"  { yylval = createNode("PLUS", 0); return PLUS; }
"-"  { yylval = createNode("MINUS", 0); return MINUS; }
"*"  { yylval = createNode("MUL", 0); return MUL; }
"/"  { yylval = createNode("DIV", 0); return DIV; }
"&&" { yylval = createNode("AND", 0); return AND; }
"||" { yylval = createNode("OR", 0); return OR; }
"!"  { yylval = createNode("NOT", 0); return NOT; }
"("  { yylval = createNode("LP", 0); return LP; }
")"  { yylval = createNode("RP", 0); return RP; }
"["  { yylval = createNode("LB", 0); return LB; }
"]"  { yylval = createNode("RB", 0); return RB; }
"{"  { yylval = createNode("LC", 0); return LC; }
"}"  { yylval = createNode("RC", 0); return RC; }

'.' {
        yylval = createNode("CHAR", 0);
        yylval->type = 1;
        yylval->string_val = strdup(yytext);
        return CHAR; 
    }

[ \n\r\t] {}

. { 
    yylval = createNode("ID", 0);
    yylval->type = 1;
    yylval->string_val = strdup(yytext);
    fprintf(stderr, "Error type A at line %d: Mysterious lexeme %s\n", yylineno, yytext); 
    return ID; 
    }
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