%option yylineno
%option stack
%{
    #include "splcdef.h"
    #include "utils.h"
    #include "lut.h"
    #include "ast.h"
    #include "preprocessor.h"
    #include "semantics.h"

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

    char *ppd_macro_name = NULL;
    splc_loc ppd_macro_pt = SPLC_INVALID_LOC;
%}

/* In cross-line comment */
%s IN_CL_COMMENT
/* In string */
%s IN_STRING
/* Preprocessor Directive */
%s IN_PPD
/* Preprocessor Directive: include */
%s IN_PPD_INCL
/* Preprocessor Directive: define */
%s IN_PPD_DEFINE
/* Preprocessor Directive: define body */
%s IN_PPD_DEFINE_BODY

identifier [a-zA-Z_][a-zA-Z0-9_]*

/* Cross-line comment reference: http://westes.github.io/flex/manual/How-can-I-match-C_002dstyle-comments_003f.html */
%%

    /* =================== COMMENT =================== */
"/*" {
    /* ignore comments */
    yy_push_state(IN_CL_COMMENT);
}

<IN_CL_COMMENT>"*/" {
    yy_pop_state();
}

<IN_CL_COMMENT>[^\*]+ {
    // eat comment in chunks
}

<IN_CL_COMMENT>"*" {
    // eat the lone star
}

    /* <IN_CL_COMMENT><<EOF>> { */
        /* SPLC_ERROR(SPLM_ERR_UNIV, SPLC_YY2LOC_CF_D(yylloc), "unclosed comment"); */
        /* BEGIN(INITIAL); */
    /* } */

    /* =================== SINGLE-LINE COMMENT =================== */
"//"([^\r\n]*(\\[\r]?\n)?)*\n {
    /* match single-line comments that may spread across lines by '\\' */
    /* Explanation: match the prefix "//", then: */
    /*     match any character except newline or "\\(\r, if any)\n" as the body, terminated by "\n" */
}
    
    /* ============== Preprocessor Directive Translation ============== */
<INITIAL># {
    yy_push_state(IN_PPD);
}

    /* PPD Dispatch: include */
<IN_PPD>"include" {
    yy_pop_state();
    yy_push_state(IN_PPD_INCL);
}

    /* PPD Dispatch: define */
<IN_PPD>"define" {
    yy_pop_state();
    yy_push_state(IN_PPD_DEFINE);
}

    /* PPD Dispatch: if */
    /* <IN_PPD>"if" { */
    /*     BEGIN(IN_PPD_INCL); */
    /* } */

    /* PPD Dispatch: ifdef */
    /*<IN_PPD>"ifdef" { */
    /*    BEGIN(IN_PPD_INCL); */
    /*} */

    /* PPD Dispatch: ifndef */
    /* <IN_PPD>"ifndef" { */
    /*     BEGIN(IN_PPD_INCL); */
    /* } */

<IN_PPD>. {

}

<IN_PPD>\\\r?\n {
    /* allow line break */
}

<IN_PPD>\r?\n {
    yy_pop_state();
}

    /* ============== PPD: #include directive ============== */
<IN_PPD_INCL>[ ] {
    /* Skip spaces */
}

<IN_PPD_INCL>\<([^\r\n]|(\\\r?\n))+\> {
    char *filename = strndup(yytext + 1, strlen(yytext) - 2);
    if (splc_push_file_buffer(filename, SPLC_YY2LOC_CF_D(yylloc)) == 0)
    {
        yy_push_state(INITIAL);
    }
    free(filename);
}

<IN_PPD_INCL>\"([^\r\n]|(\\\r?\n))+\" {
    char *filename = strndup(yytext + 1, strlen(yytext) - 2);
    if (splc_push_file_buffer(filename, SPLC_YY2LOC_CF_D(yylloc)) == 0)
    {
        yy_push_state(INITIAL);
    }
    free(filename);
}

<IN_PPD_INCL>. {
    SPLC_WARN(SPLM_Wuniv, SPLC_YY2LOC_CF_D(yylloc), "extra tokens at end of #include directive");
}

