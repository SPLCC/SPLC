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
    /* Skip spaces */
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

<IN_PPD_INCL>. {
    splcwarn(SPLC_YY2LOC_CF_D(yylloc), "Unrecognized token in #include directive");
}

<IN_PPD_INCL>\r?\n {
    BEGIN(INITIAL);
}

<IN_MACRO_DEF>. {
    splcwarn(SPLC_YY2LOC_CF_D(yylloc), "unsupported preprocessor directive");
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
    yylval = create_leaf_node(AST_STR, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);
    return STR;
}

<IN_STRING>\\\r?\n {
    /* by std C definition, skip this and the newline character */
}

<IN_STRING>[^\n\r\"\\]+ {
    yylval = create_leaf_node(AST_STR, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);
    return STR;
}

<IN_STRING>\" {
    BEGIN(INITIAL);
}

    /* =================== SPL: keywords =================== */
<INITIAL>"int"|"float"|"char"  {
    yylval = create_leaf_node(AST_BUILTIN_TYPE, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);
    return TYPE;
}

<INITIAL>"struct"  { yylval = create_leaf_node(AST_STRUCT, SPLC_YY2LOC_CF_D(yylloc)); return STRUCT; }
<INITIAL>"if"      { yylval = create_leaf_node(AST_IF, SPLC_YY2LOC_CF_D(yylloc)); return IF; }
<INITIAL>"else"    { yylval = create_leaf_node(AST_ELSE, SPLC_YY2LOC_CF_D(yylloc)); return ELSE; }
<INITIAL>"while"   { yylval = create_leaf_node(AST_WHILE, SPLC_YY2LOC_CF_D(yylloc)); return WHILE; }
<INITIAL>"for"     { yylval = create_leaf_node(AST_FOR, SPLC_YY2LOC_CF_D(yylloc)); return FOR; }
<INITIAL>"return"  { yylval = create_leaf_node(AST_RETURN, SPLC_YY2LOC_CF_D(yylloc)); return RETURN; }

    /* =================== SPL: floating point =================== */
<INITIAL>[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {
    yylval = create_leaf_node(AST_FLOAT, SPLC_YY2LOC_CF_D(yylloc));
    yylval->float_val = strtof(yytext, NULL);
    if (errno == ERANGE)
    {
        errno = 0;
        splcwarn(SPLC_YY2LOC_CF_D(yylloc), "overflow in floating-point constant");
    }
    return FLOAT;
}

<INITIAL>[0-9]*\.([0-9]|[eE]|[-+]|[\.])+ {
    yylval = create_leaf_node(AST_FLOAT, SPLC_YY2LOC_CF_D(yylloc));
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "too many decimal points or exponential indicators");
    yylval->float_val = 0.0f;
    return FLOAT;
}

    /* =================== SPL: integers =================== */
<INITIAL>[0-9]+ {
    yylval = create_leaf_node(AST_INT, SPLC_YY2LOC_CF_D(yylloc));
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
    yylval = create_leaf_node(AST_INT, SPLC_YY2LOC_CF_D(yylloc));
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
    yylval = create_leaf_node(AST_INT, SPLC_YY2LOC_CF_D(yylloc));
    splcwarn(SPLC_YY2LOC_CF_D(yylloc), "overflow in integer constant");
    yylval->ulong_val = 0;
    return INT;
}

    /* =================== SPL: char =================== */
<INITIAL>0[xX][0-9a-zA-Z]+ {
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "hexadecimal integer contains invalid characters");
    yylval = create_leaf_node(AST_INT, SPLC_YY2LOC_CF_D(yylloc));
    return INT;
}

<INITIAL>'\\x[0-9a-fA-F]{2}' {
    yylval = create_leaf_node(AST_CHAR, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strndup(yytext + 1, 4);
    return CHAR;
}

<INITIAL>'\\x[0-9a-zA-Z]*' {
    yylval = create_leaf_node(AST_CHAR, SPLC_YY2LOC_CF_D(yylloc));
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "invalid form of octal-form character");
    yylval->val = (void *)strndup(yytext + 1, strlen(yytext) - 2);
    return CHAR;
}

    /* =================== SPL: ID =================== */
<INITIAL>{identifier} { 
    yylval = create_leaf_node(AST_ID, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);
    return ID;
}

<INITIAL>[0-9][a-zA-Z0-9_]* {
    yylval = create_leaf_node(AST_ID, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);       
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "identifier name cannot start with digits");
    return ID;
}

