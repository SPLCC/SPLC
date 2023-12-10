%option c++
%option yylineno
%option stack
%{   
    // #include <syntax.tab.hpp>
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
        /* SPLC_MSG(SPLM_ERR_UNIV, SPLC_YY2LOC_CF_D(yylloc), "unclosed comment"); */
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

<IN_PPD_INCL>\<([^\r\n]|(\\\r?\n))+\> {}

<IN_PPD_INCL>\"([^\r\n]|(\\\r?\n))+\" {}

<IN_PPD_INCL>. {}

<IN_PPD_INCL>\r?\n {
    yy_pop_state();
}

    /* ============== PPD: #define directive ============== */
<IN_PPD_DEFINE>{identifier} {}

<IN_PPD_DEFINE>. {

}

<IN_PPD_DEFINE>\\\r?\n {

}

<IN_PPD_DEFINE>\r?\n {
    yy_pop_state();
}

<IN_PPD_DEFINE_BODY>([^\r\n]*(\\[\r]?\n)?)* {}

<IN_PPD_DEFINE_BODY>. {

}

<IN_PPD_DEFINE_BODY>\r?\n {}

    /* =================== STRING =================== */
<INITIAL>\" {}

<INITIAL>\"\" {}

    /* Match escape characters */
<IN_STRING>\\[abefnrtv\\\'\"\?] {}

    /* Allow line break */
<IN_STRING>\\\r?\n {
    /* by std C definition, skip this and the newline character */
}

    /* Match line break */
<IN_STRING>[^\n\r\"\\]+ {}

    /* Match line break */
<IN_STRING>\\[0-9]{1,3} {}

    /* Match escape characters */
<IN_STRING>\\x[0-9a-fA-F]{2,3} {}

<IN_STRING>\" {}

<IN_STRING><<EOF>> {}

    /* =================== SPL: keywords =================== */
<INITIAL>"enum"     {}
<INITIAL>"struct"   {}
<INITIAL>"union"    {}

<INITIAL>"auto"     {}
<INITIAL>"extern"   {}
<INITIAL>"register" {}
<INITIAL>"static"   {}
<INITIAL>"typedef"  {}

<INITIAL>"const"    {}
<INITIAL>"restrict" {}
<INITIAL>"volatile" {}

<INITIAL>"..."      {}

<INITIAL>"while"    {}
<INITIAL>"for"      {}
<INITIAL>"do"       {}

<INITIAL>"if"       {}
<INITIAL>"else"     {}
<INITIAL>"switch"   {}
<INITIAL>"default"  {}
<INITIAL>"case"     {}

<INITIAL>"goto"     {}
<INITIAL>"continue" {}
<INITIAL>"break"    {}
<INITIAL>"return"   {}

<INITIAL>"sizeof"   {}

<INITIAL>"void"     {}
<INITIAL>"int"      {}
<INITIAL>"signed"   {}
<INITIAL>"unsigned" {}
<INITIAL>"long"     {}
<INITIAL>"float"    {}
<INITIAL>"char"     {}

    /* =================== SPL: floating point =================== */
<INITIAL>[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {}

<INITIAL>[0-9]*\.[0-9]+([eE]|[-+]|[\.])+ {}

    /* =================== SPL: integers =================== */
<INITIAL>[0-9]+ {}

<INITIAL>0[xX][0-9a-fA-F]{1,8} {}

    /* SPL: invalid hexadecimal integer */
<INITIAL>0[xX][0-9a-fA-F]{9,} {}

    /* =================== SPL: char =================== */
<INITIAL>0[xX][0-9a-zA-Z]+ {}

<INITIAL>'\\x[0-9a-fA-F]{2}' {}

<INITIAL>'\\x[0-9a-zA-Z]*' {}

    /* =================== SPL: ID =================== */
<INITIAL>{identifier} {}

<INITIAL>[0-9][a-zA-Z0-9_]* {}

<INITIAL>'\\[abefnrtv\\\'\"\?]' {}

<INITIAL>'\\([abefnrtv\\\'\"\?]|[0-9]{1,3})' {}

<INITIAL>'.' {}


    /* =================== SPL: punctuators =================== */
<INITIAL>";"  {}
<INITIAL>","  {}
<INITIAL>"?"  {}
<INITIAL>":"  {}

<INITIAL>"="  {}
<INITIAL>"*="  {}
<INITIAL>"/="  {}
<INITIAL>"%="  {}
<INITIAL>"+="  {}
<INITIAL>"-="  {}
<INITIAL>"<<="  {}
<INITIAL>">>="  {}
<INITIAL>"&="  {}
<INITIAL>"^="  {}
<INITIAL>"|="  {}

<INITIAL>"<<" {}
<INITIAL>">>" {}
<INITIAL>"&"  {}
<INITIAL>"|"  {}
<INITIAL>"^"  {}
<INITIAL>"~"  {}
<INITIAL>"&&" {}
<INITIAL>"||" {}
<INITIAL>"!" {}

<INITIAL>"<"  {}
<INITIAL>"<=" {}
<INITIAL>">"  {}
<INITIAL>">=" {}
<INITIAL>"!=" {}
<INITIAL>"==" {}

<INITIAL>"++"  {}
<INITIAL>"--"  {}
<INITIAL>"+"  {}
<INITIAL>"-"  {}
<INITIAL>"*"  {}
<INITIAL>"/"  {}
<INITIAL>"%"  {}

<INITIAL>"{"  {}
<INITIAL>"}"  {}
<INITIAL>"("  {}
<INITIAL>")"  {}
<INITIAL>"["  {}
<INITIAL>"]"  {}

<INITIAL>"."  {}
<INITIAL>"->"  {}


    /* =================== SPL: whitespace characters =================== */
<INITIAL>"\n" { }
<INITIAL>[ \r\t] { }

    /* =================== SPL: unknown lexemes =================== */
<INITIAL>. {}

%%

/* This function defines what to do when EOF has been reached */
int yywrap();