<IN_PPD_INCL>\r?\n {
    yy_pop_state();
}

    /* ============== PPD: #define directive ============== */
<IN_PPD_DEFINE>{identifier} {
    ppd_macro_pt = SPLC_YY2LOC_CF_D(yylloc);
    ppd_macro_name = strdup(yytext);
    SPLC_FWARN(SPLM_Wuniv, ppd_macro_pt, "detected macro definition '\033[1m%s\033[0m'. This is an experimental feature.", ppd_macro_name);
    yy_pop_state();
    yy_push_state(IN_PPD_DEFINE_BODY);
}

<IN_PPD_DEFINE>. {

}

<IN_PPD_DEFINE>\\\r?\n {

}

<IN_PPD_DEFINE>\r?\n {
    yy_pop_state();
}

<IN_PPD_DEFINE_BODY>([^\r\n]*(\\[\r]?\n)?)* {
    char *expanded = get_expanded_macro_content(yytext);
    lut_insert(current_trans_unit->global_symtable, ppd_macro_name, SPLE_MACRO_VAR, SPLE_NULL, NULL, expanded, NULL, SPLC_YY2LOC_CF_D(yylloc));
    SPLC_FDIAG("registered identifier %s", ppd_macro_name);
    free(expanded);
}

<IN_PPD_DEFINE_BODY>. {

}

<IN_PPD_DEFINE_BODY>\r?\n {
    free(ppd_macro_name);
    ppd_macro_name = NULL;
    yy_pop_state();
}

    /* =================== STRING =================== */
<INITIAL>\" {
    yy_push_state(IN_STRING);
}

<INITIAL>\"\" {
    yylval = ast_create_leaf_node(SPLT_STR_UNIT, SPLC_YY2LOC_CF_D(yylloc));
    char *text = (char *)malloc(sizeof(char));
    text[0] = '\0';
    yylval->val = text;
    return STR_UNIT;
}

    /* Match escape characters */
<IN_STRING>\\[abefnrtv\\\'\"\?] {
    yylval = ast_create_leaf_node(SPLT_STR_UNIT, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);
    return STR_UNIT;
}

    /* Allow line break */
<IN_STRING>\\\r?\n {
    /* by std C definition, skip this and the newline character */
}

    /* Match line break */
<IN_STRING>[^\n\r\"\\]+ {
    yylval = ast_create_leaf_node(SPLT_STR_UNIT, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);
    return STR_UNIT;
}

    /* Match line break */
<IN_STRING>\\[0-9]{1,3} {
    yylval = ast_create_leaf_node(SPLT_STR_UNIT, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);
    return STR_UNIT;
}

    /* Match escape characters */
<IN_STRING>\\x[0-9a-fA-F]{2,3} {
    yylval = ast_create_leaf_node(SPLT_STR_UNIT, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);
    return STR_UNIT;
}

<IN_STRING>\" {
    yy_pop_state();
}

<IN_STRING><<EOF>> {
    SPLC_ERROR(SPLM_ERR_UNIV, SPLC_YY2LOC_CF_D(yylloc), "unclosed string literal");
    yy_pop_state();
}

    /* =================== SPL: keywords =================== */
<INITIAL>"enum"     { yylval = ast_create_leaf_node(SPLT_KWD_ENUM, SPLC_YY2LOC_CF_D(yylloc)); return KWD_ENUM; }
<INITIAL>"struct"   { yylval = ast_create_leaf_node(SPLT_KWD_STRUCT, SPLC_YY2LOC_CF_D(yylloc)); return KWD_STRUCT; }
<INITIAL>"union"    { yylval = ast_create_leaf_node(SPLT_KWD_UNION, SPLC_YY2LOC_CF_D(yylloc)); return KWD_UNION; }

