/*===----------------------------------------------------------------------===//
//                            Code Requirements                                 
//===----------------------------------------------------------------------===*/
%{

#undef  YY_DECL
#define YY_DECL int splc::IO::Scanner::yylex(splc::IO::Parser::value_type *const yylval, splc::utils::Location *yyloc)

// Implementation of yyFlexScanner
#include "AST/AST.hh"
#include "Core/splc.hh"
#include "IO/Scanner.hh"

// Required std headers
#include <string>
#include <iostream>
#include <ranges>
#include <algorithm>

// typedef to make the returns for the tokens shorter
using Token = splc::IO::Parser::token;
using SymbolType = splc::ASTSymbolType;
using String = std::string;

// define yyterminate as this instead of NULL
// #define yyterminate() return( Token::END )

// update location on matching
#define YY_USER_ACTION gloc->step(); gloc->columns(yyleng);

%}

/*===----------------------------------------------------------------------===//
//                               Flex Options
//===----------------------------------------------------------------------===*/
%option c++
%option prefix="Splc"
%option never-interactive
%option yyclass="splc::IO::Scanner"
%option debug
%option nodefault
%option stack

/*===----------------------------------------------------------------------===//
//                          Predefined FLEX States
//===----------------------------------------------------------------------===*/
/* In single-line comment */
%s IN_SL_COMMENT
/* In cross-line comment */
%s IN_CL_COMMENT
/* In string */
%s IN_STRING
/* Preprocessor Directive */
%s IN_PPD
/* Preprocessor Directive: include */
%s IN_PPD_INCL
/* Preprocessor Directive: include filename, double quote */
%s IN_PPD_INCL_DQFN
/* Preprocessor Directive: include filename, angle bracket */
%s IN_PPD_INCL_ABFN
/* Preprocessor Directive: define */
%s IN_PPD_DEFINE
/* Preprocessor Directive: define body */
%s IN_PPD_DEFINE_BODY

/* Cross-line comment reference: http://westes.github.io/flex/manual/How-can-I-match-C_002dstyle-comments_003f.html */

/*===----------------------------------------------------------------------===//
//                         Predefined Token Variables
//===----------------------------------------------------------------------===*/
identifier [a-zA-Z_][a-zA-Z0-9_]*

/*===----------------------------------------------------------------------===//
//                             Action Definitions
//===----------------------------------------------------------------------===*/
%%
    /*===------------------------------------------------------------------===//
    //       `yylex()` initialization: executed at beginning of `yylex()`
    //===------------------------------------------------------------------===*/
%{          
            yyloc->switchToContext(transMgr.getCurrentTransContextKey());
            glval = yylval;
            gloc = yyloc;
%}

    /*===------------------------------------------------------------------===//
    //                        Multi-Line Comments
    //===------------------------------------------------------------------===*/
"/*" {
    /* ignore comments */
    yy_push_state(IN_CL_COMMENT);
}

<IN_CL_COMMENT>"*/" {
    yy_pop_state();
}

<IN_CL_COMMENT>\r?\n {
    gloc->lines();
}

<IN_CL_COMMENT>[^\*\r\n]+ {
    // eat comment in chunks
    // but not newline
}

<IN_CL_COMMENT>"*" {
    // eat the lone star
}

    /*===------------------------------------------------------------------===//
    //                           Single-Line Comments
    //===------------------------------------------------------------------===*/

"//" {
    /* match single-line comments that may spread across lines by '\\' */
    /* Explanation: match the prefix "//", then: */
    /*     match any character except newline or "\\(\r, if any)\n" as the body, terminated by "\n" */
    yy_push_state(IN_SL_COMMENT);
}

<IN_SL_COMMENT>[^\r\n]* {
    /* eat comments */
}

<IN_SL_COMMENT>\\\r?\n {
    /* allow SL comment to span across multiple lines */
    gloc->lines();
}

<IN_SL_COMMENT>\r?\n {
    /* exit single-line comment mode */
    gloc->lines();
    yy_pop_state();
}

    /*===------------------------------------------------------------------===//
    //                   Preprocessor Directive Translation
    //===------------------------------------------------------------------===*/