<INITIAL>'\\[abefnrtv\\\'\"\?]' {
    yylval = create_leaf_node(AST_CHAR, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strndup(yytext + 2, 1);
    return CHAR; 
}

<INITIAL>'.' {
    yylval = create_leaf_node(AST_CHAR, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strndup(yytext + 1, 1);
    return CHAR;
}


    /* =================== SPL: operators =================== */
<INITIAL>"="  { yylval = create_leaf_node(AST_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return ASSIGN; }

<INITIAL>"&&" { yylval = create_leaf_node(AST_AND, SPLC_YY2LOC_CF_D(yylloc)); return AND; }
<INITIAL>"||" { yylval = create_leaf_node(AST_OR, SPLC_YY2LOC_CF_D(yylloc)); return OR; }
<INITIAL>"&"  { 
    yylval = create_leaf_node(AST_BITWISE_AND, SPLC_YY2LOC_CF_D(yylloc)); 
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "unsupported lexeme");
    return BITWISE_AND; 
}
<INITIAL>"|"  { 
    yylval = create_leaf_node(AST_BITWISE_OR, SPLC_YY2LOC_CF_D(yylloc)); 
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "unsupported lexeme");
    return BITWISE_OR; 
}

<INITIAL>"<"  { yylval = create_leaf_node(AST_LT, SPLC_YY2LOC_CF_D(yylloc)); return LT; }
<INITIAL>"<=" { yylval = create_leaf_node(AST_LE, SPLC_YY2LOC_CF_D(yylloc)); return LE; }
<INITIAL>">"  { yylval = create_leaf_node(AST_GT, SPLC_YY2LOC_CF_D(yylloc)); return GT; }
<INITIAL>">=" { yylval = create_leaf_node(AST_GE, SPLC_YY2LOC_CF_D(yylloc)); return GE; }
<INITIAL>"!=" { yylval = create_leaf_node(AST_NE, SPLC_YY2LOC_CF_D(yylloc)); return NE; }
<INITIAL>"==" { yylval = create_leaf_node(AST_EQ, SPLC_YY2LOC_CF_D(yylloc)); return EQ; }
<INITIAL>"."  { yylval = create_leaf_node(AST_DOT, SPLC_YY2LOC_CF_D(yylloc)); return DOT; }
<INITIAL>";"  { yylval = create_leaf_node(AST_SEMI, SPLC_YY2LOC_CF_D(yylloc)); return SEMI; }
<INITIAL>","  { yylval = create_leaf_node(AST_COMMA, SPLC_YY2LOC_CF_D(yylloc)); return COMMA; }
<INITIAL>"+"  { yylval = create_leaf_node(AST_PLUS, SPLC_YY2LOC_CF_D(yylloc)); return PLUS; }
<INITIAL>"-"  { yylval = create_leaf_node(AST_MINUS, SPLC_YY2LOC_CF_D(yylloc)); return MINUS; }
<INITIAL>"*"  { yylval = create_leaf_node(AST_MUL, SPLC_YY2LOC_CF_D(yylloc)); return MUL; }
<INITIAL>"/"  { yylval = create_leaf_node(AST_DIV, SPLC_YY2LOC_CF_D(yylloc)); return DIV; }


<INITIAL>"!"  { yylval = create_leaf_node(AST_NOT, SPLC_YY2LOC_CF_D(yylloc)); return NOT; }
<INITIAL>"{"  { yylval = create_leaf_node(AST_LC, SPLC_YY2LOC_CF_D(yylloc)); return LC; }
<INITIAL>"}"  { yylval = create_leaf_node(AST_RC, SPLC_YY2LOC_CF_D(yylloc)); return RC; }
<INITIAL>"("  { yylval = create_leaf_node(AST_LP, SPLC_YY2LOC_CF_D(yylloc)); return LP; }
<INITIAL>")"  { yylval = create_leaf_node(AST_RP, SPLC_YY2LOC_CF_D(yylloc)); return RP; }
<INITIAL>"["  { yylval = create_leaf_node(AST_LSB, SPLC_YY2LOC_CF_D(yylloc)); return LSB; }
<INITIAL>"]"  { yylval = create_leaf_node(AST_RSB, SPLC_YY2LOC_CF_D(yylloc)); return RSB; }



    /* =================== SPL: whitespace characters =================== */
<INITIAL>"\n" { }
<INITIAL>[ \r\t] { }

    /* =================== SPL: unknown lexemes =================== */
<INITIAL>. {
    yylval = create_leaf_node(AST_ID, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);
    splcerror(SPLC_ERR_A, SPLC_YY2LOC_CF_D(yylloc), "unknown lexeme");
    return ID;
}
%%

/* This function defines what to do when EOF has been reached */
int yywrap()
{
    int op = splc_exit_file();
    if (op == 0)
    {
        BEGIN(IN_PPD_INCL);
    }
    return op;
}