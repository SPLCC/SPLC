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
    class TypeContext;
    class TranslationManager;

    namespace IO {
        class Driver;
        class Scanner;
        class Parser;
    } // namespace splc::IO

    } // namespace splc

    // TODO: finish all error recovery
}

%parse-param { TranslationManager  &transMgr }
%parse-param { Ptr<TypeContext>     tyCtxt   }
%parse-param { Driver              &driver   }
%parse-param { Scanner             &scanner  }

%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    
    // include for all driver functions
    #include "Core/splc.hh"
    #include "IO/Driver.hh"
    #include "AST/DerivedAST.hh"
    #include "Translation/TranslationManager.hh"

    using SymType = splc::ASTSymType;

    #undef yylex
    #define yylex scanner.yylex

    // TODO: allow context pushing/switch
    // TODO: error recovery (instead of applying the default one in Bison)
}

//===----------------------------------------------------------------------===//
//                               API Settings
//===----------------------------------------------------------------------===//
%define api.namespace {splc::IO}
%define api.parser.class {Parser}
// %define api.header.include { "IO/Parser.hh" }
// %define api.location.file "../../include/Core/Utils/location.hh"
%define api.location.type { splc::Location }

%define api.symbol.prefix {S_} // The empty prefix is generally invalid, but there is namespace in C++.
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
%token OpLSB OpRSB 

// Misc
%token OpComma OpEllipsis

//===----------------------------------------------------------------------===//
//===-Punctuators
%token PSemi
%token PLC PRC
%token PLP PRP

//===-Literals
%token UIntLiteral SIntLiteral FloatLiteral CharLiteral StrUnit

//===----------------------------------------------------------------------===//
//                           Additional Tokens
//===----------------------------------------------------------------------===//
%token SubscriptExpr CallExpr AccessExpr 
%token ExplicitCastExpr
%token AddrOfExpr DerefExpr
%token SizeOfExpr

//===----------------------------------------------------------------------===//
//                         Precedence Specification
//===----------------------------------------------------------------------===//
%precedence KwdThen
%precedence KwdElse

%precedence DecltrPrec
%precedence FuncDeclPrec

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
//                             Test Specification
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//                           Production Definitions
//===----------------------------------------------------------------------===//
%%
/* Entire translation unit */
ParseRoot: 
    { transMgr.pushASTCtxt(); } 
    TransUnit {
        transMgr.setRootNode($TransUnit);
        SPLC_LOG_DEBUG(&@TransUnit, true) << "completed parsing";

        $TransUnit->setContext(transMgr.getASTCtxtMgr()[0]);
        transMgr.popASTCtxt();
    }
    ;

TransUnit: 
      ExternDeclList { $$ = AST::make(tyCtxt, SymType::TransUnit, @$, $1); }
    | { $$ = AST::make(tyCtxt, SymType::TransUnit, @$); }
    ;

/* External definition list: Recursive definition */
ExternDeclList: 
      ExternDecl { $$ = AST::make(tyCtxt, SymType::ExternDeclList, @1, $1); }
    | ExternDeclList ExternDecl { $1->addChild($2); $$ = $1; }
    ;

/* External definition list: A single unit of one of {}. */
ExternDecl: 
      PSemi { $$ = AST::make(tyCtxt, SymType::ExternDecl, @$); }
    | Decl { $$ = AST::make(tyCtxt, SymType::ExternDecl, @$, $1); }
    | FuncDef { $$ = AST::make(tyCtxt, SymType::ExternDecl, @$, $1); }
    | FuncDecl { $$ = AST::make(tyCtxt, SymType::ExternDecl, @$, $1); }
    ;

DeclSpec:
      StorageSpec { $$ = AST::make(tyCtxt, SymType::DeclSpec, @$, $1); }
    | TypeSpec { $$ = AST::make(tyCtxt, SymType::DeclSpec, @$, $1); }
    | TypeQual { $$ = AST::make(tyCtxt, SymType::DeclSpec, @$, $1); }
    | FuncSpec { $$ = AST::make(tyCtxt, SymType::DeclSpec, @$, $1); }
    | DeclSpec TypeSpec { $1->addChild($2); $$ = $1; }
    | DeclSpec StorageSpec { $1->addChild($2); $$ = $1; }
    | DeclSpec TypeQual { $1->addChild($2); $$ = $1; }
    | DeclSpec FuncSpec { $1->addChild($2); $$ = $1; }
    ;

