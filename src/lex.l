%option yylineno
%{
    #include "splcdef.h"
    #include "utils.h"
    #include "lut.h"
    #include "ast.h"

    #include "syntax.tab.h"

    #include <errno.h>
    #include <limits.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
    
    // TODO: define YY_USER_ACTION
    int yynewfile = 0;
    int yycolno = 1;

    #define YY_USER_ACTION \
        yylloc.first_line = (yynewfile == 1) ? 1 : yylloc.last_line; \
        yylloc.first_column = (yynewfile == 1) ? 1 : yylloc.last_column; \
        if (yynewfile == 1) { \
            yynewfile = 0; \
        } \
        yylloc.last_line = yylloc.first_line; \
        yylloc.last_column = yylloc.first_column; \
        for (int i = 0; yytext[i] != '\0'; ++i) { \
            if (yytext[i] == '\n') { \
                yylloc.last_line++; \
                yylloc.last_column = 1; \
                yycolno = 1; \
            } \
            else { \
                yylloc.last_column++; \
                yycolno++; \
            } \
        } \
        // yylloc.last_line = yylineno;
        // yylloc.last_column = yycolno + yyleng;
        // yycolno += yyleng;
    
    ast_node pproot = NULL;
    ast_node tmp = NULL;
%}

/* In cross-line comment */
%s IN_CL_COMMENT
/* In string */
%s IN_STRING
/* In preprocessor directive */
%s IN_PPD
/* Directive: Include */
%s IN_PPD_INCL
/* In preprocessor directive: define: test if name exists */
%s IN_MACRO_DEF_TEST
/* In preprocessor directive: define: content */
%s IN_MACRO_DEF

identifier [a-zA-Z_][a-zA-Z0-9_]*

/* Cross-line comment reference: http://westes.github.io/flex/manual/How-can-I-match-C_002dstyle-comments_003f.html */
%%

    /* =================== COMMENT =================== */
<INITIAL>"/*" {
    /* ignore comments */
    BEGIN(IN_CL_COMMENT);
}

<IN_CL_COMMENT>"*/" {
    BEGIN(INITIAL);
}

<IN_CL_COMMENT>[^\*]+ {
    // eat comment in chunks
}

<IN_CL_COMMENT>"*" {
    // eat the lone star
}
    
    /* =================== MACRO =================== */
<INITIAL># {
    BEGIN(IN_PPD);
}

<IN_PPD>"include" {
    BEGIN(IN_PPD_INCL);
}

<IN_PPD_INCL>[ ] {

}

<IN_PPD_INCL>\<([^\r\n]|(\\\r?\n))+\> {
    char *filename = strndup(yytext + 1, strlen(yytext) - 2);
    if (splc_enter_file(filename, SPLC_YY2LOC_CF_D(yylloc)) == 0)
    {
        BEGIN(INITIAL);
    }
    free(filename);
}

<IN_PPD_INCL>\"([^\r\n]|(\\\r?\n))+\" {
    char *filename = strndup(yytext + 1, strlen(yytext) - 2);
    if (splc_enter_file(filename, SPLC_YY2LOC_CF_D(yylloc)) == 0)
    {
        BEGIN(INITIAL);
    }
    free(filename);
}

<IN_PPD_INCL>\r?\n {
    BEGIN(INITIAL);
}

<IN_MACRO_DEF>. {
    splcwarn(SPLC_YY2LOC_CF_1_PNT_E(yylineno, yycolno),"unsupported preprocessor directive");
    BEGIN(INITIAL);
}

    /* =================== SINGLE-LINE COMMENT =================== */
<INITIAL>"//"([^\r\n]*(\\[\r]?\n)?)*\n {
    /* match single-line comments that may spread across lines by '\\' */
    /* Explanation: match the prefix "//", then: */
    /*     match any character except newline or "\\(\r, if any)\n" as the body, terminated by "\n" */
}

    /* =================== STRING =================== */
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

    /* =================== SPL: keywords =================== */
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

    /* =================== SPL: floating point =================== */