<INITIAL># {
    yy_push_state(IN_PPD);
}

    /* PPD Dispatch: include */
<IN_PPD>"include" {
    yy_pop_state(); /* pop state IN_PPD */
    yy_push_state(IN_PPD_INCL);
}

    /* PPD Dispatch: define */
<IN_PPD>"define" {
    yy_pop_state();
    yy_push_state(IN_PPD_DEFINE);
}

<IN_PPD>. {

}

<IN_PPD>\\\r?\n {
    /* allow line break */
    gloc->lines();
}

<IN_PPD>\r?\n {
    gloc->lines();
    yy_pop_state();
}

    /*===------------------------------------------------------------------===//
    //                        PPD: #include directive
    //===------------------------------------------------------------------===*/
<IN_PPD_INCL>[ ] {
    /* Skip spaces */
}

<IN_PPD_INCL>\< {
    yy_pop_state();
    yy_push_state(IN_PPD_INCL_ABFN); /* Enter filename detection */
}

<IN_PPD_INCL>\" {
    yy_pop_state();
    yy_push_state(IN_PPD_INCL_DQFN); /* Enter filename detection */
}

<IN_PPD_INCL_ABFN,IN_PPD_INCL_DQFN>[^\>\"\r\n]* {
    strVec.push_back(yytext);
    locVec.push_back(*gloc);
}

<IN_PPD_INCL_ABFN,IN_PPD_INCL_DQFN>\\\r?\n {
    gloc->lines();
    locVec.push_back(*gloc);
}

<IN_PPD_INCL_ABFN>\> {
    String resStr = concatTmpStrVec();
    Location resLoc = concatTmpLocVec();
    if (pushFileContext(&resLoc, resStr)) {
        yy_push_state(INITIAL);
    }
}

<IN_PPD_INCL_DQFN>\" {
    String resStr = concatTmpStrVec();
    Location resLoc = concatTmpLocVec();
    if (pushFileContext(&resLoc, resStr)) {
        yy_push_state(INITIAL);
    }
}

<IN_PPD_INCL_ABFN,IN_PPD_INCL_DQFN>\r?\n {
    yy_pop_state();
    yy_pop_state();
}

<IN_PPD_INCL_ABFN,IN_PPD_INCL_DQFN>. {
}

    /*===------------------------------------------------------------------===//
    //                        PPD: #define directive
    //===------------------------------------------------------------------===*/
<IN_PPD_DEFINE>{identifier} {
    if (!isMacroVarContextPresent(yytext))
    {
        tmpStr = yytext;
        tmpLoc = *gloc;
        internalFlag = true;
    }
    else {
        internalFlag = false;
    }
    yy_pop_state();
    yy_push_state(IN_PPD_DEFINE_BODY);
}

<IN_PPD_DEFINE>. {

}

<IN_PPD_DEFINE>\\\r?\n {
    gloc->lines();
}

<IN_PPD_DEFINE>\r?\n {
    gloc->lines();
    yy_pop_state();
}

<IN_PPD_DEFINE_BODY>[^\r\n]+ {
    if (internalFlag) {
        strVec.push_back(yytext);
        locVec.push_back(*gloc);
    }
}

<IN_PPD_DEFINE_BODY>\\\r?\n {
    gloc->lines();
    if (internalFlag) {
        strVec.push_back(yytext);
        locVec.push_back(*gloc);
    }
}

<IN_PPD_DEFINE_BODY>\r?\n {
    yy_pop_state();

    gloc->lines();
    if (internalFlag) {
        locVec.push_back(*gloc);
        
        String content = concatTmpStrVec();
        *gloc = concatTmpLocVec();
        SPLC_ASSERT(registerMacroVarContext(&tmpLoc, tmpStr, content)) << "internal error: cannot register macro var";
    }
}

    /*===------------------------------------------------------------------===//
    //                            String Literals
    //===------------------------------------------------------------------===*/
<INITIAL>\" {
    yy_push_state(IN_STRING);
}
    /* Empty string */