StorageSpec:
      KwdAuto { $$ = AST::make(tyCtxt, SymType::StorageSpec, @$, $1); }
    | KwdExtern { $$ = AST::make(tyCtxt, SymType::StorageSpec, @$, $1); }
    | KwdRegister { $$ = AST::make(tyCtxt, SymType::StorageSpec, @$, $1); }
    | KwdStatic { $$ = AST::make(tyCtxt, SymType::StorageSpec, @$, $1); }
    | KwdTypedef { $$ = AST::make(tyCtxt, SymType::StorageSpec, @$, $1); }
    ;

SpecQualList:
      TypeSpec { $$ = AST::make(tyCtxt, SymType::SpecQualList, @$, $1); }
    | TypeQual { $$ = AST::make(tyCtxt, SymType::SpecQualList, @$, $1); }
    | SpecQualList TypeSpec { $1->addChild($2); $$ = $1; }
    | SpecQualList TypeQual { $1->addChild($2); $$ = $1; }
    ;

TypeSpec: 
      BuiltinTypeSpec { $$ = AST::make(tyCtxt, SymType::TypeSpec, @$, $1); }
    /* | identifier {} */
    | StructOrUnionSpec { $$ = AST::make(tyCtxt, SymType::TypeSpec, @$, $1); }
    | EnumSpec { $$ = AST::make(tyCtxt, SymType::TypeSpec, @$, $1); }
    | TypedefID { $$ = AST::make(tyCtxt, SymType::TypeSpec, @$, $1); }
    ;

FuncSpec:
      KwdInline { $$ = AST::make(tyCtxt, SymType::FuncSpec, @$, $1); }
    ;

TypeQual:
      KwdConst { $$ = AST::make(tyCtxt, SymType::TypeQual, @$, $1); }
    | KwdRestrict { $$ = AST::make(tyCtxt, SymType::TypeQual, @$, $1); }
    | KwdVolatile { $$ = AST::make(tyCtxt, SymType::TypeQual, @$, $1); }
    ;

TypeName:
      SpecQualList { $$ = AST::make(tyCtxt, SymType::TypeName, @$, $1); }
    | SpecQualList AbsDecltr { $$ = AST::make(tyCtxt, SymType::TypeQual, @$, $1, $2); }
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
      OpAstrk { $$ = AST::make(tyCtxt, SymType::AbsDecltr, @$, $1); }
    | DirAbsDecltr { $$ = AST::make(tyCtxt, SymType::AbsDecltr, @$, $1); }
    | OpAstrk AbsDecltr { $$ = AST::make(tyCtxt, SymType::AbsDecltr, @$, $1, $2); }
    | OpAstrk TypeQualList AbsDecltr { $$ = AST::make(tyCtxt, SymType::AbsDecltr, @$, $1, $2, $3); }
    ;

DirAbsDecltr:
      PLP AbsDecltr PRP { $$ = AST::make(tyCtxt, SymType::DirAbsDecltr, @$, $1); }
    | DirAbsDecltr OpLSB AssignExpr OpRSB { $$ = AST::make(tyCtxt, SymType::DirAbsDecltr, @$, $1, $2, $3, $4); }
    | DirAbsDecltr OpLSB OpRSB { $$ = AST::make(tyCtxt, SymType::DirAbsDecltr, @$, $1, $2, $3); }
    | DirAbsDecltr PLP ParamList PRP { $$ = AST::make(tyCtxt, SymType::DirAbsDecltr, @$, $1, $3); }
    | PLP ParamList PRP { $$ = AST::make(tyCtxt, SymType::DirAbsDecltr, @$, $2); }
    
    | DirAbsDecltr OpLSB error { SPLC_LOG_ERROR(&@3, true) << "Expect ']' here"; $$ = AST::make(tyCtxt, SymType::DirAbsDecltr, @$, $1); yyerrok; }
    | DirAbsDecltr OpRSB { SPLC_LOG_ERROR(&@2, true) << "Expect '[' here"; $$ = AST::make(tyCtxt, SymType::DirAbsDecltr, @$, $1); yyerrok; } 
    ;

/* Specify a structure */
StructOrUnionSpec: 
      StructOrUnion IDWrapper { $$ = AST::make(tyCtxt, SymType::StructOrUnionSpec, @$, $1, $2); }
    | StructOrUnion StructDeclBody { $$ = AST::make(tyCtxt, SymType::StructOrUnionSpec, @$, $1, $2); }
    | StructOrUnion IDWrapper StructDeclBody { $$ = AST::make(tyCtxt, SymType::StructOrUnionSpec, @$, $1, $2, $3); }
    ;