<INITIAL>"auto"     { yylval = ast_create_leaf_node(SPLT_AUTO, SPLC_YY2LOC_CF_D(yylloc)); return AUTO; }
<INITIAL>"extern"   { yylval = ast_create_leaf_node(SPLT_EXTERN, SPLC_YY2LOC_CF_D(yylloc)); return EXTERN; }
<INITIAL>"register" { yylval = ast_create_leaf_node(SPLT_REGISTER, SPLC_YY2LOC_CF_D(yylloc)); return REGISTER; }
<INITIAL>"static"   { yylval = ast_create_leaf_node(SPLT_STATIC, SPLC_YY2LOC_CF_D(yylloc)); return STATIC; }
<INITIAL>"typedef"  { yylval = ast_create_leaf_node(SPLT_TYPEDEF, SPLC_YY2LOC_CF_D(yylloc)); return TYPEDEF; }

<INITIAL>"const"    { yylval = ast_create_leaf_node(SPLT_KWD_CONST, SPLC_YY2LOC_CF_D(yylloc)); return KWD_CONST; }
<INITIAL>"restrict" { yylval = ast_create_leaf_node(SPLT_RESTRICT, SPLC_YY2LOC_CF_D(yylloc)); return RESTRICT; }
<INITIAL>"volatile" { yylval = ast_create_leaf_node(SPLT_VOLATILE, SPLC_YY2LOC_CF_D(yylloc)); return VOLATILE; }

<INITIAL>"..."      { yylval = ast_create_leaf_node(SPLT_ELLIPSIS, SPLC_YY2LOC_CF_D(yylloc)); return ELLIPSIS; }

<INITIAL>"while"    { yylval = ast_create_leaf_node(SPLT_WHILE, SPLC_YY2LOC_CF_D(yylloc)); return WHILE; }
<INITIAL>"for"      { yylval = ast_create_leaf_node(SPLT_FOR, SPLC_YY2LOC_CF_D(yylloc)); return FOR; }
<INITIAL>"do"       { yylval = ast_create_leaf_node(SPLT_DO, SPLC_YY2LOC_CF_D(yylloc)); return DO; }

<INITIAL>"if"       { yylval = ast_create_leaf_node(SPLT_IF, SPLC_YY2LOC_CF_D(yylloc)); return IF; }
<INITIAL>"else"     { yylval = ast_create_leaf_node(SPLT_ELSE, SPLC_YY2LOC_CF_D(yylloc)); return ELSE; }
<INITIAL>"switch"   { yylval = ast_create_leaf_node(SPLT_SWITCH, SPLC_YY2LOC_CF_D(yylloc)); return SWITCH; }
<INITIAL>"default"  { yylval = ast_create_leaf_node(SPLT_DEFAULT, SPLC_YY2LOC_CF_D(yylloc)); return DEFAULT; }
<INITIAL>"case"     { yylval = ast_create_leaf_node(SPLT_CASE, SPLC_YY2LOC_CF_D(yylloc)); return CASE; }

<INITIAL>"goto"     { yylval = ast_create_leaf_node(SPLT_GOTO, SPLC_YY2LOC_CF_D(yylloc)); return GOTO; }
<INITIAL>"continue" { yylval = ast_create_leaf_node(SPLT_CONTINUE, SPLC_YY2LOC_CF_D(yylloc)); return CONTINUE; }
<INITIAL>"break"    { yylval = ast_create_leaf_node(SPLT_BREAK, SPLC_YY2LOC_CF_D(yylloc)); return BREAK; }
<INITIAL>"return"   { yylval = ast_create_leaf_node(SPLT_RETURN, SPLC_YY2LOC_CF_D(yylloc)); return RETURN; }

<INITIAL>"sizeof"   { yylval = ast_create_leaf_node(SPLT_SIZEOF, SPLC_YY2LOC_CF_D(yylloc)); return SIZEOF; }

