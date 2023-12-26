/*===----------------------------------------------------------------------===//
//                            Code Requirements                                 
//===----------------------------------------------------------------------===*/
%{

#undef  YY_DECL
#define YY_DECL int splc::IO::Scanner::yylex(splc::IO::Parser::value_type *const yylval, splc::utils::Location *yyloc)

// Implementation of yyFlexScanner
#include "Core/splc.hh"
#include "IO/Scanner.hh"
#include "AST/DerivedAST.hh"

// Required std headers
#include <string>
#include <iostream>
#include <ranges>
#include <algorithm>

// typedef to make the returns for the tokens shorter
using Token = splc::IO::Parser::token;
using SymType = splc::ASTSymType;
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
        splc_assert(registerMacroVarContext(&tmpLoc, tmpStr, content)) << "internal error: cannot register macro var";
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
    *glval = AST::make(tyCtxt, SymType::StrUnit, *gloc, String{});
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
    *glval = AST::make(tyCtxt, SymType::StrUnit, *gloc, str);
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
<INITIAL>"auto"     { *glval = AST::make(tyCtxt, SymType::KwdAuto, *gloc); return Token::KwdAuto; }
<INITIAL>"extern"   { *glval = AST::make(tyCtxt, SymType::KwdExtern, *gloc); return Token::KwdExtern; }
<INITIAL>"register" { *glval = AST::make(tyCtxt, SymType::KwdRegister, *gloc); return Token::KwdRegister; }
<INITIAL>"static"   { *glval = AST::make(tyCtxt, SymType::KwdStatic, *gloc); return Token::KwdStatic; }
<INITIAL>"typedef"  { *glval = AST::make(tyCtxt, SymType::KwdTypedef, *gloc); return Token::KwdTypedef; }

<INITIAL>"const"    { *glval = AST::make(tyCtxt, SymType::KwdConst, *gloc); return Token::KwdConst; }
<INITIAL>"restrict" { *glval = AST::make(tyCtxt, SymType::KwdRestrict, *gloc); return Token::KwdRestrict; }
<INITIAL>"volatile" { *glval = AST::make(tyCtxt, SymType::KwdVolatile, *gloc); return Token::KwdVolatile; }

<INITIAL>"inline"   { *glval = AST::make(tyCtxt, SymType::KwdInline, *gloc); return Token::KwdInline; }

<INITIAL>"void"     { *glval = AST::make(tyCtxt, SymType::VoidTy, *gloc); return Token::VoidTy; }
<INITIAL>"int"      { *glval = AST::make(tyCtxt, SymType::IntTy, *gloc); return Token::IntTy; }
<INITIAL>"signed"   { *glval = AST::make(tyCtxt, SymType::SignedTy, *gloc); return Token::SignedTy; }
<INITIAL>"unsigned" { *glval = AST::make(tyCtxt, SymType::UnsignedTy, *gloc); return Token::UnsignedTy; }
<INITIAL>"long"     { *glval = AST::make(tyCtxt, SymType::LongTy, *gloc); return Token::LongTy; }
<INITIAL>"float"    { *glval = AST::make(tyCtxt, SymType::FloatTy, *gloc); return Token::FloatTy; }
<INITIAL>"double"   { *glval = AST::make(tyCtxt, SymType::DoubleTy, *gloc); return Token::DoubleTy; }
<INITIAL>"char"     { *glval = AST::make(tyCtxt, SymType::CharTy, *gloc); return Token::CharTy; }
<INITIAL>"enum"     { *glval = AST::make(tyCtxt, SymType::KwdEnum, *gloc); return Token::KwdEnum; }

<INITIAL>"struct"   { *glval = AST::make(tyCtxt, SymType::KwdStruct, *gloc); return Token::KwdStruct; }
<INITIAL>"union"    { *glval = AST::make(tyCtxt, SymType::KwdUnion, *gloc); return Token::KwdUnion; }

<INITIAL>"if"       { *glval = AST::make(tyCtxt, SymType::KwdIf, *gloc); return Token::KwdIf; }
<INITIAL>"else"     { *glval = AST::make(tyCtxt, SymType::KwdElse, *gloc); return Token::KwdElse; }
<INITIAL>"switch"   { *glval = AST::make(tyCtxt, SymType::KwdSwitch, *gloc); return Token::KwdSwitch; }

<INITIAL>"while"    { *glval = AST::make(tyCtxt, SymType::KwdWhile, *gloc); return Token::KwdWhile; }
<INITIAL>"for"      { *glval = AST::make(tyCtxt, SymType::KwdFor, *gloc); return Token::KwdFor; }
<INITIAL>"do"       { *glval = AST::make(tyCtxt, SymType::KwdDo, *gloc); return Token::KwdDo; }

<INITIAL>"default"  { *glval = AST::make(tyCtxt, SymType::KwdDefault, *gloc); return Token::KwdDefault; }
<INITIAL>"case"     { *glval = AST::make(tyCtxt, SymType::KwdCase, *gloc); return Token::KwdCase; }

<INITIAL>"goto"     { *glval = AST::make(tyCtxt, SymType::KwdGoto, *gloc); return Token::KwdGoto; }
<INITIAL>"continue" { *glval = AST::make(tyCtxt, SymType::KwdContinue, *gloc); return Token::KwdContinue; }
<INITIAL>"break"    { *glval = AST::make(tyCtxt, SymType::KwdBreak, *gloc); return Token::KwdBreak; }
<INITIAL>"return"   { *glval = AST::make(tyCtxt, SymType::KwdReturn, *gloc); return Token::KwdReturn; }


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
    *glval = AST::make(tyCtxt, SymType::FloatLiteral, *gloc, val);
    return Token::FloatLiteral;
}
    