StructOrUnion:
      KwdStruct
    | KwdUnion
    ;

StructDeclBody:
      PLC PRC { $$ = AST::make(tyCtxt, SymType::StructDeclBody, @$); }
    | PLC StructDeclList PRC { $$ = AST::make(tyCtxt, SymType::StructDeclBody, @$, $1); }

    | PLC error { SPLC_LOG_ERROR(&@1, true) << "expect token '}'"; $$ = AST::make(tyCtxt, SymType::StructDeclBody, @$); yyerrok; }
    | PLC StructDeclList error { SPLC_LOG_ERROR(&@3, true) << "expect token '}'"; $$ = AST::make(tyCtxt, SymType::StructDeclBody, @$, $2); yyerrok; }
    ;

StructDeclList:
      StructDecl { $$ = AST::make(tyCtxt, SymType::StructDeclBody, @$, $1); }
    | StructDeclList StructDecl { $1->addChild($2); $$ = $1; }
    ;

StructDecl:
      SpecQualList PSemi { $$ = AST::make(tyCtxt, SymType::StructDecl, @$, $1); }
    | SpecQualList StructDecltrList PSemi { $$ = AST::make(tyCtxt, SymType::StructDecl, @$, $1, $2); }

    | SpecQualList error {}
    | SpecQualList StructDecltrList error {}
    ;

StructDecltrList:
      StructDecltr { $$ = AST::make(tyCtxt, SymType::StructDecltrList, @$, $1); }
    | StructDecltrList OpComma StructDecltr { $1->addChild($3); $$ = $1; }

    | StructDecltrList OpComma error {}
    ;

StructDecltr:
      Decltr { $$ = AST::make(tyCtxt, SymType::StructDecltr, @$, $1); }
    | OpColon ConstExpr { $$ = AST::make(tyCtxt, SymType::StructDecltr, @$, $1, $2); }
    | Decltr OpColon ConstExpr { $$ = AST::make(tyCtxt, SymType::StructDecltr, @$, $1, $2, $3); }

    | OpColon error {}
    | Decltr OpColon error {}
    ;

EnumSpec:
      KwdEnum IDWrapper { $$ = AST::make(tyCtxt, SymType::EnumSpec, @$, $1, $2); }
    | KwdEnum EnumBody { $$ = AST::make(tyCtxt, SymType::EnumSpec, @$, $1, $2); }
    | KwdEnum IDWrapper EnumBody { $$ = AST::make(tyCtxt, SymType::EnumSpec, @$, $1, $2, $3); }
    
    | KwdEnum error {}
    ;

EnumBody:
      PLC PRC { $$ = AST::make(tyCtxt, SymType::EnumBody, @$); }
    | PLC EnumeratorList PRC { $$ = AST::make(tyCtxt, SymType::EnumBody, @$, $2); }
    | PLC EnumeratorList OpComma PRC { $$ = AST::make(tyCtxt, SymType::EnumBody, @$, $2); }

    | PLC error {}
    | PLC EnumeratorList error {}
    ;

EnumeratorList:
      Enumerator { $$ = AST::make(tyCtxt, SymType::EnumeratorList, @$, $1); }
    | EnumeratorList OpComma Enumerator { $1->addChild($3); $$ = $1; }

    | OpComma Enumerator {}
    ;

Enumerator:
      EnumConst { $$ = AST::make(tyCtxt, SymType::Enumerator, @$, $1); }
    | EnumConst OpAssign ConstExpr { $$ = AST::make(tyCtxt, SymType::Enumerator, @$, $1, $2, $3); }

    | EnumConst OpAssign error {}
    ;

EnumConst:
      IDWrapper { $$ = AST::make(tyCtxt, SymType::EnumConst, @$, $1); }
    ;

/* Single variable declaration */
Decltr: 
      DirDecltr { $$ = AST::make(tyCtxt, SymType::Decltr, @$, $1); }
    | OpAstrk Decltr  { $$ = AST::make(tyCtxt, SymType::Decltr, @$, $1, $2); }
    | OpAstrk TypeQualList Decltr  { $$ = AST::make(tyCtxt, SymType::Decltr, @$, $1, $2, $3); }
    ;