<INITIAL>\"\" {
    *glval = transMgr.makeAST<AST>(SymbolType::StrUnit, *gloc, String{});
    return Token::StrUnit;
}

    /* Match escape characters */
<IN_STRING>\\[abefnrtv\\\'\"\?] {
    strVec.push_back(yytext + 1); /* Ignore the escape operator */
    locVec.push_back(*gloc);
}

    /* Match octal escape sequence */
<IN_STRING>\\0[0-7]{0,2} {
    int val = std::stoi({yytext + 2}, nullptr, 8);
    strVec.push_back({1, static_cast<char>(val)}); /* Ignore the escape operator */
    locVec.push_back(*gloc);
}

<IN_STRING>\\[0-9]{1,3} {
    locVec.push_back(*gloc);
    SPLC_LOG_ERROR(gloc, true) << "invalid octal escape sequence";
}

    /* Match escape characters */
<IN_STRING>\\x[0-9a-fA-F]{2,3} {
    int val = std::stoi({yytext + 2}, nullptr, 16);
    strVec.push_back({1, static_cast<char>(val)}); /* Ignore the escape operator */
    locVec.push_back(*gloc);
}
    /* Match escape characters */
<IN_STRING>\\x[0-9a-zA-Z]{2,3} {
    locVec.push_back(*gloc);
    SPLC_LOG_ERROR(gloc, true) << "invalid hexadecimal escape sequence";
}

    /* Match invalid escape characters */
<IN_STRING>\\. {
    strVec.push_back(yytext); /* Ignore the escape operator */
    locVec.push_back(*gloc);
}

    /* Allow line break */
<IN_STRING>\\\r?\n {
    /* by std C definition, skip this and the newline character */
    gloc->lines();
    locVec.push_back(*gloc);
}

<IN_STRING>\" {
    yy_pop_state(); /* exit IN_STRING state */

    locVec.push_back(*gloc);
    String str = concatTmpStrVec();
    *gloc = concatTmpLocVec();
    *glval = transMgr.makeAST<AST>(SymbolType::StrUnit, *gloc, str);
    return Token::StrUnit;
}

    /* Match everything except line break, double quote and the escape operator */
<IN_STRING>[^\n\r\"\\]+ {
    strVec.push_back(yytext);
    locVec.push_back(*gloc);
}

<IN_STRING><<EOF>> {
    SPLC_LOG_ERROR(gloc, true) << "unclosed string literal";
    yy_pop_state();
}

    /*===------------------------------------------------------------------===//
    //                  Token: Keyword/Qualifiers
    //===------------------------------------------------------------------===*/
<INITIAL>"auto"     { *glval = transMgr.makeAST<AST>(SymbolType::KwdAuto, *gloc); return Token::KwdAuto; }
<INITIAL>"extern"   { *glval = transMgr.makeAST<AST>(SymbolType::KwdExtern, *gloc); return Token::KwdExtern; }
<INITIAL>"register" { *glval = transMgr.makeAST<AST>(SymbolType::KwdRegister, *gloc); return Token::KwdRegister; }
<INITIAL>"static"   { *glval = transMgr.makeAST<AST>(SymbolType::KwdStatic, *gloc); return Token::KwdStatic; }
<INITIAL>"typedef"  { *glval = transMgr.makeAST<AST>(SymbolType::KwdTypedef, *gloc); return Token::KwdTypedef; }

<INITIAL>"const"    { *glval = transMgr.makeAST<AST>(SymbolType::KwdConst, *gloc); return Token::KwdConst; }
<INITIAL>"restrict" { *glval = transMgr.makeAST<AST>(SymbolType::KwdRestrict, *gloc); return Token::KwdRestrict; }
<INITIAL>"volatile" { *glval = transMgr.makeAST<AST>(SymbolType::KwdVolatile, *gloc); return Token::KwdVolatile; }

<INITIAL>"inline"   { *glval = transMgr.makeAST<AST>(SymbolType::KwdInline, *gloc); return Token::KwdInline; }