<INITIAL>[0-9]*\.[0-9]+([eE]|[-+]|[\.])+ {
    ASTFloatType val = 0.0;
    SPLC_LOG_ERROR(gloc, true) << "too many decimal points or exponential indicators";
    *glval = AST::make(tyCtxt, SymType::FloatLiteral, *gloc, val);
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
    *glval = AST::make(tyCtxt, SymType::UIntLiteral, *gloc, val);
    return Token::UIntLiteral;
}

<INITIAL>0[xX][0-9a-fA-F]{1,16} {
    ASTUIntType val = 0ULL;
    try {
        val = std::stoull({yytext}, nullptr, 16);
    } catch (std::out_of_range e) {
        SPLC_LOG_ERROR(gloc, true) << e.what();
    }
    *glval = AST::make(tyCtxt, SymType::UIntLiteral, *gloc, val);
    return Token::UIntLiteral;
}

    /* SPL: invalid hexadecimal integer */
<INITIAL>0[xX][0-9a-zA-Z]+ {
    ASTUIntType val = 0ULL;
    SPLC_LOG_ERROR(gloc, true) << "ill-formed hexadecimal integer";
    *glval = AST::make(tyCtxt, SymType::UIntLiteral, *gloc, val);
    return Token::UIntLiteral;
}

    /* =================== SPL: char =================== */