DirDecltr:
      IDWrapper { $$ = AST::make(tyCtxt, SymType::DirDecltr, @$, $1); }
    | WrappedDirDecltr { $$ = AST::make(tyCtxt, SymType::DirDecltr, @$, $1); }
    | DirDecltr OpLSB AssignExpr OpRSB { $$ = AST::make(tyCtxt, SymType::DirDecltr, @$, $1, $2, $3, $4); }
    | DirDecltr OpLSB OpRSB { $$ = AST::make(tyCtxt, SymType::DirDecltr, @$, $1, $2, $3); }
    | WrappedDirDecltr PLP ParamList PRP { 
          $$ = AST::make(tyCtxt, SymType::DirDecltr, @$, $1, $3); 
      }
    | DirDecltr OpLSB AssignExpr error {} 
    /* | direct-declarator error {}  */
    | DirDecltr OpRSB {} 
    ;

WrappedDirDecltr: 
      PLP Decltr PRP { $$ = AST::make(tyCtxt, SymType::WrappedDirDecltr, @$, $2); }
    ;

/* PtrDecltr:
      OpAstrk { $$ = AST::make(tyCtxt, SymType::PtrDecltr, @$, $1); }
    | OpAstrk TypeQualList { $$ = AST::make(tyCtxt, SymType::PtrDecltr, @$, $1, $2); }
    | OpAstrk PtrDecltr { $$ = AST::make(tyCtxt, SymType::PtrDecltr, @$, $1, $2); }
    | OpAstrk TypeQualList PtrDecltr { $$ = AST::make(tyCtxt, SymType::PtrDecltr, @$, $1, $2, $3); }
    ; */

TypeQualList:
      TypeQual { $$ = AST::make(tyCtxt, SymType::TypeQualList, @$, $1); } 
    | TypeQualList TypeQual { $1->addChild($2); $$ = $1; }
    ;

/* Definition: List of definitions. Recursive definition. */
/* declaration-list: 
      declaration {}
    | declaration-list declaration {}
    ; */

/* Definition: Base */
Decl: 
      DirDecl PSemi { $$ = AST::make(tyCtxt, SymType::Decl, @$, $1); transMgr.tryRegisterSymbol($$); }

    | DirDecl error {}
    ;

DirDecl:
      DeclSpec { $$ = AST::make(tyCtxt, SymType::DirDecl, @$, $1); }
    | DeclSpec InitDecltrList { $$ = AST::make(tyCtxt, SymType::DirDecl, @$, $1, $2); }
    ;

/* Definition: Declaration of multiple variable.  */ 
InitDecltrList: 
      InitDecltr { $$ = AST::make(tyCtxt, SymType::InitDecltrList, @$, $1); }
    | InitDecltrList OpComma InitDecltr { $1->addChild($3); $$ = $1; }

    | InitDecltrList OpComma {}
    | OpComma InitDecltr {}
    | OpComma {}
    ;

/* Definition: Single declaration unit. */
InitDecltr: 
      Decltr { $$ = AST::make(tyCtxt, SymType::InitDecltr, @$, $1); }
    | Decltr OpAssign Initializer { $$ = AST::make(tyCtxt, SymType::InitDecltr, @$, $1, $2, $3); }

    | Decltr OpAssign error {}
    ;

Initializer:
      AssignExpr { $$ = AST::make(tyCtxt, SymType::Initializer, @$, $1); }
    | PLC InitializerList PRC { $$ = AST::make(tyCtxt, SymType::Initializer, @$, $2); }
    | PLC InitializerList OpComma PRC { $$ = AST::make(tyCtxt, SymType::Initializer, @$, $2); }

    | PLC InitializerList error {}
    ;

InitializerList:
      Initializer { $$ = AST::make(tyCtxt, SymType::InitializerList, @$, $1); }
    | Designation Initializer { $$ = AST::make(tyCtxt, SymType::InitializerList, @$, $1, $2); }
    | InitializerList OpComma Designation Initializer { $1->addChildren($3, $4) ; $$ = $1; }
    | InitializerList OpComma Initializer { $1->addChild($3) ; $$ = $1; }

    | Designation error {}
    | InitializerList OpComma error {}
    ;

Designation:
      DesignatorList OpAssign { $$ = AST::make(tyCtxt, SymType::Designation, @$, $1, $2); }
    ;

DesignatorList:
      Designator { $$ = AST::make(tyCtxt, SymType::DesignatorList, @$, $1); }
    | DesignatorList Designator { $1->addChild($2) ; $$ = $1; }
    ;

Designator:
      OpLSB ConstExpr OpRSB { $$ = AST::make(tyCtxt, SymType::Designator, @$, $1, $2, $3); }
    | OpDot IDWrapper { $$ = AST::make(tyCtxt, SymType::Designator, @$, $1, $2); }

    | OpLSB ConstExpr error {}
    | OpDot error {}
    ;