<INITIAL>"void"     { yylval = ast_create_leaf_node(SPLT_TYPE_VOID, SPLC_YY2LOC_CF_D(yylloc)); return TYPE_VOID; }
<INITIAL>"int"      { yylval = ast_create_leaf_node(SPLT_TYPE_INT, SPLC_YY2LOC_CF_D(yylloc)); return TYPE_INT; }
<INITIAL>"signed"   { yylval = ast_create_leaf_node(SPLT_TYPE_SIGNED, SPLC_YY2LOC_CF_D(yylloc)); return TYPE_SIGNED; }
<INITIAL>"unsigned" { yylval = ast_create_leaf_node(SPLT_TYPE_UNSIGNED, SPLC_YY2LOC_CF_D(yylloc)); return TYPE_UNSIGNED; }
<INITIAL>"long"     { yylval = ast_create_leaf_node(SPLT_TYPE_LONG, SPLC_YY2LOC_CF_D(yylloc)); return TYPE_LONG; }
<INITIAL>"float"    { yylval = ast_create_leaf_node(SPLT_TYPE_FLOAT, SPLC_YY2LOC_CF_D(yylloc)); return TYPE_FLOAT; }
<INITIAL>"char"     { yylval = ast_create_leaf_node(SPLT_TYPE_CHAR, SPLC_YY2LOC_CF_D(yylloc)); return TYPE_CHAR; }

    /* =================== SPL: floating point =================== */