<INITIAL>"void"     { *glval = transMgr.makeAST<AST>(SymbolType::VoidTy, *gloc); return Token::VoidTy; }
<INITIAL>"int"      { *glval = transMgr.makeAST<AST>(SymbolType::IntTy, *gloc); return Token::IntTy; }
<INITIAL>"signed"   { *glval = transMgr.makeAST<AST>(SymbolType::SignedTy, *gloc); return Token::SignedTy; }
<INITIAL>"unsigned" { *glval = transMgr.makeAST<AST>(SymbolType::UnsignedTy, *gloc); return Token::UnsignedTy; }
<INITIAL>"long"     { *glval = transMgr.makeAST<AST>(SymbolType::LongTy, *gloc); return Token::LongTy; }
<INITIAL>"float"    { *glval = transMgr.makeAST<AST>(SymbolType::FloatTy, *gloc); return Token::FloatTy; }
<INITIAL>"double"   { *glval = transMgr.makeAST<AST>(SymbolType::DoubleTy, *gloc); return Token::DoubleTy; }
<INITIAL>"char"     { *glval = transMgr.makeAST<AST>(SymbolType::CharTy, *gloc); return Token::CharTy; }
<INITIAL>"enum"     { *glval = transMgr.makeAST<AST>(SymbolType::KwdEnum, *gloc); return Token::KwdEnum; }

<INITIAL>"struct"   { *glval = transMgr.makeAST<AST>(SymbolType::KwdStruct, *gloc); return Token::KwdStruct; }
<INITIAL>"union"    { *glval = transMgr.makeAST<AST>(SymbolType::KwdUnion, *gloc); return Token::KwdUnion; }

<INITIAL>"if"       { *glval = transMgr.makeAST<AST>(SymbolType::KwdIf, *gloc); return Token::KwdIf; }
<INITIAL>"else"     { *glval = transMgr.makeAST<AST>(SymbolType::KwdElse, *gloc); return Token::KwdElse; }
<INITIAL>"switch"   { *glval = transMgr.makeAST<AST>(SymbolType::KwdSwitch, *gloc); return Token::KwdSwitch; }

<INITIAL>"while"    { *glval = transMgr.makeAST<AST>(SymbolType::KwdWhile, *gloc); return Token::KwdWhile; }
<INITIAL>"for"      { *glval = transMgr.makeAST<AST>(SymbolType::KwdFor, *gloc); return Token::KwdFor; }
<INITIAL>"do"       { *glval = transMgr.makeAST<AST>(SymbolType::KwdDo, *gloc); return Token::KwdDo; }

<INITIAL>"default"  { *glval = transMgr.makeAST<AST>(SymbolType::KwdDefault, *gloc); return Token::KwdDefault; }
<INITIAL>"case"     { *glval = transMgr.makeAST<AST>(SymbolType::KwdCase, *gloc); return Token::KwdCase; }

<INITIAL>"goto"     { *glval = transMgr.makeAST<AST>(SymbolType::KwdGoto, *gloc); return Token::KwdGoto; }
<INITIAL>"continue" { *glval = transMgr.makeAST<AST>(SymbolType::KwdContinue, *gloc); return Token::KwdContinue; }
<INITIAL>"break"    { *glval = transMgr.makeAST<AST>(SymbolType::KwdBreak, *gloc); return Token::KwdBreak; }
<INITIAL>"return"   { *glval = transMgr.makeAST<AST>(SymbolType::KwdReturn, *gloc); return Token::KwdReturn; }


    /*===------------------------------------------------------------------===//
    //                        Literal Definitions
    //===------------------------------------------------------------------===*/
    
    /* =================== SPL: floating point =================== */