FuncDef:
      DeclSpec FuncDecltr CompStmt { $$ = AST::make(tyCtxt, SymType::FuncDef, @$, $1, $2, $3); transMgr.tryRegisterSymbol($$); }
    | FuncDecltr CompStmt { 
          SPLC_LOG_WARN(&@1, true) << "function is missing a specifier and will default to 'int'";
          auto declSpec = ASTHelper::makeDeclSpecifierTree(Location{@$.begin}, SymType::IntTy);
          $$ = AST::make(tyCtxt, SymType::FuncDef, @$, declSpec, $1, $2);
          transMgr.tryRegisterSymbol($$); 
      } 
    | DeclSpec FuncDecltr error {}
    ;

FuncDecl:
      FuncDecltr PSemi { 
          SPLC_LOG_WARN(&@1, true) << "function is missing a specifier and will default to 'int'";
          auto declSpec = ASTHelper::makeDeclSpecifierTree(Location{@$.begin}, SymType::IntTy);
          $$ = AST::make(tyCtxt, SymType::FuncDecl, @$, declSpec, $1);
           transMgr.tryRegisterSymbol($$); 
      } 
    | DeclSpec FuncDecltr PSemi { $$ = AST::make(tyCtxt, SymType::FuncDecl, @$, $1, $2); transMgr.tryRegisterSymbol($$); }
    ;


/* Function: Function name and body. */
FuncDecltr: 
      DirFuncDecltr { $$ = AST::make(tyCtxt, SymType::FuncDecltr, @$, $1); }
    | OpAstrk DirFuncDecltr { $$ = AST::make(tyCtxt, SymType::FuncDecltr, @$, $1, $2); }
    | OpAstrk TypeQualList DirFuncDecltr { $$ = AST::make(tyCtxt, SymType::FuncDecltr, @$, $1, $2, $3); }
    ;

DirFuncDecltr:
      DirDecltrForFunc PLP ParamTypeList PRP { $$ = AST::make(tyCtxt, SymType::DirFuncDecltr, @$, $1, $3); }
    /* | direct-declarator-for-function PLP PRP {} */

    /* | direct-declarator-for-function PLP error {} */
    | DirDecltrForFunc PLP ParamTypeList error {}
    /* | direct-declarator-for-function PLP error {} */

    | PLP ParamTypeList PRP {}
    /* | PLP PRP {} */
    ;

DirDecltrForFunc:
      IDWrapper 
    ;

/* List of variables names */
ParamTypeList: 
      ParamList { $$ = AST::make(tyCtxt, SymType::ParamTypeList, @$, $1); }
    | ParamList OpComma OpEllipsis { $$ = AST::make(tyCtxt, SymType::ParamTypeList, @$, $1, $3); }
    ;

ParamList:
      { $$ = AST::make(tyCtxt, SymType::ParamList, @$); }
    | ParamDecltr { $$ = AST::make(tyCtxt, SymType::ParamList, @$, $1); }
    | ParamList OpComma ParamDecltr { $1->addChild($3); $$ = $1; }

    | ParamList OpComma error {}
    ;

/* Parameter declaration */ 
ParamDecltr: 
      DeclSpec Decltr { $$ = AST::make(tyCtxt, SymType::ParamDecltr, @$, $1, $2); }
    | DeclSpec AbsDecltr { $$ = AST::make(tyCtxt, SymType::ParamDecltr, @$, $1, $2); }
    | DeclSpec { $$ = AST::make(tyCtxt, SymType::ParamDecltr, @$, $1); }

    /* | error {} */
    ;

/* Compound statement: A new scope. */
CompStmt: 
      /* PLC general-statement-list PRC */
      PLC GeneralStmtList PRC { $$ = AST::make(tyCtxt, SymType::CompStmt, @$, $2); }
    | PLC PRC { $$ = AST::make(tyCtxt, SymType::CompStmt, @$); }

    | PLC GeneralStmtList error {}
    | PLC error {}
    ;

/* wrapper for C99 standard for statements */
GeneralStmtList: 
      Stmt { $$ = AST::make(tyCtxt, SymType::GeneralStmtList, @$, $1); }
    | Decl { $$ = AST::make(tyCtxt, SymType::GeneralStmtList, @$, $1); }
    /* | FuncDecl { $$ = AST::make(tyCtxt, SymType::GeneralStmtList, @$, $1); } */
    | GeneralStmtList Stmt { $1->addChild($2); $$ = $1; }
    | GeneralStmtList Decl { $1->addChild($2); $$ = $1; }
    ;

