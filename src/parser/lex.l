%option yylineno
%{
    #include "spldef.h"
    #include "utils.h"
    #include "lut.h"
    #include "ast.h"

    #include "parser_syntax.tab.h"

    #include <errno.h>
    #include <limits.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
    // TODO: define YY_USER_ACTION
    int yycolno = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line; \
        yylloc.first_column = yylloc.last_column; \
        for (int i = 0; yytext[i] != '\0'; ++i) { \
            if (yytext[i] == '\n') { \
                yylloc.last_line++; \
                yylloc.last_column = 1; \
            } \
            else { \
                yylloc.last_column++; \
            } \
        } \
        // yylloc.last_line = yylineno;
        // yylloc.last_column = yycolno + yyleng;
        // yycolno += yyleng;
%}

/* In cross-line comment */
%s IN_CL_COMMENT
/* In string */
%s IN_STRING

/* Cross-line comment reference: http://westes.github.io/flex/manual/How-can-I-match-C_002dstyle-comments_003f.html */
%%

<INITIAL>"/*" {
    /* ignore comments */
    BEGIN(IN_CL_COMMENT);
}

<IN_CL_COMMENT>"*/" {
    BEGIN(INITIAL);
}

<IN_CL_COMMENT>[^*]+ {
    // eat comment in chunks
}

<IN_CL_COMMENT>"*" {
    // eat the lone star
}

<INITIAL>\" {
    BEGIN(IN_STRING);
}

<IN_STRING>\\[abefnrtv\\\'\"\?] {
    yylval = create_leaf_node(AST_STR);
    yylval->val = (void *)strdup(yytext);
    return STR;
}

<IN_STRING>\\\r?\n {
    /* by std C definition, skip this and the newline character */
}

<IN_STRING>[^\n\r\"\\]+ {
    yylval = create_leaf_node(AST_STR);
    yylval->val = (void *)strdup(yytext);
    return STR;
}

<IN_STRING>\" {
    BEGIN(INITIAL);
}

<INITIAL>"//"([^\r\n]*(\\[\r]?\n)?)*\n {
    /* match single-line comments that may spread across lines by '\\' */
    /* Explanation: match the prefix "//", then: */
    /*     match any character except newline or "\\(\r, if any)\n" as the body, terminated by "\n" */
}

<INITIAL>"int"|"float"|"char"  {
    yylval = create_leaf_node(AST_BUILTIN_TYPE);
    yylval->val = (void *)strdup(yytext);
    return TYPE;
}

<INITIAL>"struct"  { yylval = create_leaf_node(AST_STRUCT); return STRUCT; }
<INITIAL>"if"      { yylval = create_leaf_node(AST_IF); return IF; }
<INITIAL>"else"    { yylval = create_leaf_node(AST_ELSE); return ELSE; }
<INITIAL>"while"   { yylval = create_leaf_node(AST_WHILE); return WHILE; }
<INITIAL>"for"     { yylval = create_leaf_node(AST_FOR); return FOR; }
<INITIAL>"return"  { yylval = create_leaf_node(AST_RETURN); return RETURN; }

<INITIAL>[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {
    yylval = create_leaf_node(AST_FLOAT);
    yylval->float_val = strtof(yytext, NULL);
    if (errno == ERANGE)
    {
        errno = 0;
        splwarn(yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "overflow in floating-point constant");
    }
    return FLOAT;
}

<INITIAL>[0-9]*\.([0-9]|[eE]|[-+]|[\.])+ {
    yylval = create_leaf_node(AST_FLOAT);
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "too many decimal points or exponential indicators");
    yylval->float_val = 0.0f;
    return FLOAT;
}

<INITIAL>[0-9]+ {
    yylval = create_leaf_node(AST_INT);
    if (yyleng > 1 && yytext[0] == '0') 
    {
        splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "decimal integer cannot start with 0");
    }
    yylval->ulong_val = strtoul(yytext, NULL, 10);
    if (errno == ERANGE || yylval->ulong_val > (unsigned long)(-(long)INT_MIN))
    {
        errno = 0;
        splwarn(yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "overflow in integer constant");
    }
    return INT;
}

<INITIAL>0[xX][0-9a-fA-F]{1,8} {
    yylval = create_leaf_node(AST_INT);
    if (yyleng > 3 && yytext[2] == '0')
    {
        splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "hexadecimal integer cannot start with 0");
    }
    yylval->ulong_val = strtoul(yytext, NULL, 16);
    if (errno == ERANGE)
    {
        errno = 0;
        splwarn(yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "overflow in integer constant");
    }
    return INT;
}

