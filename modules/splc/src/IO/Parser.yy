%skeleton "lalr1.cc"
%require  "3.8.2"
%define parse.trace // This is required for runtime traces. For example, symbol_name.
%define parse.assert

%code requires{
    // Code section there will be placed directly inside `IO/Parser.hh`.
    #include "Core/Utils/LocationWrapper.hh"
    #include "Core/Base.hh"
    namespace splc {
    
    class AST;
    using PtrAST = Ptr<AST>;
    class TranslationManager;

    namespace IO {
    class Driver;
    class Scanner;
    class Parser;
    } // namespace splc::IO

    } // namespace splc
}

%parse-param { TranslationManager  &transMgr }
%parse-param { Driver              &driver  }
%parse-param { Scanner             &scanner  }

%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    
    // include for all driver functions
    #include "Core/splc.hh"

    #include "IO/Driver.hh"

    #include "AST/AST.hh"
    #include "Translation/TranslationManager.hh"

    using Token = splc::IO::Parser::token;
    using SymbolType = splc::ASTSymbolType;

    #undef yylex
    #define yylex scanner.yylex
}

//===----------------------------------------------------------------------===//
//                               API Settings
//===----------------------------------------------------------------------===//
%define api.namespace {splc::IO}
%define api.parser.class {Parser}
// %define api.header.include { "IO/Parser.hh" }
// %define api.location.file "../../include/Core/Utils/location.hh"
%define api.location.type { splc::Location }

%define api.symbol.prefix {} // The empty prefix is generally invalid, but there is namespace in C++.
%define api.value.type { splc::PtrAST }
%locations


//===----------------------------------------------------------------------===//
//                              Token Definitions 
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//===-Storage Qualifiers
%token KwdAuto KwdExtern KwdRegister KwdStatic KwdTypedef

//===----------------------------------------------------------------------===//
//===-Type Qualifiers
%token KwdConst KwdRestrict KwdVolatile

//===----------------------------------------------------------------------===//
//===-Function Specifiers
%token KwdInline

//===----------------------------------------------------------------------===//
//===-Primitive Type Specifiers
%token VoidTy IntTy SignedTy UnsignedTy LongTy FloatTy DoubleTy CharTy
%token KwdEnum 

//===----------------------------------------------------------------------===//
//===-Aggregate Type Specifier
%token KwdStruct KwdUnion

//===----------------------------------------------------------------------===//
//===-Keywords
// Flow Controls
%token KwdIf KwdElse KwdSwitch
%token KwdWhile KwdFor KwdDo
// Labels
%token KwdDefault KwdCase 
// Jumps
%token KwdGoto KwdContinue KwdBreak KwdReturn

//===----------------------------------------------------------------------===//
//===-IDs
%token ID TypedefID 

//===----------------------------------------------------------------------===//
//===-Operators
// Assignments
%token OpAssign 
%token OpMulAssign OpDivAssign OpModAssign OpPlusAssign OpMinusAssign 
%token OpLShiftAssign OpRShiftAssign OpBAndAssign OpBXorAssign OpBOrAssign

// Conditional
%token OpAnd OpOr OpNot
%token OpLT OpLE OpGT OpGE OpNE OpEQ 
%token OpQMark OpColon

// Arithmetics
%token OpLShift OpRShift
%token OpBAnd OpBOr OpBNot OpBXor

%token OpDPlus OpDMinus OpPlus OpMinus OpAstrk OpDiv OpMod

// Builtin
%token OpDot OpRArrow
%token OpSizeOf

// Misc
%token OpComma OpEllipsis

//===----------------------------------------------------------------------===//
//===-Punctuators
%token PuncSemi
%token PuncLCurly PuncRCurly
%token PuncLParen PuncRParen
%token PuncLSBracket PuncRSBracket 

//===-Literals
%token UIntLiteral SIntLiteral FloatLiteral CharLiteral StrUnit


//===----------------------------------------------------------------------===//
//                         Precedence Specification
//===----------------------------------------------------------------------===//
%precedence KwdThen
%precedence KwdElse