/* Statement: List of statements. Recursive definition. */
/* statement-list: 
      statement {}
    | statement-list statement {} 
    ; */

/* Statement: A single statement. */
Stmt: // TODO: use hierarchy
      PSemi { $$ = AST::make(tyCtxt, SymType::Stmt, @$); }
    | CompStmt { $$ = AST::make(tyCtxt, SymType::Stmt, @$, $1); }
    | ExprStmt { $$ = AST::make(tyCtxt, SymType::Stmt, @$, $1); }
    | SelStmt { $$ = AST::make(tyCtxt, SymType::Stmt, @$, $1); }
    | IterStmt { $$ = AST::make(tyCtxt, SymType::Stmt, @$, $1); }
    | LabeledStmt { $$ = AST::make(tyCtxt, SymType::Stmt, @$, $1); }
    | JumpStmt { $$ = AST::make(tyCtxt, SymType::Stmt, @$, $1); }

    /* | error PSemi {} */
    ;

ExprStmt:
      Expr PSemi { $$ = AST::make(tyCtxt, SymType::ExprStmt, @$, $1); }
    | Expr error {}
    ;

SelStmt:
      KwdIf PLP Expr PRP Stmt %prec KwdThen { $$ = AST::make(tyCtxt, SymType::SelStmt, @$, $1, $3, $5); }

    | KwdIf error PRP Stmt %prec KwdThen {}
    | KwdIf PLP PRP Stmt %prec KwdThen {}
    | KwdIf PLP Expr PRP error %prec KwdThen {}
    | KwdIf PLP PRP error %prec KwdThen {}
    
    | KwdIf PLP Expr PRP Stmt KwdElse Stmt %prec KwdElse { $$ = AST::make(tyCtxt, SymType::SelStmt, @$, $1, $3, $5, $6, $7); }

    | KwdIf error PRP Stmt KwdElse Stmt %prec KwdElse {}
    | KwdIf PLP Expr PRP Stmt KwdElse error %prec KwdElse {}
    | KwdIf PLP PRP Stmt KwdElse Stmt %prec KwdElse {}
    | KwdIf PLP PRP Stmt KwdElse error %prec KwdElse {}
    | KwdIf PLP Expr error %prec KwdElse {}
    | KwdElse Stmt {}

    | KwdSwitch PLP Expr PRP Stmt { $$ = AST::make(tyCtxt, SymType::SelStmt, @$, $KwdSwitch, $Expr, $Stmt); }
    /* | KwdSwitch PLP expression statement {} */
    | KwdSwitch error PRP Stmt {}
    ;

LabeledStmt:
      IDWrapper OpColon Stmt { $$ = AST::make(tyCtxt, SymType::LabeledStmt, @$, $1, $2, $3); }
    | KwdCase ConstExpr OpColon Stmt { $$ = AST::make(tyCtxt, SymType::LabeledStmt, @$, $1, $2, $3, $4); }
    | KwdDefault OpColon Stmt { $$ = AST::make(tyCtxt, SymType::LabeledStmt, @$, $1, $2, $3); }

    | OpColon Stmt {}
    ;

JumpStmt:
      KwdGoto IDWrapper PSemi { $$ = AST::make(tyCtxt, SymType::JumpStmt, @$, $1, $2); }
    | KwdContinue PSemi { $$ = AST::make(tyCtxt, SymType::JumpStmt, @$, $1); }
    | KwdBreak PSemi { $$ = AST::make(tyCtxt, SymType::JumpStmt, @$, $1); }
    | KwdReturn Expr PSemi { $$ = AST::make(tyCtxt, SymType::JumpStmt, @$, $1, $2); }
    | KwdReturn PSemi { $$ = AST::make(tyCtxt, SymType::JumpStmt, @$, $1); }

    | KwdReturn Expr error {}
    | KwdReturn error {}
    ;

IterStmt:
      KwdWhile PLP Expr PRP Stmt { $$ = AST::make(tyCtxt, SymType::IterStmt, @$, $KwdWhile, $Expr, $Stmt); }
    | KwdWhile error PRP Stmt {}
    | KwdWhile PLP Expr PRP error {}
    | KwdWhile PLP Expr error {}
    
    | KwdDo Stmt KwdWhile PLP Expr PRP PSemi { $$ = AST::make(tyCtxt, SymType::IterStmt, @$, $KwdDo, $Stmt, $KwdWhile, $Expr); }
    | KwdDo Stmt KwdWhile PLP error PSemi {}

    | KwdFor PLP ForLoopBody PRP Stmt { $$ = AST::make(tyCtxt, SymType::IterStmt, @$, $KwdFor, $ForLoopBody, $Stmt); }
    | KwdFor PLP ForLoopBody PRP error {}
    | KwdFor PLP ForLoopBody error {}
    ;