<INITIAL>0[xX][0-9a-fA-F]{9,} {
    yylval = create_leaf_node(AST_INT);
    splwarn(yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "overflow in integer constant");
    yylval->ulong_val = 0;
    return INT;
}

<INITIAL>0[xX][0-9a-zA-Z]+ {
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "hexadecimal integer contains invalid characters");
    yylval = create_leaf_node(AST_INT);
    return INT;
}

<INITIAL>'\\x[0-9a-fA-F]{2}' {
    yylval = create_leaf_node(AST_CHAR);
    yylval->val = (void *)strndup(yytext + 1, 4);
    return CHAR;
}

<INITIAL>'\\x[0-9a-zA-Z]*' {
    yylval = create_leaf_node(AST_CHAR);
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "invalid form of octal-form character");
    yylval->val = (void *)strndup(yytext + 1, strlen(yytext) - 2);
    return CHAR;
}

<INITIAL>[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval = create_leaf_node(AST_ID);
    yylval->val = (void *)strdup(yytext);
    return ID;
}

<INITIAL>[0-9][a-zA-Z0-9_]* {
    yylval = create_leaf_node(AST_ID);
    yylval->val = (void *)strdup(yytext);       
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "identifier name cannot start with digits");
    return ID;
}

<INITIAL>'\\[abefnrtv\\\'\"\?]' {
    yylval = create_leaf_node(AST_CHAR);
    yylval->val = (void *)strndup(yytext + 2, 1);
    return CHAR; 
}

<INITIAL>'.' {
    yylval = create_leaf_node(AST_CHAR);
    yylval->val = (void *)strndup(yytext + 1, 1);
    return CHAR;
}


<INITIAL>"="  { yylval = create_leaf_node(AST_ASSIGN); return ASSIGN; }

<INITIAL>"&&" { yylval = create_leaf_node(AST_AND); return AND; }
<INITIAL>"||" { yylval = create_leaf_node(AST_OR); return OR; }
<INITIAL>"&"  { 
    yylval = create_leaf_node(AST_BITWISE_AND); 
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "unsupported lexeme");
    return BITWISE_AND; 
}
<INITIAL>"|"  { 
    yylval = create_leaf_node(AST_BITWISE_OR); 
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "unsupported lexeme");
    return BITWISE_OR; 
}

<INITIAL>"<"  { yylval = create_leaf_node(AST_LT); return LT; }
<INITIAL>"<=" { yylval = create_leaf_node(AST_LE); return LE; }
<INITIAL>">"  { yylval = create_leaf_node(AST_GT); return GT; }
<INITIAL>">=" { yylval = create_leaf_node(AST_GE); return GE; }
<INITIAL>"!=" { yylval = create_leaf_node(AST_NE); return NE; }
<INITIAL>"==" { yylval = create_leaf_node(AST_EQ); return EQ; }
<INITIAL>"."  { yylval = create_leaf_node(AST_DOT); return DOT; }
<INITIAL>";"  { yylval = create_leaf_node(AST_SEMI); return SEMI; }
<INITIAL>","  { yylval = create_leaf_node(AST_COMMA); return COMMA; }
<INITIAL>"+"  { yylval = create_leaf_node(AST_PLUS); return PLUS; }
<INITIAL>"-"  { yylval = create_leaf_node(AST_MINUS); return MINUS; }
<INITIAL>"*"  { yylval = create_leaf_node(AST_MUL); return MUL; }
<INITIAL>"/"  { yylval = create_leaf_node(AST_DIV); return DIV; }


<INITIAL>"!"  { yylval = create_leaf_node(AST_NOT); return NOT; }
<INITIAL>"{"  { yylval = create_leaf_node(AST_LC); return LC; }
<INITIAL>"}"  { yylval = create_leaf_node(AST_RC); return RC; }
<INITIAL>"("  { yylval = create_leaf_node(AST_LP); return LP; }
<INITIAL>")"  { yylval = create_leaf_node(AST_RP); return RP; }
<INITIAL>"["  { yylval = create_leaf_node(AST_LSB); return LSB; }
<INITIAL>"]"  { yylval = create_leaf_node(AST_RSB); return RSB; }



<INITIAL>"\n" { }
<INITIAL>[ \r\t] { }

<INITIAL>. {
    yylval = create_leaf_node(AST_ID);
    yylval->val = (void *)strdup(yytext);
    splerror(SPLERR_A, yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, "unknown lexeme");
    return ID;
}
%%
