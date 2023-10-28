%option yylineno
%{
    #include "spldef.h"
    #include "utils.h"
    #include "lut.h"
    #include "ast.h"

    #include "parser_syntax.tab.h"

    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
    // TODO: define YY_USER_ACTION
    int yycolno = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylineno; \
        yylloc.first_column = yycolno; \
        yylloc.last_line = yylineno; \
        yylloc.last_column = yycolno + yyleng; \
        yycolno += yyleng;
%}

%%

"int"|"float"|"char"   {
    yylval = create_leaf_node(AST_BUILTIN_TYPE);
    yylval->val = (void *)strdup(yytext);
    return TYPE;
}

"struct"               { yylval = create_leaf_node(AST_STRUCT); return STRUCT; }
"if"                   { yylval = create_leaf_node(AST_IF); return IF; }
"else"                 { yylval = create_leaf_node(AST_ELSE); return ELSE; }
"while"                { yylval = create_leaf_node(AST_WHILE); return WHILE; }
"for"                  { yylval = create_leaf_node(AST_FOR); return FOR; }
"return"               { yylval = create_leaf_node(AST_RETURN); return RETURN; }


[0-9]*\.[0-9]+ {
    yylval = create_leaf_node(AST_FLOAT);
    yylval->float_val = atof(yytext);
    return FLOAT;
}

[0-9]*\.[0-9\.]+ {
    yylval = create_leaf_node(AST_FLOAT);
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "too many decimal points in number");
    yylval->float_val = 0.0f;
    return FLOAT;
}

[0-9]+ {
    yylval = create_leaf_node(AST_INT);
    if (yyleng > 1 && yytext[0] == '0') 
    {
        splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "decimal integer cannot start with 0");
    }
    else 
    {
        yylval->int_val = atoi(yytext);
    }
    return INT;
}

0[xX][0-9a-fA-F]+ {
    yylval = create_leaf_node(AST_INT);
    if (yyleng > 3 && yytext[2] == '0')
    {
        splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "hexadecimal integer cannot start with 0");
    }
    yylval->int_val = strtol(yytext, NULL, 16);
    return INT;
}

0[xX][0-9a-zA-Z]+ {
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "hexadecimal integer contains invalid characters");
    yylval = create_leaf_node(AST_INT);
    return INT;
}

'\\x[0-9a-fA-F]{2}' {
    yylval = create_leaf_node(AST_CHAR);
    yylval->val = (void *)strndup(yytext + 1, 4);
    return CHAR;
}

'\\x[0-9a-zA-Z]*' {
    yylval = create_leaf_node(AST_CHAR);
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "invalid form of octal-form character");
    yylval->val = (void *)strndup(yytext + 1, strlen(yytext) - 2);
    return CHAR;
}

[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval = create_leaf_node(AST_ID);
    yylval->val = (void *)strdup(yytext);
    return ID;
}

[0-9][a-zA-Z0-9_]* {
    yylval = create_leaf_node(AST_ID);
    yylval->val = (void *)strdup(yytext);       
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "identifier name cannot start with digits");
    return ID;
}

'\\[abefnrtv\\\'\"\?]' {
    yylval = create_leaf_node(AST_CHAR);
    yylval->val = (void *)strndup(yytext + 2, 1);
    return CHAR; 
}

'.' {
    yylval = create_leaf_node(AST_CHAR);
    yylval->val = (void *)strndup(yytext + 1, 1);
    return CHAR;
}


"="  { yylval = create_leaf_node(AST_ASSIGN); return ASSIGN; }

"&&" { yylval = create_leaf_node(AST_AND); return AND; }
"||" { yylval = create_leaf_node(AST_OR); return OR; }
"&"  { 
    yylval = create_leaf_node(AST_BITWISE_AND); 
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "unsupported lexeme");
    return BITWISE_AND; 
}
"|"  { 
    yylval = create_leaf_node(AST_BITWISE_OR); 
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "unsupported lexeme");
    return BITWISE_OR; 
}

"<"  { yylval = create_leaf_node(AST_LT); return LT; }
"<=" { yylval = create_leaf_node(AST_LE); return LE; }
">"  { yylval = create_leaf_node(AST_GT); return GT; }
">=" { yylval = create_leaf_node(AST_GE); return GE; }
"!=" { yylval = create_leaf_node(AST_NE); return NE; }
"==" { yylval = create_leaf_node(AST_EQ); return EQ; }
"."  { yylval = create_leaf_node(AST_DOT); return DOT; }
";"  { yylval = create_leaf_node(AST_SEMI); return SEMI; }
","  { yylval = create_leaf_node(AST_COMMA); return COMMA; }
"+"  { yylval = create_leaf_node(AST_PLUS); return PLUS; }
"-"  { yylval = create_leaf_node(AST_MINUS); return MINUS; }
"*"  { yylval = create_leaf_node(AST_MUL); return MUL; }
"/"  { yylval = create_leaf_node(AST_DIV); return DIV; }


"!"  { yylval = create_leaf_node(AST_NOT); return NOT; }
"{"  { yylval = create_leaf_node(AST_LC); return LC; }
"}"  { yylval = create_leaf_node(AST_RC); return RC; }
"("  { yylval = create_leaf_node(AST_LP); return LP; }
")"  { yylval = create_leaf_node(AST_RP); return RP; }
"["  { yylval = create_leaf_node(AST_LSB); return LSB; }
"]"  { yylval = create_leaf_node(AST_RSB); return RSB; }

"\n" { yycolno = 1; }
[ \r\t] {}

. {
    yylval = create_leaf_node(AST_ID);
    yylval->val = (void *)strdup(yytext);
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "unknown lexeme");
    return ID;
}
%%