%left OpComma
%right OpAssign OpMulAssign OpDivAssign OpModAssign OpPlusAssign OpMinusAssign OpLShiftAssign OpRShiftAssign OpBAndAssign OpBXorAssign OpBOrAssign
%right OpQMark OpColon
%left OpOr
%left OpAnd
%left OpBOr
%left OpBXor
%left OpBAnd
%left OpLT OpLE OpGT OpGE OpNE OpEQ 
%left OpPlus OpMinus
%left OpAstrk OpDiv OpMod
%right OpUnaryPrec
%right OpNot OpBNot OpDPlus OpDMinus OpSizeOf
%left PLParen PRParen PLSBracket PRSBracket OpDot

//===----------------------------------------------------------------------===//
//                              Test Specification
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//                           Production Definitions
//===----------------------------------------------------------------------===//
%%
/* Entire translation unit */
TransUnit: 
      ExternDeclList {}
    | {}
    ;

/* External definition list: Recursive definition */
ExternDeclList: 
      ExternDecl {}
    | ExternDeclList ExternDecl {}
    ;

/* External definition list: A single unit of one of {}. */
ExternDecl: 
      PuncSemi {}
    | Decl {}
    | FuncDef {}
    ;

DeclSpec:
      StorageSpec {}
    | TypeSpec {}
    | TypeQual {}
    | FuncSpec {}
    | DeclSpec TypeSpec {}
    | DeclSpec StorageSpec {}
    | DeclSpec TypeQual {}
    | DeclSpec FuncSpec {}
    ;

StorageSpec:
      KwdAuto {}
    | KwdExtern {}
    | KwdRegister {}
    | KwdStatic {}
    | KwdTypedef {}
    ;

SpecQualList:
      TypeSpec {}
    | TypeQual {}
    | SpecQualList TypeSpec {}
    | SpecQualList TypeQual {}
    ;

TypeSpec: 
      BuiltinTypeSpec {}
    /* | identifier {} */
    | StructOrUnionSpec {}
    | EnumSpec {}
    | TypedefID {}
    ;

FuncSpec:
      KwdInline {}
    ;

TypeQual:
      KwdConst {}
    | KwdRestrict {}
    | KwdVolatile {}
    ;

TypeName:
      SpecQualList {}
    | SpecQualList AbsDecltr {}
    ;

BuiltinTypeSpec:
      VoidTy
    | IntTy
    | FloatTy
    | DoubleTy
    | CharTy
    | SignedTy
    | UnsignedTy
    | LongTy
    ;

AbsDecltr:
      Ptr {}
    | Ptr DirAbsDecltr {}
    ;

DirAbsDecltr:
      PuncLParen AbsDecltr PuncRParen {}
    | DirAbsDecltr PuncLSBracket AssignExpr PuncRSBracket {}
    | DirAbsDecltr PuncLSBracket PuncRSBracket {}
    | DirAbsDecltr PuncLSBracket error {}
    | DirAbsDecltr PuncRSBracket {} 
    ;

/* Specify a structure */
StructOrUnionSpec: 
      StructOrUnion ID {}
    | StructOrUnion StructDeclBody {}
    | StructOrUnion ID StructDeclBody {}
    ;

StructOrUnion:
      KwdStruct
    | KwdUnion
    ;

StructDeclBody:
      PuncLCurly PuncRCurly {}
    | PuncLCurly StructDeclList PuncRCurly {}

    | PuncLCurly error {}
    | PuncLCurly StructDeclList error {}
    ;

StructDeclList:
      StructDecl {}
    | StructDeclList StructDecl {}
    ;

StructDecl:
      SpecQualList PuncSemi {}
    | SpecQualList StructDecltrList PuncSemi {}

    | SpecQualList error {}
    | SpecQualList StructDecltrList error {}
    ;

StructDecltrList:
      StructDecltr {}
    | StructDecltrList OpComma StructDecltr {}

    | StructDecltrList OpComma error {}
    ;