ForLoopBody: // TODO: add constant expressions 
      InitExpr PSemi Expr PSemi Expr { $$ = AST::make(tyCtxt, SymType::ForLoopBody, @$, $1, $2, $3, $4, $5); }

    | PSemi Expr PSemi Expr { $$ = AST::make(tyCtxt, SymType::ForLoopBody, @$, $1, $2, $3, $4); } 
    | InitExpr PSemi Expr PSemi { $$ = AST::make(tyCtxt, SymType::ForLoopBody, @$, $1, $2, $3, $4); }
    | InitExpr PSemi PSemi Expr { $$ = AST::make(tyCtxt, SymType::ForLoopBody, @$, $1, $2, $3, $4); }

    | PSemi Expr PSemi { $$ = AST::make(tyCtxt, SymType::ForLoopBody, @$, $1, $2, $3); }
    | PSemi PSemi Expr { $$ = AST::make(tyCtxt, SymType::ForLoopBody, @$, $1, $2, $3); }
    /* | definition PSemi {} */
    | InitExpr PSemi PSemi { $$ = AST::make(tyCtxt, SymType::ForLoopBody, @$, $1, $2, $3); }
    
    | PSemi PSemi { $$ = AST::make(tyCtxt, SymType::ForLoopBody, @$, $1, $2); }
    ;

ConstExpr: 
      CondExpr { $$ = AST::make(tyCtxt, SymType::ConstExpr, @$, $1); }
    ;

Constant:
      UIntLiteral { $$ = AST::make(tyCtxt, SymType::Constant, @$, $1); }
    | SIntLiteral { $$ = AST::make(tyCtxt, SymType::Constant, @$, $1); }
    | FloatLiteral { $$ = AST::make(tyCtxt, SymType::Constant, @$, $1); }
    | CharLiteral { $$ = AST::make(tyCtxt, SymType::Constant, @$, $1); }
    /* | StrUnit {} */
    ;

PrimaryExpr:
      IDWrapper { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1); }
    | Constant { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1); }
    | StringLiteral { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1); }
    | PLP Expr PRP { $$ = AST::make(tyCtxt, SymType::Expr, @$, $2); }

    | PLP Expr error {}
    /* | PLP expression {} */
    ;

PostfixExpr:
      PrimaryExpr
    | PostfixExpr OpLSB Expr OpRSB { $$ = AST::make(tyCtxt, SymType::SubscriptExpr, @$, $1, $2, $3, $4); }
    | PostfixExpr PLP ArgList PRP { $$ = AST::make(tyCtxt, SymType::CallExpr, @$, $1, $3); }
    /* | postfix-expression PLP PRP {} */
    | PostfixExpr MemberAcessOp IDWrapper { $$ = AST::make(tyCtxt, SymType::AccessExpr, @$, $1, $2, $3); }
    | PostfixExpr OpDPlus { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2); }
    | PostfixExpr OpDMinus { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2); }
    | PLP TypeName PRP PLC InitializerList PRC { $$ = AST::make(tyCtxt, SymType::ExplicitCastExpr, @$, $1, $2, $3, $5); }
    | PLP TypeName PRP PLC InitializerList OpComma PRC { $$ = AST::make(tyCtxt, SymType::ExplicitCastExpr, @$, $1, $2, $3, $5); }

    | PostfixExpr OpLSB Expr error {}
    | PostfixExpr PLP ArgList error {}
    | PostfixExpr MemberAcessOp {}
    | OpRArrow IDWrapper {}
    | PLP TypeName PRP PLC InitializerList error {}
    ;

MemberAcessOp:
      OpDot
    | OpRArrow
    ;

UnaryExpr:
      PostfixExpr
    | OpDPlus UnaryExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2); }
    | OpDMinus UnaryExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2); }
    | OpBAnd CastExpr %prec OpUnaryPrec { $$ = AST::make(tyCtxt, SymType::AddrOfExpr, @$, $1, $2); }
    | OpAstrk CastExpr %prec OpUnaryPrec { $$ = AST::make(tyCtxt, SymType::DerefExpr, @$, $1, $2); }
    | UnaryArithOp CastExpr %prec OpUnaryPrec { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2); }
    | OpSizeOf UnaryExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2); }
    | OpSizeOf PLP TypeName PRP { $$ = AST::make(tyCtxt, SymType::SizeOfExpr, @$, $1, $3); }

    | OpBAnd error {}
    | OpAstrk error {}
    | OpBNot error {}
    | OpNot error {}
    | OpDPlus error {}
    | OpDMinus error {}
    | OpSizeOf error {}
    /* | OpSizeOf PLP unary-expression PRP {} */
    ;