<INITIAL>[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {
    ASTFloatType val = 0.0;
    try {
        val = std::stod({yytext});
    } catch (std::out_of_range e) {
        SPLC_LOG_ERROR(gloc, true) << e.what();
    }
    *glval = transMgr.makeAST<AST>(SymbolType::FloatLiteral, *gloc, val);
    return Token::FloatLiteral;
}
    
<INITIAL>[0-9]*\.[0-9]+([eE]|[-+]|[\.])+ {
    ASTFloatType val = 0.0;
    SPLC_LOG_ERROR(gloc, true) << "too many decimal points or exponential indicators";
    *glval = transMgr.makeAST<AST>(SymbolType::FloatLiteral, *gloc, val);
    return Token::FloatLiteral;
}

    /* =================== SPL: integers =================== */
<INITIAL>[0-9]+ {
    ASTUIntType val = 0ULL;
    try {
        val = std::stoull({yytext});
    } catch (std::out_of_range e) {
        SPLC_LOG_ERROR(gloc, true) << e.what();
    }
    *glval = transMgr.makeAST<AST>(SymbolType::UIntLiteral, *gloc, val);
    return Token::UIntLiteral;
}

<INITIAL>0[xX][0-9a-fA-F]{1,16} {
    ASTUIntType val = 0ULL;
    try {
        val = std::stoull({yytext}, nullptr, 16);
    } catch (std::out_of_range e) {
        SPLC_LOG_ERROR(gloc, true) << e.what();
    }
    *glval = transMgr.makeAST<AST>(SymbolType::UIntLiteral, *gloc, val);
    return Token::UIntLiteral;
}

    /* SPL: invalid hexadecimal integer */
<INITIAL>0[xX][0-9a-zA-Z]+ {
    ASTUIntType val = 0ULL;
    SPLC_LOG_ERROR(gloc, true) << "ill-formed hexadecimal integer";
    *glval = transMgr.makeAST<AST>(SymbolType::UIntLiteral, *gloc, val);
    return Token::UIntLiteral;
}

    /* =================== SPL: char =================== */
<INITIAL>'\\x[0-9a-fA-F]{2}' {
    ASTCharType val = static_cast<ASTCharType>(std::stoi({yytext + 3}, nullptr, 16));
    *glval = transMgr.makeAST<AST>(SymbolType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

<INITIAL>'\\x[0-9a-zA-Z]*' {
    SPLC_LOG_ERROR(gloc, true) << "ill-formed char";
    ASTCharType val = '\0';
    *glval = transMgr.makeAST<AST>(SymbolType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

<INITIAL>'\\[abefnrtv\\\'\"\?]' {
    ASTCharType val = yytext[2];
    *glval = transMgr.makeAST<AST>(SymbolType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

<INITIAL>'\\0[0-7]{0,2}' {
    ASTCharType val = static_cast<ASTCharType>(std::stoi({yytext + 2}, nullptr, 8));
    *glval = transMgr.makeAST<AST>(SymbolType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

<INITIAL>'\\[0-9]{1,3}' {
    SPLC_LOG_ERROR(gloc, true) << "ill-formed char";
    ASTCharType val = '\0';
    *glval = transMgr.makeAST<AST>(SymbolType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

<INITIAL>'.' {
    ASTCharType val = yytext[1];
    *glval = transMgr.makeAST<AST>(SymbolType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

    /*===------------------------------------------------------------------===//
    //                         Operator Declarations
    //===------------------------------------------------------------------===*/

    /* Assignments */
<INITIAL>"="      { *glval = transMgr.makeAST<AST>(SymbolType::OpAssign, *gloc); return Token::OpAssign; }
<INITIAL>"*="     { *glval = transMgr.makeAST<AST>(SymbolType::OpMulAssign, *gloc); return Token::OpMulAssign; }
<INITIAL>"/="     { *glval = transMgr.makeAST<AST>(SymbolType::OpDivAssign, *gloc); return Token::OpDivAssign; }
<INITIAL>"%="     { *glval = transMgr.makeAST<AST>(SymbolType::OpModAssign, *gloc); return Token::OpModAssign; }
<INITIAL>"+="     { *glval = transMgr.makeAST<AST>(SymbolType::OpPlusAssign, *gloc); return Token::OpPlusAssign; }
<INITIAL>"-="     { *glval = transMgr.makeAST<AST>(SymbolType::OpMinusAssign, *gloc); return Token::OpMinusAssign; }
<INITIAL>"<<="    { *glval = transMgr.makeAST<AST>(SymbolType::OpLShiftAssign, *gloc); return Token::OpLShiftAssign; }
<INITIAL>">>="    { *glval = transMgr.makeAST<AST>(SymbolType::OpRShiftAssign, *gloc); return Token::OpRShiftAssign; }
<INITIAL>"&="     { *glval = transMgr.makeAST<AST>(SymbolType::OpBAndAssign, *gloc); return Token::OpBAndAssign; }
<INITIAL>"^="     { *glval = transMgr.makeAST<AST>(SymbolType::OpBXorAssign, *gloc); return Token::OpBXorAssign; }
<INITIAL>"|="     { *glval = transMgr.makeAST<AST>(SymbolType::OpBOrAssign, *gloc); return Token::OpBOrAssign; }

    /* Conditional */
<INITIAL>"&&"     { *glval = transMgr.makeAST<AST>(SymbolType::OpAnd, *gloc); return Token::OpAnd; }
<INITIAL>"||"     { *glval = transMgr.makeAST<AST>(SymbolType::OpOr, *gloc); return Token::OpOr; }
<INITIAL>"!"      { *glval = transMgr.makeAST<AST>(SymbolType::OpNot, *gloc); return Token::OpNot; }

<INITIAL>"<"      { *glval = transMgr.makeAST<AST>(SymbolType::OpLT, *gloc); return Token::OpLT; }
<INITIAL>"<="     { *glval = transMgr.makeAST<AST>(SymbolType::OpLE, *gloc); return Token::OpLE; }
<INITIAL>">"      { *glval = transMgr.makeAST<AST>(SymbolType::OpGT, *gloc); return Token::OpGT; }
<INITIAL>">="     { *glval = transMgr.makeAST<AST>(SymbolType::OpGE, *gloc); return Token::OpGE; }
<INITIAL>"!="     { *glval = transMgr.makeAST<AST>(SymbolType::OpNE, *gloc); return Token::OpNE; }
<INITIAL>"=="     { *glval = transMgr.makeAST<AST>(SymbolType::OpEQ, *gloc); return Token::OpEQ; }
    
<INITIAL>"?"      { *glval = transMgr.makeAST<AST>(SymbolType::OpQMark, *gloc); return Token::OpQMark; }
<INITIAL>":"      { *glval = transMgr.makeAST<AST>(SymbolType::OpColon, *gloc); return Token::OpColon; }

    /* Arithmetics */
<INITIAL>"<<"     { *glval = transMgr.makeAST<AST>(SymbolType::OpLShift, *gloc); return Token::OpLShift; }
<INITIAL>">>"     { *glval = transMgr.makeAST<AST>(SymbolType::OpRShift, *gloc); return Token::OpRShift; }
<INITIAL>"&"      { *glval = transMgr.makeAST<AST>(SymbolType::OpBAnd, *gloc); return Token::OpBAnd; }
<INITIAL>"|"      { *glval = transMgr.makeAST<AST>(SymbolType::OpBOr, *gloc); return Token::OpBOr; }
<INITIAL>"~"      { *glval = transMgr.makeAST<AST>(SymbolType::OpBNot, *gloc); return Token::OpBNot; }
<INITIAL>"^"      { *glval = transMgr.makeAST<AST>(SymbolType::OpBXor, *gloc); return Token::OpBXor; }

<INITIAL>"++"     { *glval = transMgr.makeAST<AST>(SymbolType::OpDPlus, *gloc); return Token::OpDPlus; }
<INITIAL>"--"     { *glval = transMgr.makeAST<AST>(SymbolType::OpDMinus, *gloc); return Token::OpDMinus; }
<INITIAL>"+"      { *glval = transMgr.makeAST<AST>(SymbolType::OpPlus, *gloc); return Token::OpPlus; }
<INITIAL>"-"      { *glval = transMgr.makeAST<AST>(SymbolType::OpMinus, *gloc); return Token::OpMinus; }
<INITIAL>"*"      { *glval = transMgr.makeAST<AST>(SymbolType::OpAstrk, *gloc); return Token::OpAstrk; }
<INITIAL>"/"      { *glval = transMgr.makeAST<AST>(SymbolType::OpDiv, *gloc); return Token::OpDiv; }
<INITIAL>"%"      { *glval = transMgr.makeAST<AST>(SymbolType::OpMod, *gloc); return Token::OpMod; }

    /* Builtin */
<INITIAL>"."      { *glval = transMgr.makeAST<AST>(SymbolType::OpDot, *gloc); return Token::OpDot; }
<INITIAL>"->"     { *glval = transMgr.makeAST<AST>(SymbolType::OpRArrow, *gloc); return Token::OpRArrow; }

<INITIAL>"sizeof" { *glval = transMgr.makeAST<AST>(SymbolType::OpSizeOf, *gloc); return Token::OpSizeOf; }

    /* Misc */
<INITIAL>","      { *glval = transMgr.makeAST<AST>(SymbolType::OpComma, *gloc); return Token::OpComma; }
<INITIAL>"..."    { *glval = transMgr.makeAST<AST>(SymbolType::OpEllipsis, *gloc); return Token::OpEllipsis; }

    /*===------------------------------------------------------------------===//
    //                        Punctuator Declarations
    //===------------------------------------------------------------------===*/
<INITIAL>";"      { *glval = transMgr.makeAST<AST>(SymbolType::PuncSemi, *gloc); return Token::PuncSemi; }

<INITIAL>"{"      { *glval = transMgr.makeAST<AST>(SymbolType::PuncLCurly, *gloc); return Token::PuncLCurly; }
<INITIAL>"}"      { *glval = transMgr.makeAST<AST>(SymbolType::PuncRCurly, *gloc); return Token::PuncRCurly; }

<INITIAL>"("      { *glval = transMgr.makeAST<AST>(SymbolType::PuncLParen, *gloc); return Token::PuncLParen; }
<INITIAL>")"      { *glval = transMgr.makeAST<AST>(SymbolType::PuncRParen, *gloc); return Token::PuncRParen; }

<INITIAL>"["      { *glval = transMgr.makeAST<AST>(SymbolType::PuncLSBracket, *gloc); return Token::PuncLSBracket; }
<INITIAL>"]"      { *glval = transMgr.makeAST<AST>(SymbolType::PuncRSBracket, *gloc); return Token::PuncRSBracket; }

    /*===------------------------------------------------------------------===//
    //                           Identifier Definition
    //===------------------------------------------------------------------===*/
<INITIAL>{identifier} {
    String val{yytext};
    /* First, check if it is a macro definition. If it is, just expand. */
    if (pushMacroVarContext(gloc, val)) {
        yy_push_state(INITIAL);
    }
    else if (transMgr.isSymbolDeclared(SymEntryType::Typedef, val)) {
        *glval = transMgr.makeAST<AST>(SymbolType::TypedefID, *gloc, val);
        return Token::TypedefID;
    }
    else {
        *glval = transMgr.makeAST<AST>(SymbolType::ID, *gloc, val);
        return Token::ID;
    }
}

<INITIAL>[0-9][a-zA-Z0-9_]* {
    SPLC_LOG_ERROR(gloc, true) << "identifier name cannot start with digits";
    String val{yytext};
    *glval = transMgr.makeAST<AST>(SymbolType::ID, *gloc, val);
    return Token::ID;
}

    /*===------------------------------------------------------------------===//
    //                         Whitespace Characters
    //===------------------------------------------------------------------===*/
<INITIAL>\n { gloc->lines(); }
<INITIAL>[ \r\t] { }

    /*===------------------------------------------------------------------===//
    //                                 Misc
    //===------------------------------------------------------------------===*/

    /* =================== SPL: unknown lexemes =================== */
<INITIAL>. {
    SPLC_LOG_ERROR(gloc, true) << "unknown lexeme";
    String val{1, yytext[0]};
    *glval = transMgr.makeAST<AST>(SymbolType::ID, *gloc, val);
    return Token::ID;
}

<*>.|\n {
    SPLC_ERROR(233) << "invalid lexeme. Should be caused by a previous parsing error.";
}

%%

int yyFlexLexer::yywrap()
{
    // Inherit this method to let the code compile, but don't do anything.
    return 1;
}