StructDecltr:
      Decltr {}
    | OpColon ConstExpr {}
    | Decltr OpColon ConstExpr {}

    | OpColon error {}
    | Decltr OpColon error {}
    ;

EnumSpec:
      KwdEnum ID {}
    | KwdEnum EnumBody {}
    | KwdEnum ID EnumBody {}
    
    | KwdEnum error {}
    ;

EnumBody:
      PuncLCurly PuncRCurly {}
    | PuncLCurly EnumeratorList PuncRCurly {}
    | PuncLCurly EnumeratorList OpComma PuncRCurly {}

    | PuncLCurly error {}
    | PuncLCurly EnumeratorList error {}
    ;

EnumeratorList:
      Enumerator {}
    | EnumeratorList OpComma Enumerator {}

    | OpComma Enumerator {}
    ;

Enumerator:
      EnumConst {}
    | EnumConst OpAssign ConstExpr {}

    | EnumConst OpAssign error {}
    ;

EnumConst:
      ID
    ;

/* Single variable declaration */
Decltr: 
      Ptr DirDecltr {}
    | DirDecltr {}
    ;

DirDecltr:
      ID {}
    | PuncLParen Decltr PuncRParen {}
    | DirDecltr PuncLSBracket AssignExpr PuncRSBracket {}
    | DirDecltr PuncLSBracket PuncRSBracket {}

    | DirDecltr PuncLSBracket AssignExpr error {} 
    /* | direct-declarator error {}  */
    | DirDecltr PuncRSBracket {} 
    ;

Ptr:
      OpAstrk {}
    | OpAstrk TypeQualList {}
    | Ptr OpAstrk {}
    | Ptr OpAstrk TypeQualList {}
    ;

TypeQualList:
      TypeQual {} 
    | TypeQualList TypeQual {}
    ;

/* Definition: List of definitions. Recursive definition. */
/* declaration-list: 
      declaration {}
    | declaration-list declaration {}
    ; */

/* Definition: Base */
Decl: 
      DirDecl PuncSemi {}
    | DirDecl error {}
    ;

DirDecl:
      DeclSpec {}
    | DeclSpec InitDecltrList {}
    ;

/* Definition: Declaration of multiple variable.  */ 
InitDecltrList: 
      InitDecltr {}
    | InitDecltr OpComma InitDecltrList {}

    | InitDecltr OpComma {}
    | OpComma InitDecltrList {}
    | OpComma {}
    ;

/* Definition: Single declaration unit. */
InitDecltr: 
      Decltr {}
    | Decltr OpAssign Initializer {}
    | Decltr OpAssign error {}
    ;

Initializer:
      AssignExpr {}
    | PuncLCurly InitializerList PuncRCurly {}
    | PuncLCurly InitializerList OpComma PuncRCurly {}
    | PuncLCurly InitializerList error {}
    ;

InitializerList:
      Initializer {}
    | Designation Initializer {}
    | InitializerList OpComma Designation Initializer {}
    | InitializerList OpComma Initializer {}

    | Designation error {}
    | InitializerList OpComma error {}
    ;

Designation:
      DesignatorList OpAssign {}
    ;

DesignatorList:
      Designator {}
    | DesignatorList Designator {}
    ;

Designator:
      PuncLSBracket ConstExpr PuncRSBracket {}
    | OpDot ID {}

    | PuncLSBracket ConstExpr error {}
    | OpDot error {}
    ;

FuncDef:
      DeclSpec FuncDecltr CompStmt {}
    | FuncDecltr CompStmt {} 
    | DeclSpec FuncDecltr PuncSemi {}

    | DeclSpec FuncDecltr error {}
    ;

/* Function: Function name and body. */
FuncDecltr: 
      DirFuncDecltr {}
    | Ptr DirFuncDecltr {}
    ;