UnaryArithOp: /* Take the default behavior, that is, `$$ = $1` */
      OpPlus
    | OpMinus
    | OpBNot
    | OpNot
    ;


CastExpr:
      UnaryExpr
    | PLP TypeName PRP CastExpr { $$ = AST::make(tyCtxt, SymType::ExplicitCastExpr, @$, $1, $2); }

    | PLP TypeName PRP error {}
    | PLP TypeName error {}
    ;

MulExpr:
      CastExpr
    | MulExpr MulOp CastExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }

    | MulExpr MulOp error {}
    | DivOp CastExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2); }
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
    | AddExpr AddOp MulExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }

    | AddExpr AddOp error {}
    ;

AddOp:
      OpPlus
    | OpMinus
    ;

ShiftExpr:
      AddExpr
    | ShiftExpr ShiftOp AddExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }

    | ShiftExpr ShiftOp error {}
    | ShiftOp AddExpr {}
    ;
  
ShiftOp:
      OpLShift
    | OpRShift
    ;

RelExpr:
      ShiftExpr
    | RelExpr RelOp ShiftExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }

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
    | EqualityExpr EqualityOp RelExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }

    | EqualityExpr EqualityOp error {}
    | EqualityOp RelExpr {}
    ;

EqualityOp:
      OpEQ
    | OpNE
    ;

OpBAndExpr:
      EqualityExpr
    | OpBAndExpr OpBAnd EqualityExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }

    | OpBAndExpr OpBAnd error {}
    ;

OpBXorExpr:
      OpBAndExpr
    | OpBXorExpr OpBXor OpBAndExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }

    | OpBXorExpr OpBXor error {}
    | OpBXor OpBAndExpr {}
    ;

OpBOrExpr:
      OpBXorExpr
    | OpBOrExpr OpBOr OpBXorExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }

    | OpBOrExpr OpBOr error {}
    | OpBOr OpBXorExpr {}
    ;

LogicalOpAndExpr:
      OpBOrExpr
    | LogicalOpAndExpr OpAnd OpBOrExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }

    | LogicalOpAndExpr OpAnd error {}
    | OpAnd OpBOrExpr {}
    ;

LogicalOpOrExpr:
      LogicalOpAndExpr
    | LogicalOpOrExpr OpOr LogicalOpAndExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }

    | LogicalOpOrExpr OpOr error {}
    | OpOr LogicalOpAndExpr {}
    ;

CondExpr:
      LogicalOpOrExpr
    | LogicalOpOrExpr OpQMark Expr OpColon CondExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3, $4, $5); }

    | LogicalOpOrExpr OpQMark OpColon CondExpr {}
    | LogicalOpOrExpr OpQMark Expr OpColon {}
    | OpQMark error {}
    ;

AssignExpr:
      CondExpr
    
    | CondExpr AssignOp AssignExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $2, $3); }
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
      AssignExpr
    | Expr OpComma AssignExpr { $$ = AST::make(tyCtxt, SymType::Expr, @$, $1, $3); }

    | Expr OpComma error {}
    | OpComma AssignExpr {}
    ;
  
InitExpr:
      Expr { $$ = AST::make(tyCtxt, SymType::InitExpr, @$, $1); }
    | DirDecl { $$ = AST::make(tyCtxt, SymType::InitExpr, @$, $1); }
    ;

/* Argument: List of arguments */
ArgList: 
      { $$ = AST::make(tyCtxt, SymType::ArgList, @$); }
    | ArgList OpComma AssignExpr { $1->addChild($3); $$ = $1; }
    | AssignExpr { $$ = AST::make(tyCtxt, SymType::ArgList, @$, $1); }

    | ArgList OpComma error {}
    /* | error {} */
    ;

/* String intermediate expression. Allowing concatenation of strings. */
StringLiteral: 
      StrUnit { $$ = AST::make(tyCtxt, SymType::StringLiteral, @$, $1); }
    | StringLiteral StrUnit { $1->addChild($2); $$ = $1; }
    ;

IDWrapper:
      ID
    ;
%%


void splc::IO::Parser::error(const location_type &l, const std::string &err_message)
{
    SPLC_LOG_ERROR(&l, true) << err_message;
}