<INITIAL>[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {
    yylval = ast_create_leaf_node(SPLT_LTR_FLOAT, SPLC_YY2LOC_CF_D(yylloc));
    yylval->float_val = strtof(yytext, NULL);
    if (errno == ERANGE)
    {
        errno = 0;
        SPLC_WARN(SPLM_Woverflow, SPLC_YY2LOC_CF_D(yylloc), "overflow in floating-point constant");
    }
    return LTR_FLOAT;
}

<INITIAL>[0-9]*\.[0-9]+([eE]|[-+]|[\.])+ {
    yylval = ast_create_leaf_node(SPLT_LTR_FLOAT, SPLC_YY2LOC_CF_D(yylloc));
    SPLC_ERROR(SPLM_ERR_SYN_A, SPLC_YY2LOC_CF_D(yylloc), "too many decimal points or exponential indicators");
    yylval->float_val = 0.0f;
    return LTR_FLOAT;
}

    /* =================== SPL: integers =================== */
<INITIAL>[0-9]+ {
    yylval = ast_create_leaf_node(SPLT_LTR_INT, SPLC_YY2LOC_CF_D(yylloc));
    if (yyleng > 1 && yytext[0] == '0') 
    {
        SPLC_ERROR(SPLM_ERR_SYN_A, SPLC_YY2LOC_CF_D(yylloc), "decimal integer cannot start with 0");
    }
    yylval->ull_val = strtoul(yytext, NULL, 10);
    if (errno == ERANGE || yylval->ull_val > (unsigned long)(-(long)INT_MIN))
    {
        errno = 0;
        SPLC_WARN(SPLM_Woverflow, SPLC_YY2LOC_CF_D(yylloc), "overflow in integer constant");
    }
    return LTR_INT;
}

<INITIAL>0[xX][0-9a-fA-F]{1,8} {
    yylval = ast_create_leaf_node(SPLT_LTR_INT, SPLC_YY2LOC_CF_D(yylloc));
    if (yyleng > 3 && yytext[2] == '0')
    {
        SPLC_ERROR(SPLM_ERR_SYN_A, SPLC_YY2LOC_CF_D(yylloc), "hexadecimal integer cannot start with 0");
    }
    yylval->ull_val = strtoul(yytext, NULL, 16);
    if (errno == ERANGE)
    {
        errno = 0;
        SPLC_WARN(SPLM_Woverflow, SPLC_YY2LOC_CF_D(yylloc), "overflow in integer constant");
    }
    return LTR_INT;
}

    /* SPL: invalid hexadecimal integer */
<INITIAL>0[xX][0-9a-fA-F]{9,} {
    yylval = ast_create_leaf_node(SPLT_LTR_INT, SPLC_YY2LOC_CF_D(yylloc));
    SPLC_WARN(SPLM_Woverflow, SPLC_YY2LOC_CF_D(yylloc), "overflow in integer constant");
    yylval->ull_val = 0;
    return LTR_INT;
}

    /* =================== SPL: char =================== */
<INITIAL>0[xX][0-9a-zA-Z]+ {
    SPLC_ERROR(SPLM_ERR_SYN_A, SPLC_YY2LOC_CF_D(yylloc), "hexadecimal integer contains invalid characters");
    yylval = ast_create_leaf_node(SPLT_LTR_INT, SPLC_YY2LOC_CF_D(yylloc));
    return LTR_INT;
}

<INITIAL>'\\x[0-9a-fA-F]{2}' {
    yylval = ast_create_leaf_node(SPLT_LTR_CHAR, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strndup(yytext + 1, 4);
    return LTR_CHAR;
}

<INITIAL>'\\x[0-9a-zA-Z]*' {
    yylval = ast_create_leaf_node(SPLT_LTR_CHAR, SPLC_YY2LOC_CF_D(yylloc));
    SPLC_ERROR(SPLM_ERR_SYN_A, SPLC_YY2LOC_CF_D(yylloc), "invalid form of octal-form character");
    yylval->val = (void *)strndup(yytext + 1, strlen(yytext) - 2);
    return LTR_CHAR;
}

    /* =================== SPL: ID =================== */
<INITIAL>{identifier} {
    /* the only valid identifier entry */
    char *textcopy = strdup(yytext);
    // EXPERIMENTAL
    lut_entry ent = NULL;
    if ((ent = lut_find(current_trans_unit->global_symtable, textcopy, SPLE_MACRO_VAR)) != NULL && !buffer_stack_check_node_exists(ent->id))
    {
        free(textcopy);
        yy_push_state(INITIAL);
        splc_push_char_buffer(ent->id, SPLC_YY2LOC_CF_D(yylloc), ent->content);
    }
    else
    {
        if (ent != NULL && buffer_stack_check_node_exists(ent->id))
        {
            SPLC_FWARN(SPLM_Wuniv, SPLC_YY2LOC_CF_D(yylloc), "repeated expansion of macro \033[1m%s\033[0m is forbidden and will be ignored.", ent->id);
        }
        if (sem_test_typedef_name(textcopy))
        {
            yylval = ast_create_leaf_node(SPLT_TYPEDEF_NAME, SPLC_YY2LOC_CF_D(yylloc));
            yylval->val = textcopy;
            return TYPEDEF_NAME;
        }
        else
        {
            yylval = ast_create_leaf_node(SPLT_ID, SPLC_YY2LOC_CF_D(yylloc));
            yylval->val = textcopy;
            return ID;
        }
    }
}

<INITIAL>[0-9][a-zA-Z0-9_]* {
    yylval = ast_create_leaf_node(SPLT_ID, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);       
    SPLC_ERROR(SPLM_ERR_SYN_A, SPLC_YY2LOC_CF_D(yylloc), "identifier name cannot start with digits");
    return ID;
}

<INITIAL>'\\[abefnrtv\\\'\"\?]' {
    yylval = ast_create_leaf_node(SPLT_LTR_CHAR, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strndup(yytext + 2, 1);
    return LTR_CHAR; 
}

<INITIAL>'\\([abefnrtv\\\'\"\?]|[0-9]{1,3})' {
    yylval = ast_create_leaf_node(SPLT_LTR_CHAR, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strndup(yytext + 2, 1);
    return LTR_CHAR; 
}

<INITIAL>'.' {
    yylval = ast_create_leaf_node(SPLT_LTR_CHAR, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strndup(yytext + 1, 1);
    return LTR_CHAR;
}


    /* =================== SPL: punctuators =================== */
<INITIAL>";"  { yylval = ast_create_leaf_node(SPLT_SEMI, SPLC_YY2LOC_CF_D(yylloc)); return SEMI; }
<INITIAL>","  { yylval = ast_create_leaf_node(SPLT_COMMA, SPLC_YY2LOC_CF_D(yylloc)); return COMMA; }
<INITIAL>"?"  { yylval = ast_create_leaf_node(SPLT_QM, SPLC_YY2LOC_CF_D(yylloc)); return QM; }
<INITIAL>":"  { yylval = ast_create_leaf_node(SPLT_COLON, SPLC_YY2LOC_CF_D(yylloc)); return COLON; }

<INITIAL>"="  { yylval = ast_create_leaf_node(SPLT_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return ASSIGN; }
<INITIAL>"*="  { yylval = ast_create_leaf_node(SPLT_MUL_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return MUL_ASSIGN; }
<INITIAL>"/="  { yylval = ast_create_leaf_node(SPLT_DIV_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return DIV_ASSIGN; }
<INITIAL>"%="  { yylval = ast_create_leaf_node(SPLT_MOD_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return MOD_ASSIGN; }
<INITIAL>"+="  { yylval = ast_create_leaf_node(SPLT_PLUS_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return PLUS_ASSIGN; }
<INITIAL>"-="  { yylval = ast_create_leaf_node(SPLT_MINUS_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return MINUS_ASSIGN; }
<INITIAL>"<<="  { yylval = ast_create_leaf_node(SPLT_LSHIFT_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return LSHIFT_ASSIGN; }
<INITIAL>">>="  { yylval = ast_create_leaf_node(SPLT_RSHIFT_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return RSHIFT_ASSIGN; }
<INITIAL>"&="  { yylval = ast_create_leaf_node(SPLT_BW_AND_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return BW_AND_ASSIGN; }
<INITIAL>"^="  { yylval = ast_create_leaf_node(SPLT_BW_XOR_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return BW_XOR_ASSIGN; }
<INITIAL>"|="  { yylval = ast_create_leaf_node(SPLT_BW_OR_ASSIGN, SPLC_YY2LOC_CF_D(yylloc)); return BW_OR_ASSIGN; }

<INITIAL>"<<" { yylval = ast_create_leaf_node(SPLT_LSHIFT, SPLC_YY2LOC_CF_D(yylloc)); return LSHIFT; }
<INITIAL>">>" { yylval = ast_create_leaf_node(SPLT_RSHIFT, SPLC_YY2LOC_CF_D(yylloc)); return RSHIFT; }
<INITIAL>"&"  { yylval = ast_create_leaf_node(SPLT_BW_AND, SPLC_YY2LOC_CF_D(yylloc)); return BW_AND; }
<INITIAL>"|"  { yylval = ast_create_leaf_node(SPLT_BW_OR, SPLC_YY2LOC_CF_D(yylloc)); return BW_OR; }
<INITIAL>"^"  { yylval = ast_create_leaf_node(SPLT_BW_XOR, SPLC_YY2LOC_CF_D(yylloc)); return BW_XOR; }
<INITIAL>"~"  { yylval = ast_create_leaf_node(SPLT_BW_NOT, SPLC_YY2LOC_CF_D(yylloc)); return BW_NOT; }
<INITIAL>"&&" { yylval = ast_create_leaf_node(SPLT_AND, SPLC_YY2LOC_CF_D(yylloc)); return AND; }
<INITIAL>"||" { yylval = ast_create_leaf_node(SPLT_OR, SPLC_YY2LOC_CF_D(yylloc)); return OR; }
<INITIAL>"!" { yylval = ast_create_leaf_node(SPLT_NOT, SPLC_YY2LOC_CF_D(yylloc)); return NOT; }

<INITIAL>"<"  { yylval = ast_create_leaf_node(SPLT_LT, SPLC_YY2LOC_CF_D(yylloc)); return LT; }
<INITIAL>"<=" { yylval = ast_create_leaf_node(SPLT_LE, SPLC_YY2LOC_CF_D(yylloc)); return LE; }
<INITIAL>">"  { yylval = ast_create_leaf_node(SPLT_GT, SPLC_YY2LOC_CF_D(yylloc)); return GT; }
<INITIAL>">=" { yylval = ast_create_leaf_node(SPLT_GE, SPLC_YY2LOC_CF_D(yylloc)); return GE; }
<INITIAL>"!=" { yylval = ast_create_leaf_node(SPLT_NE, SPLC_YY2LOC_CF_D(yylloc)); return NE; }
<INITIAL>"==" { yylval = ast_create_leaf_node(SPLT_EQ, SPLC_YY2LOC_CF_D(yylloc)); return EQ; }

<INITIAL>"++"  { yylval = ast_create_leaf_node(SPLT_DPLUS, SPLC_YY2LOC_CF_D(yylloc)); return DPLUS; }
<INITIAL>"--"  { yylval = ast_create_leaf_node(SPLT_DMINUS, SPLC_YY2LOC_CF_D(yylloc)); return DMINUS; }
<INITIAL>"+"  { yylval = ast_create_leaf_node(SPLT_PLUS, SPLC_YY2LOC_CF_D(yylloc)); return PLUS; }
<INITIAL>"-"  { yylval = ast_create_leaf_node(SPLT_MINUS, SPLC_YY2LOC_CF_D(yylloc)); return MINUS; }
<INITIAL>"*"  { yylval = ast_create_leaf_node(SPLT_ASTRK, SPLC_YY2LOC_CF_D(yylloc)); return ASTRK; }
<INITIAL>"/"  { yylval = ast_create_leaf_node(SPLT_DIV, SPLC_YY2LOC_CF_D(yylloc)); return DIV; }
<INITIAL>"%"  { yylval = ast_create_leaf_node(SPLT_MOD, SPLC_YY2LOC_CF_D(yylloc)); return MOD; }

<INITIAL>"{"  { yylval = ast_create_leaf_node(SPLT_LC, SPLC_YY2LOC_CF_D(yylloc)); return LC; }
<INITIAL>"}"  { yylval = ast_create_leaf_node(SPLT_RC, SPLC_YY2LOC_CF_D(yylloc)); return RC; }
<INITIAL>"("  { yylval = ast_create_leaf_node(SPLT_LP, SPLC_YY2LOC_CF_D(yylloc)); return LP; }
<INITIAL>")"  { yylval = ast_create_leaf_node(SPLT_RP, SPLC_YY2LOC_CF_D(yylloc)); return RP; }
<INITIAL>"["  { yylval = ast_create_leaf_node(SPLT_LSB, SPLC_YY2LOC_CF_D(yylloc)); return LSB; }
<INITIAL>"]"  { yylval = ast_create_leaf_node(SPLT_RSB, SPLC_YY2LOC_CF_D(yylloc)); return RSB; }

<INITIAL>"."  { yylval = ast_create_leaf_node(SPLT_DOT, SPLC_YY2LOC_CF_D(yylloc)); return DOT; }
<INITIAL>"->"  { yylval = ast_create_leaf_node(SPLT_RARROW, SPLC_YY2LOC_CF_D(yylloc)); return RARROW; }


    /* =================== SPL: whitespace characters =================== */
<INITIAL>"\n" { }
<INITIAL>[ \r\t] { }

    /* =================== SPL: unknown lexemes =================== */
<INITIAL>. {
    yylval = ast_create_leaf_node(SPLT_ID, SPLC_YY2LOC_CF_D(yylloc));
    yylval->val = (void *)strdup(yytext);
    SPLC_ERROR(SPLM_ERR_SYN_A, SPLC_YY2LOC_CF_D(yylloc), "unknown lexeme");
    return ID;
}

%%

/* This function defines what to do when EOF has been reached */
int yywrap()
{
    if (YY_START == IN_CL_COMMENT)
    {
        SPLC_ERROR(SPLM_ERR_UNIV, SPLC_YY2LOC_CF_D(yylloc), "unclosed comment at the end of file");
    }
    int op = splc_pop_buffer();
    if (op == 0)
    {
        yy_pop_state();
    }
    return op;
}