DirFuncDecltr:
      DirDecltrForFunc PuncLParen ParamTypeList PuncRParen {}
    /* | direct-declarator-for-function PuncLParen PuncRParen {} */

    /* | direct-declarator-for-function PuncLParen error {} */
    | DirDecltrForFunc PuncLParen ParamTypeList error {}
    /* | direct-declarator-for-function PuncLParen error {} */

    | PuncLParen ParamTypeList PuncRParen {}
    /* | PuncLParen PuncRParen {} */
    ;

DirDecltrForFunc:
      ID {}
    ;

/* List of variables names */
ParamTypeList: 
      {}
    | ParamList {}
    | ParamList OpComma OpEllipsis {}
    ;

ParamList:
      ParamDecl {}
    | ParamList OpComma ParamDecl {}

    | ParamList OpComma error {}
    | OpComma {}
    ;

/* Parameter declaration */ 
ParamDecl: 
      DeclSpec Decltr {}
    | DeclSpec AbsDecltr {}
    | DeclSpec {}

    /* | error {} */
    ;

/* Compound statement: A new scope. */
CompStmt: 
      /* PuncLCurly general-statement-list PuncRCurly */
      PuncLCurly GeneralStmtList PuncRCurly {}
    | PuncLCurly GeneralStmtList error {}
    /* | error PuncRCurly { SPLC_MSG(SPLM_ERR_SYN_B, SPLC_YY2LOC_CF_1_PNT_F(@1), "missing opening bracket '{} */

      /* PuncLCurly PuncRCurly */
    | PuncLCurly PuncRCurly {}
    | PuncLCurly error {}
    ;

/* wrapper for C99 standard for statements */
GeneralStmtList: 
      Stmt {}
    | Decl {}
    | GeneralStmtList Stmt {}
    | GeneralStmtList Decl {}
    ;

/* Statement: List of statements. Recursive definition. */
/* statement-list: 
      statement {}
    | statement-list statement {} 
    ; */

/* Statement: A single statement. */
Stmt: 
      PuncSemi {}
    | CompStmt {}
    | ExprStmt {}
    | SelStmt {}
    | IterStmt {}
    | LabeledStmt {}
    | JumpStmt {}

    /* | error PuncSemi {} */
    ;

ExprStmt:
      Expr PuncSemi {}
    | Expr error {}
    ;

SelStmt:
      KwdIf PuncLParen Expr PuncRParen Stmt %prec KwdThen {}

    | KwdIf error PuncRParen Stmt %prec KwdThen {}
    | KwdIf PuncLParen PuncRParen Stmt %prec KwdThen {}
    | KwdIf PuncLParen Expr PuncRParen error %prec KwdThen {}
    | KwdIf PuncLParen PuncRParen error %prec KwdThen {}
    
    | KwdIf PuncLParen Expr PuncRParen Stmt KwdElse Stmt %prec KwdElse {}

    | KwdIf error PuncRParen Stmt KwdElse Stmt %prec KwdElse {}
    | KwdIf PuncLParen Expr PuncRParen Stmt KwdElse error %prec KwdElse {}
    | KwdIf PuncLParen PuncRParen Stmt KwdElse Stmt %prec KwdElse {}
    | KwdIf PuncLParen PuncRParen Stmt KwdElse error %prec KwdElse {}
    | KwdIf PuncLParen Expr error %prec KwdElse {}
    | KwdElse Stmt {}

    | KwdSwitch PuncLParen Expr PuncRParen Stmt {}
    /* | KwdSwitch PuncLParen expression statement {} */
    | KwdSwitch error PuncRParen Stmt {}
    ;

LabeledStmt:
      ID OpColon Stmt {}
    | KwdCase ConstExpr OpColon Stmt {}
    | KwdDefault OpColon Stmt {}

    | OpColon Stmt {}
    ;

JumpStmt:
      KwdGoto ID PuncSemi {}
    | KwdContinue PuncSemi {}
    | KwdBreak PuncSemi {}
    | KwdReturn Expr PuncSemi {}
    | KwdReturn Expr error {}
    | KwdReturn PuncSemi {}

    | KwdReturn error {}
    ;