<INITIAL>'\\x[0-9a-fA-F]{2}' {
    ASTCharType val = static_cast<ASTCharType>(std::stoi({yytext + 3}, nullptr, 16));
    *glval = AST::make(tyCtxt, SymType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

<INITIAL>'\\x[0-9a-zA-Z]*' {
    SPLC_LOG_ERROR(gloc, true) << "ill-formed char";
    ASTCharType val = '\0';
    *glval = AST::make(tyCtxt, SymType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

<INITIAL>'\\[abefnrtv\\\'\"\?]' {
    ASTCharType val = yytext[2];
    *glval = AST::make(tyCtxt, SymType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

<INITIAL>'\\0[0-7]{0,2}' {
    ASTCharType val = static_cast<ASTCharType>(std::stoi({yytext + 2}, nullptr, 8));
    *glval = AST::make(tyCtxt, SymType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

<INITIAL>'\\[0-9]{1,3}' {
    SPLC_LOG_ERROR(gloc, true) << "ill-formed char";
    ASTCharType val = '\0';
    *glval = AST::make(tyCtxt, SymType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

<INITIAL>'.' {
    ASTCharType val = yytext[1];
    *glval = AST::make(tyCtxt, SymType::CharLiteral, *gloc, val);
    return Token::CharLiteral;
}

    /*===------------------------------------------------------------------===//
    //                         Operator Declarations
    //===------------------------------------------------------------------===*/

    /* Assignments */
<INITIAL>"="      { *glval = AST::make(tyCtxt, SymType::OpAssign, *gloc); return Token::OpAssign; }
<INITIAL>"*="     { *glval = AST::make(tyCtxt, SymType::OpMulAssign, *gloc); return Token::OpMulAssign; }
<INITIAL>"/="     { *glval = AST::make(tyCtxt, SymType::OpDivAssign, *gloc); return Token::OpDivAssign; }
<INITIAL>"%="     { *glval = AST::make(tyCtxt, SymType::OpModAssign, *gloc); return Token::OpModAssign; }
<INITIAL>"+="     { *glval = AST::make(tyCtxt, SymType::OpPlusAssign, *gloc); return Token::OpPlusAssign; }
<INITIAL>"-="     { *glval = AST::make(tyCtxt, SymType::OpMinusAssign, *gloc); return Token::OpMinusAssign; }
<INITIAL>"<<="    { *glval = AST::make(tyCtxt, SymType::OpLShiftAssign, *gloc); return Token::OpLShiftAssign; }
<INITIAL>">>="    { *glval = AST::make(tyCtxt, SymType::OpRShiftAssign, *gloc); return Token::OpRShiftAssign; }
<INITIAL>"&="     { *glval = AST::make(tyCtxt, SymType::OpBAndAssign, *gloc); return Token::OpBAndAssign; }
<INITIAL>"^="     { *glval = AST::make(tyCtxt, SymType::OpBXorAssign, *gloc); return Token::OpBXorAssign; }
<INITIAL>"|="     { *glval = AST::make(tyCtxt, SymType::OpBOrAssign, *gloc); return Token::OpBOrAssign; }

    /* Conditional */
<INITIAL>"&&"     { *glval = AST::make(tyCtxt, SymType::OpAnd, *gloc); return Token::OpAnd; }
<INITIAL>"||"     { *glval = AST::make(tyCtxt, SymType::OpOr, *gloc); return Token::OpOr; }
<INITIAL>"!"      { *glval = AST::make(tyCtxt, SymType::OpNot, *gloc); return Token::OpNot; }

<INITIAL>"<"      { *glval = AST::make(tyCtxt, SymType::OpLT, *gloc); return Token::OpLT; }
<INITIAL>"<="     { *glval = AST::make(tyCtxt, SymType::OpLE, *gloc); return Token::OpLE; }
<INITIAL>">"      { *glval = AST::make(tyCtxt, SymType::OpGT, *gloc); return Token::OpGT; }
<INITIAL>">="     { *glval = AST::make(tyCtxt, SymType::OpGE, *gloc); return Token::OpGE; }
<INITIAL>"!="     { *glval = AST::make(tyCtxt, SymType::OpNE, *gloc); return Token::OpNE; }
<INITIAL>"=="     { *glval = AST::make(tyCtxt, SymType::OpEQ, *gloc); return Token::OpEQ; }
    
<INITIAL>"?"      { *glval = AST::make(tyCtxt, SymType::OpQMark, *gloc); return Token::OpQMark; }
<INITIAL>":"      { *glval = AST::make(tyCtxt, SymType::OpColon, *gloc); return Token::OpColon; }

    /* Arithmetics */
<INITIAL>"<<"     { *glval = AST::make(tyCtxt, SymType::OpLShift, *gloc); return Token::OpLShift; }
<INITIAL>">>"     { *glval = AST::make(tyCtxt, SymType::OpRShift, *gloc); return Token::OpRShift; }
<INITIAL>"&"      { *glval = AST::make(tyCtxt, SymType::OpBAnd, *gloc); return Token::OpBAnd; }
<INITIAL>"|"      { *glval = AST::make(tyCtxt, SymType::OpBOr, *gloc); return Token::OpBOr; }
<INITIAL>"~"      { *glval = AST::make(tyCtxt, SymType::OpBNot, *gloc); return Token::OpBNot; }
<INITIAL>"^"      { *glval = AST::make(tyCtxt, SymType::OpBXor, *gloc); return Token::OpBXor; }

<INITIAL>"++"     { *glval = AST::make(tyCtxt, SymType::OpDPlus, *gloc); return Token::OpDPlus; }
<INITIAL>"--"     { *glval = AST::make(tyCtxt, SymType::OpDMinus, *gloc); return Token::OpDMinus; }
<INITIAL>"+"      { *glval = AST::make(tyCtxt, SymType::OpPlus, *gloc); return Token::OpPlus; }
<INITIAL>"-"      { *glval = AST::make(tyCtxt, SymType::OpMinus, *gloc); return Token::OpMinus; }
<INITIAL>"*"      { *glval = AST::make(tyCtxt, SymType::OpAstrk, *gloc); return Token::OpAstrk; }
<INITIAL>"/"      { *glval = AST::make(tyCtxt, SymType::OpDiv, *gloc); return Token::OpDiv; }
<INITIAL>"%"      { *glval = AST::make(tyCtxt, SymType::OpMod, *gloc); return Token::OpMod; }

    /* Builtin */
<INITIAL>"."      { *glval = AST::make(tyCtxt, SymType::OpDot, *gloc); return Token::OpDot; }
<INITIAL>"->"     { *glval = AST::make(tyCtxt, SymType::OpRArrow, *gloc); return Token::OpRArrow; }

<INITIAL>"["      { *glval = AST::make(tyCtxt, SymType::OpLSB, *gloc); return Token::OpLSB; }
<INITIAL>"]"      { *glval = AST::make(tyCtxt, SymType::OpRSB, *gloc); return Token::OpRSB; }

<INITIAL>"sizeof" { *glval = AST::make(tyCtxt, SymType::OpSizeOf, *gloc); return Token::OpSizeOf; }

    /* Misc */
<INITIAL>","      { *glval = AST::make(tyCtxt, SymType::OpComma, *gloc); return Token::OpComma; }
<INITIAL>"..."    { *glval = AST::make(tyCtxt, SymType::OpEllipsis, *gloc); return Token::OpEllipsis; }

    /*===------------------------------------------------------------------===//
    //                        Punctuator Declarations
    //===------------------------------------------------------------------===*/
<INITIAL>";"      { *glval = AST::make(tyCtxt, SymType::PSemi, *gloc); return Token::PSemi; }

<INITIAL>"{"      { *glval = AST::make(tyCtxt, SymType::PLC, *gloc); return Token::PLC; }
<INITIAL>"}"      { *glval = AST::make(tyCtxt, SymType::PRC, *gloc); return Token::PRC; }

<INITIAL>"("      { *glval = AST::make(tyCtxt, SymType::PLP, *gloc); return Token::PLP; }
<INITIAL>")"      { *glval = AST::make(tyCtxt, SymType::PRP, *gloc); return Token::PRP; }

    /*===------------------------------------------------------------------===//
    //                           Identifier Definition
    //===------------------------------------------------------------------===*/
<INITIAL>{identifier} {
    String val{yytext};
    /* First, check if it is a macro definition. If it is, just expand. */
    if (isMacroVarContextPresent(val)) {
        splc_assert(pushMacroVarContext(gloc, val));
        yy_push_state(INITIAL);
    }
    else if (transMgr.isSymDeclared(SymEntryType::Typedef, val)) {
        *glval = AST::make(tyCtxt, SymType::TypedefID, *gloc, val);
        return Token::TypedefID;
    }
    else {
        *glval = AST::make(tyCtxt, SymType::ID, *gloc, val);
        return Token::ID;
    }
}

<INITIAL>[0-9][a-zA-Z0-9_]* {
    SPLC_LOG_ERROR(gloc, true) << "identifier name cannot start with digits";
    String val{yytext};
    *glval = AST::make(tyCtxt, SymType::ID, *gloc, val);
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
    *glval = AST::make(tyCtxt, SymType::ID, *gloc, val);
    return Token::ID;
}

<*>.|\n {
    splc_error() << "invalid lexeme. Should be caused by a previous parsing error.";
}

%%

int yyFlexLexer::yywrap()
{
    // Inherit this method to let the code compile, but don't do anything.
    return 1;
}