<INITIAL>[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {
    yylval = create_leaf_node(AST_FLOAT);
    yylval->float_val = strtof(yytext, NULL);
    if (errno == ERANGE)
    {
        errno = 0;
        splcwarn(SPLC_YY2LOC_CF_D(yylloc), "overflow in floating-point constant");
    }
    return FLOAT;
}

<INITIAL>[0-9]*\.([0-9]|[eE]|[-+]|[\.])+ {
    yylval = create_leaf_node(AST_FLOAT);
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "too many decimal points or exponential indicators");
    yylval->float_val = 0.0f;
    return FLOAT;
}

    /* =================== SPL: integers =================== */
<INITIAL>[0-9]+ {
    yylval = create_leaf_node(AST_INT);
    if (yyleng > 1 && yytext[0] == '0') 
    {
        splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "decimal integer cannot start with 0");
    }
    yylval->ulong_val = strtoul(yytext, NULL, 10);
    if (errno == ERANGE || yylval->ulong_val > (unsigned long)(-(long)INT_MIN))
    {
        errno = 0;
        splcwarn(SPLC_YY2LOC_CF_D(yylloc), "overflow in integer constant");
    }
    return INT;
}

<INITIAL>0[xX][0-9a-fA-F]{1,8} {
    yylval = create_leaf_node(AST_INT);
    if (yyleng > 3 && yytext[2] == '0')
    {
        splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "hexadecimal integer cannot start with 0");
    }
    yylval->ulong_val = strtoul(yytext, NULL, 16);
    if (errno == ERANGE)
    {
        errno = 0;
        splcwarn(SPLC_YY2LOC_CF_D(yylloc), "overflow in integer constant");
    }
    return INT;
}

    /* SPL: invalid hexadecimal integer */
<INITIAL>0[xX][0-9a-fA-F]{9,} {
    yylval = create_leaf_node(AST_INT);
    splcwarn(SPLC_YY2LOC_CF_D(yylloc), "overflow in integer constant");
    yylval->ulong_val = 0;
    return INT;
}

    /* =================== SPL: char =================== */
<INITIAL>0[xX][0-9a-zA-Z]+ {
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "hexadecimal integer contains invalid characters");
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
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "invalid form of octal-form character");
    yylval->val = (void *)strndup(yytext + 1, strlen(yytext) - 2);
    return CHAR;
}

    /* =================== SPL: ID =================== */
<INITIAL>{identifier} { 
    yylval = create_leaf_node(AST_ID);
    yylval->val = (void *)strdup(yytext);
    return ID;
}

<INITIAL>[0-9][a-zA-Z0-9_]* {
    yylval = create_leaf_node(AST_ID);
    yylval->val = (void *)strdup(yytext);       
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "identifier name cannot start with digits");
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


    /* =================== SPL: operators =================== */
<INITIAL>"="  { yylval = create_leaf_node(AST_ASSIGN); return ASSIGN; }

<INITIAL>"&&" { yylval = create_leaf_node(AST_AND); return AND; }
<INITIAL>"||" { yylval = create_leaf_node(AST_OR); return OR; }
<INITIAL>"&"  { 
    yylval = create_leaf_node(AST_BITWISE_AND); 
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "unsupported lexeme");
    return BITWISE_AND; 
}
<INITIAL>"|"  { 
    yylval = create_leaf_node(AST_BITWISE_OR); 
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "unsupported lexeme");
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



    /* =================== SPL: whitespace characters =================== */
<INITIAL>"\n" { }
<INITIAL>[ \r\t] { }

    /* =================== SPL: unknown lexemes =================== */
<INITIAL>. {
    yylval = create_leaf_node(AST_ID);
    yylval->val = (void *)strdup(yytext);
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "unknown lexeme");
    return ID;
}
%%

int yywrap()
{
    return splc_exit_file();
}