IterStmt:
      KwdWhile PuncLParen Expr PuncRParen Stmt {}
    | KwdWhile error PuncRParen Stmt {}
    | KwdWhile PuncLParen Expr PuncRParen error {}
    | KwdWhile PuncLParen Expr error {}
    
    | KwdDo Stmt KwdWhile PuncLParen Expr PuncRParen PuncSemi {}
    | KwdDo Stmt KwdWhile PuncLParen error PuncSemi {}

    | KwdFor PuncLParen ForLoopBody PuncRParen Stmt {}
    | KwdFor PuncLParen ForLoopBody PuncRParen error {}
    | KwdFor PuncLParen ForLoopBody error {}
    ;

ForLoopBody: // TODO: add constant expressions 
      InitExpr PuncSemi Expr PuncSemi Expr {}

    | PuncSemi Expr PuncSemi Expr {} 
    | InitExpr PuncSemi Expr PuncSemi {}
    | InitExpr PuncSemi PuncSemi Expr {}

    | PuncSemi Expr PuncSemi {}
    | PuncSemi PuncSemi Expr {}
    /* | definition PuncSemi {} */
    | InitExpr PuncSemi PuncSemi {}
    
    | PuncSemi PuncSemi {}
    ;

ConstExpr: 
      CondExpr {}
    ;

Constant:
      UIntLiteral {}
    | SIntLiteral {}
    | FloatLiteral {}
    | CharLiteral {}
    /* | StrUnit {} */
    ;

PrimaryExpr:
      ID
    | Constant {}
    | StringLiteral {}
    | PuncLParen Expr PuncRParen {}
    | PuncLParen Expr error {}
    /* | PuncLParen expression {} */
    ;

PostfixExpr:
      PrimaryExpr
    | PostfixExpr PuncLSBracket Expr PuncRSBracket {}
    | PostfixExpr PuncLParen ArgList PuncRParen {}
    /* | postfix-expression PuncLParen PuncRParen {} */
    | PostfixExpr MemberAcessOp ID {}
    | PostfixExpr OpDPlus {}
    | PostfixExpr OpDMinus {}
    | PuncLParen TypeName PuncRParen PuncLCurly InitializerList PuncRCurly {}
    | PuncLParen TypeName PuncRParen PuncLCurly InitializerList OpComma PuncRCurly {}

    | PostfixExpr PuncLSBracket Expr error {}
    | PostfixExpr PuncLParen ArgList error {}
    | PostfixExpr MemberAcessOp {}
    | OpRArrow ID {}
    | PuncLParen TypeName PuncRParen PuncLCurly InitializerList error {}
    ;

MemberAcessOp:
      OpDot
    | OpRArrow
    ;

UnaryExpr:
      PostfixExpr
    | OpDPlus UnaryExpr {}
    | OpDMinus UnaryExpr {}
    | UnaryOp CastExpr %prec OpUnaryPrec {}
    | OpSizeOf UnaryExpr {}
    | OpSizeOf PuncLParen TypeName PuncRParen {}

    | OpBAnd error {}
    | OpAstrk error {}
    | OpBNot error {}
    | OpNot error {}
    | OpDPlus error {}
    | OpDMinus error {}
    | OpSizeOf error {}
    /* | OpSizeOf PuncLParen unary-expression PuncRParen {} */
    ;

UnaryOp: /* Take the default behavior, that is, `$$ = $1` */
      OpBAnd
    | OpAstrk 
    | OpPlus
    | OpMinus
    | OpBNot
    | OpNot
    ;

CastExpr:
      UnaryExpr
    | PuncLParen TypeName PuncRParen CastExpr {}

    | PuncLParen TypeName PuncRParen error {}
    | PuncLParen TypeName error {}
    ;

MulExpr:
      CastExpr
    | MulExpr MulOp CastExpr {}

    | MulExpr MulOp error {}
    | DivOp CastExpr {}
    ;
  
MulOp:
      OpAstrk
    | DivOp
    ;

DivOp:
      OpDiv
    | OpMod
    ;

AddExpr:
      MulExpr
    | AddExpr AddOp MulExpr {}

    | AddExpr AddOp error {}
    ;

AddOp:
      OpPlus
    | OpMinus
    ;

ShiftExpr:
      AddExpr
    | ShiftExpr ShiftOp AddExpr {}

    | ShiftExpr ShiftOp error {}
    | ShiftOp AddExpr {}
    ;
  
ShiftOp:
      OpLShift
    | OpRShift
    ;

RelExpr:
      ShiftExpr
    | RelExpr RelOp ShiftExpr {}

    | RelExpr RelOp error {}
    | RelOp ShiftExpr {}
    ;

RelOp:
      OpLT
    | OpGT
    | OpLE
    | OpGE
    ;

EqualityExpr:
      RelExpr
    | EqualityExpr EqualityOp RelExpr {}

    | EqualityExpr EqualityOp error {}
    | EqualityOp RelExpr {}
    ;

EqualityOp:
      OpEQ
    | OpNE
    ;

OpBAndExpr:
      EqualityExpr
    | OpBAndExpr OpBAnd EqualityExpr {}

    | OpBAndExpr OpBAnd error {}
    ;

OpBXorExpr:
      OpBAndExpr
    | OpBXorExpr OpBXor OpBAndExpr {}

    | OpBXorExpr OpBXor error {}
    | OpBXor OpBAndExpr {}
    ;

OpBOrExpr:
      OpBXorExpr
    | OpBOrExpr OpBOr OpBXorExpr {}

    | OpBOrExpr OpBOr error {}
    | OpBOr OpBXorExpr {}
    ;

LogicalOpAndExpr:
      OpBOrExpr
    | LogicalOpAndExpr OpAnd OpBOrExpr {}

    | LogicalOpAndExpr OpAnd error {}
    | OpAnd OpBOrExpr {}
    ;

LogicalOpOrExpr:
      LogicalOpAndExpr
    | LogicalOpOrExpr OpOr LogicalOpAndExpr {}

    | LogicalOpOrExpr OpOr error {}
    | OpOr LogicalOpAndExpr {}
    ;

CondExpr:
      LogicalOpOrExpr {}
    | LogicalOpOrExpr OpQMark Expr OpColon CondExpr {}

    | LogicalOpOrExpr OpQMark OpColon CondExpr {}
    | LogicalOpOrExpr OpQMark Expr OpColon {}
    | OpQMark error {}
    ;

AssignExpr:
      CondExpr {}
    
    | CondExpr AssignOp AssignExpr {}
    | CondExpr AssignOp error {}
    | AssignOp AssignExpr {}
    
    /* | unary-expression assignment-operator assignment-expression {} */
    /* | unary-expression assignment-operator error {} */
    /* | assignment-operator assignment-expression {} */
    ;
    
AssignOp: /* Use the default behavior to pass the value */
      OpAssign 
    | OpMulAssign
    | OpDivAssign
    | OpModAssign
    | OpPlusAssign
    | OpMinusAssign
    | OpLShiftAssign
    | OpRShiftAssign
    | OpBAndAssign
    | OpBXorAssign
    | OpBOrAssign
    ;

/* expressions */
Expr: 
      AssignExpr {}
    | Expr OpComma AssignExpr {}

    | Expr OpComma error {}
    | OpComma AssignExpr {}
    ;
  
InitExpr:
      Expr {}
    | DirDecl {}
    ;

/* Argument: List of arguments */
ArgList: 
      {}
    | ArgList OpComma AssignExpr {}
    | AssignExpr {}

    | ArgList OpComma error {}
    /* | error {} */
    ;

/* String intermediate expression. Allowing concatenation of strings. */
StringLiteral: 
      StrUnit {}
    | StringLiteral StrUnit {}
    ;
%%


void splc::IO::Parser::error(const location_type &l, const std::string &err_message)
{
    SPLC_LOG_ERROR(&l, true) << err_message;
}