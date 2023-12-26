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

    // TODO: finish all error recovery
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

    using SType = splc::ASTSymbolType;

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
//                              Test Specification
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//                           Production Definitions
//===----------------------------------------------------------------------===//
%%
/* Entire translation unit */
ParseRoot: 
    { transMgr.pushASTContext(); } 
    TransUnit {
        transMgr.setRootNode($TransUnit);
        SPLC_LOG_DEBUG(&@TransUnit, true) << "completed parsing";

        $TransUnit->setASTContext(transMgr.getCurrentASTContext()); 
        transMgr.popASTContext();
    }
    ;

TransUnit: 
      ExternDeclList { $$ = transMgr.makeAST<AST>(SType::TransUnit, @$, $1); }
    | { $$ = transMgr.makeAST<AST>(SType::TransUnit, @$); }
    ;

/* External definition list: Recursive definition */
ExternDeclList: 
      ExternDecl { $$ = transMgr.makeAST<AST>(SType::ExternDeclList, @1, $1); }
    | ExternDeclList ExternDecl { $1->addChild($2); $$ = $1; }
    ;

/* External definition list: A single unit of one of {}. */
ExternDecl: 
      PSemi { $$ = transMgr.makeAST<AST>(SType::ExternDecl, @$); }
    | Decl { $$ = transMgr.makeAST<AST>(SType::ExternDecl, @$, $1); }
    | FuncDef { $$ = transMgr.makeAST<AST>(SType::ExternDecl, @$, $1); }
    | FuncDecl { $$ = transMgr.makeAST<AST>(SType::ExternDecl, @$, $1); }
    ;

DeclSpec:
      StorageSpec { $$ = transMgr.makeAST<AST>(SType::DeclSpec, @$, $1); }
    | TypeSpec { $$ = transMgr.makeAST<AST>(SType::DeclSpec, @$, $1); }
    | TypeQual { $$ = transMgr.makeAST<AST>(SType::DeclSpec, @$, $1); }
    | FuncSpec { $$ = transMgr.makeAST<AST>(SType::DeclSpec, @$, $1); }
    | DeclSpec TypeSpec { $1->addChild($2); $$ = $1; }
    | DeclSpec StorageSpec { $1->addChild($2); $$ = $1; }
    | DeclSpec TypeQual { $1->addChild($2); $$ = $1; }
    | DeclSpec FuncSpec { $1->addChild($2); $$ = $1; }
    ;

StorageSpec:
      KwdAuto { $$ = transMgr.makeAST<AST>(SType::StorageSpec, @$, $1); }
    | KwdExtern { $$ = transMgr.makeAST<AST>(SType::StorageSpec, @$, $1); }
    | KwdRegister { $$ = transMgr.makeAST<AST>(SType::StorageSpec, @$, $1); }
    | KwdStatic { $$ = transMgr.makeAST<AST>(SType::StorageSpec, @$, $1); }
    | KwdTypedef { $$ = transMgr.makeAST<AST>(SType::StorageSpec, @$, $1); }
    ;

SpecQualList:
      TypeSpec { $$ = transMgr.makeAST<AST>(SType::SpecQualList, @$, $1); }
    | TypeQual { $$ = transMgr.makeAST<AST>(SType::SpecQualList, @$, $1); }
    | SpecQualList TypeSpec { $1->addChild($2); $$ = $1; }
    | SpecQualList TypeQual { $1->addChild($2); $$ = $1; }
    ;

TypeSpec: 
      BuiltinTypeSpec { $$ = transMgr.makeAST<AST>(SType::TypeSpec, @$, $1); }
    /* | identifier {} */
    | StructOrUnionSpec { $$ = transMgr.makeAST<AST>(SType::TypeSpec, @$, $1); }
    | EnumSpec { $$ = transMgr.makeAST<AST>(SType::TypeSpec, @$, $1); }
    | TypedefID { $$ = transMgr.makeAST<AST>(SType::TypeSpec, @$, $1); }
    ;

FuncSpec:
      KwdInline { $$ = transMgr.makeAST<AST>(SType::FuncSpec, @$, $1); }
    ;

TypeQual:
      KwdConst { $$ = transMgr.makeAST<AST>(SType::TypeQual, @$, $1); }
    | KwdRestrict { $$ = transMgr.makeAST<AST>(SType::TypeQual, @$, $1); }
    | KwdVolatile { $$ = transMgr.makeAST<AST>(SType::TypeQual, @$, $1); }
    ;

TypeName:
      SpecQualList { $$ = transMgr.makeAST<AST>(SType::TypeName, @$, $1); }
    | SpecQualList AbsDecltr { $$ = transMgr.makeAST<AST>(SType::TypeQual, @$, $1, $2); }
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
      PtrDecltr { $$ = transMgr.makeAST<AST>(SType::AbsDecltr, @$, $1); }
    | PtrDecltr DirAbsDecltr { 
        // Let PtrDecltr become the parent of this node.
        auto ptrDeclRoot = ASTHelper::getPtrDeclEndPoint(*$1);
        ptrDeclRoot->addChild($2);
        $$ = transMgr.makeAST<AST>(SType::AbsDecltr, @$, ptrDeclRoot);
    }
    ;

DirAbsDecltr:
      PLP AbsDecltr PRP { $$ = transMgr.makeAST<AST>(SType::DirAbsDecltr, @$, $1); }
    | DirAbsDecltr OpLSB AssignExpr OpRSB { $$ = transMgr.makeAST<AST>(SType::DirAbsDecltr, @$, $1, $2, $3, $4); }
    | DirAbsDecltr OpLSB OpRSB { $$ = transMgr.makeAST<AST>(SType::DirAbsDecltr, @$, $1, $2, $3); }
    | DirAbsDecltr PLP ParamList PRP { $$ = transMgr.makeAST<AST>(SType::DirAbsDecltr, @$, $1, $3); }
    | PLP ParamList PRP { $$ = transMgr.makeAST<AST>(SType::DirAbsDecltr, @$, $2); }
    
    | DirAbsDecltr OpLSB error { SPLC_LOG_ERROR(&@3, true) << "Expect ']' here"; $$ = transMgr.makeAST<AST>(SType::DirAbsDecltr, @$, $1); yyerrok; }
    | DirAbsDecltr OpRSB { SPLC_LOG_ERROR(&@2, true) << "Expect '[' here"; $$ = transMgr.makeAST<AST>(SType::DirAbsDecltr, @$, $1); yyerrok; } 
    ;

/* Specify a structure */
StructOrUnionSpec: 
      StructOrUnion IDWrapper { $$ = transMgr.makeAST<AST>(SType::StructOrUnionSpec, @$, $1, $2); }
    | StructOrUnion StructDeclBody { $$ = transMgr.makeAST<AST>(SType::StructOrUnionSpec, @$, $1, $2); }
    | StructOrUnion IDWrapper StructDeclBody { $$ = transMgr.makeAST<AST>(SType::StructOrUnionSpec, @$, $1, $2, $3); }
    ;

StructOrUnion:
      KwdStruct
    | KwdUnion
    ;

StructDeclBody:
      PLC PRC { $$ = transMgr.makeAST<AST>(SType::StructDeclBody, @$); }
    | PLC StructDeclList PRC { $$ = transMgr.makeAST<AST>(SType::StructDeclBody, @$, $1); }

    | PLC error { SPLC_LOG_ERROR(&@1, true) << "expect token '}'"; $$ = transMgr.makeAST<AST>(SType::StructDeclBody, @$); yyerrok; }
    | PLC StructDeclList error { SPLC_LOG_ERROR(&@3, true) << "expect token '}'"; $$ = transMgr.makeAST<AST>(SType::StructDeclBody, @$, $2); yyerrok; }
    ;

StructDeclList:
      StructDecl { $$ = transMgr.makeAST<AST>(SType::StructDeclBody, @$, $1); }
    | StructDeclList StructDecl { $1->addChild($2); $$ = $1; }
    ;

StructDecl:
      SpecQualList PSemi { $$ = transMgr.makeAST<AST>(SType::StructDecl, @$, $1); }
    | SpecQualList StructDecltrList PSemi { $$ = transMgr.makeAST<AST>(SType::StructDecl, @$, $1, $2); }

    | SpecQualList error {}
    | SpecQualList StructDecltrList error {}
    ;

StructDecltrList:
      StructDecltr { $$ = transMgr.makeAST<AST>(SType::StructDecltrList, @$, $1); }
    | StructDecltrList OpComma StructDecltr { $1->addChild($3); $$ = $1; }

    | StructDecltrList OpComma error {}
    ;

StructDecltr:
      Decltr { $$ = transMgr.makeAST<AST>(SType::StructDecltr, @$, $1); }
    | OpColon ConstExpr { $$ = transMgr.makeAST<AST>(SType::StructDecltr, @$, $1, $2); }
    | Decltr OpColon ConstExpr { $$ = transMgr.makeAST<AST>(SType::StructDecltr, @$, $1, $2, $3); }

    | OpColon error {}
    | Decltr OpColon error {}
    ;

EnumSpec:
      KwdEnum IDWrapper { $$ = transMgr.makeAST<AST>(SType::EnumSpec, @$, $1, $2); }
    | KwdEnum EnumBody { $$ = transMgr.makeAST<AST>(SType::EnumSpec, @$, $1, $2); }
    | KwdEnum IDWrapper EnumBody { $$ = transMgr.makeAST<AST>(SType::EnumSpec, @$, $1, $2, $3); }
    
    | KwdEnum error {}
    ;

EnumBody:
      PLC PRC { $$ = transMgr.makeAST<AST>(SType::EnumBody, @$); }
    | PLC EnumeratorList PRC { $$ = transMgr.makeAST<AST>(SType::EnumBody, @$, $2); }
    | PLC EnumeratorList OpComma PRC { $$ = transMgr.makeAST<AST>(SType::EnumBody, @$, $2); }

    | PLC error {}
    | PLC EnumeratorList error {}
    ;

EnumeratorList:
      Enumerator { $$ = transMgr.makeAST<AST>(SType::EnumeratorList, @$, $1); }
    | EnumeratorList OpComma Enumerator { $1->addChild($3); $$ = $1; }

    | OpComma Enumerator {}
    ;

Enumerator:
      EnumConst { $$ = transMgr.makeAST<AST>(SType::Enumerator, @$, $1); }
    | EnumConst OpAssign ConstExpr { $$ = transMgr.makeAST<AST>(SType::Enumerator, @$, $1, $2, $3); }

    | EnumConst OpAssign error {}
    ;

EnumConst:
      IDWrapper { $$ = transMgr.makeAST<AST>(SType::EnumConst, @$, $1); }
    ;

/* Single variable declaration */
Decltr: 
      DirDecltr { $$ = transMgr.makeAST<AST>(SType::Decltr, @$, $1); }
    | PtrDecltr DirDecltr  { 
        // Let PtrDecltr become the parent of this node.
        auto ptrDeclEndPoint = ASTHelper::getPtrDeclEndPoint(*$1);
        ptrDeclEndPoint->addChild($2);
        $$ = transMgr.makeAST<AST>(SType::Decltr, @$, $1);
    }
    ;

DirDecltr:
      IDWrapper { $$ = transMgr.makeAST<AST>(SType::DirDecltr, @$, $1); }
    | WrappedDirDecltr { $$ = transMgr.makeAST<AST>(SType::DirDecltr, @$, $1); }
    | DirDecltr OpLSB AssignExpr OpRSB { $$ = transMgr.makeAST<AST>(SType::DirDecltr, @$, $1, $2, $3, $4); }
    | DirDecltr OpLSB OpRSB { $$ = transMgr.makeAST<AST>(SType::DirDecltr, @$, $1, $2, $3); }
    | WrappedDirDecltr PLP ParamList PRP { 
          $$ = transMgr.makeAST<AST>(SType::DirDecltr, @$, $1, $3); 
      }
    | DirDecltr OpLSB AssignExpr error {} 
    /* | direct-declarator error {}  */
    | DirDecltr OpRSB {} 
    ;

WrappedDirDecltr: 
      PLP Decltr PRP { $$ = transMgr.makeAST<AST>(SType::WrappedDirDecltr, @$, $2); }
    ;

PtrDecltr:
      OpAstrk { $$ = transMgr.makeAST<AST>(SType::PtrDecltr, @$, $1); }
    | OpAstrk TypeQualList { $$ = transMgr.makeAST<AST>(SType::PtrDecltr, @$, $1, $2); }
    | OpAstrk PtrDecltr { $$ = transMgr.makeAST<AST>(SType::PtrDecltr, @$, $1, $2); }
    | OpAstrk TypeQualList PtrDecltr { $$ = transMgr.makeAST<AST>(SType::PtrDecltr, @$, $1, $2, $3); }
    ;

TypeQualList:
      TypeQual { $$ = transMgr.makeAST<AST>(SType::TypeQualList, @$, $1); } 
    | TypeQualList TypeQual { $1->addChild($2); $$ = $1; }
    ;

/* Definition: List of definitions. Recursive definition. */
/* declaration-list: 
      declaration {}
    | declaration-list declaration {}
    ; */

/* Definition: Base */
Decl: 
      DirDecl PSemi { $$ = transMgr.makeAST<AST>(SType::Decl, @$, $1); transMgr.tryRegisterSymbol($$); }

    | DirDecl error {}
    ;

DirDecl:
      DeclSpec { $$ = transMgr.makeAST<AST>(SType::DirDecl, @$, $1); }
    | DeclSpec InitDecltrList { $$ = transMgr.makeAST<AST>(SType::DirDecl, @$, $1, $2); }
    ;

/* Definition: Declaration of multiple variable.  */ 
InitDecltrList: 
      InitDecltr { $$ = transMgr.makeAST<AST>(SType::InitDecltrList, @$, $1); }
    | InitDecltrList OpComma InitDecltr { $1->addChild($3); $$ = $1; }

    | InitDecltrList OpComma {}
    | OpComma InitDecltr {}
    | OpComma {}
    ;

/* Definition: Single declaration unit. */
InitDecltr: 
      Decltr { $$ = transMgr.makeAST<AST>(SType::InitDecltr, @$, $1); }
    | Decltr OpAssign Initializer { $$ = transMgr.makeAST<AST>(SType::InitDecltr, @$, $1, $2, $3); }

    | Decltr OpAssign error {}
    ;

Initializer:
      AssignExpr { $$ = transMgr.makeAST<AST>(SType::Initializer, @$, $1); }
    | PLC InitializerList PRC { $$ = transMgr.makeAST<AST>(SType::Initializer, @$, $2); }
    | PLC InitializerList OpComma PRC { $$ = transMgr.makeAST<AST>(SType::Initializer, @$, $2); }

    | PLC InitializerList error {}
    ;

InitializerList:
      Initializer { $$ = transMgr.makeAST<AST>(SType::InitializerList, @$, $1); }
    | Designation Initializer { $$ = transMgr.makeAST<AST>(SType::InitializerList, @$, $1, $2); }
    | InitializerList OpComma Designation Initializer { $1->addChildren($3, $4) ; $$ = $1; }
    | InitializerList OpComma Initializer { $1->addChild($3) ; $$ = $1; }

    | Designation error {}
    | InitializerList OpComma error {}
    ;

Designation:
      DesignatorList OpAssign { $$ = transMgr.makeAST<AST>(SType::Designation, @$, $1, $2); }
    ;

DesignatorList:
      Designator { $$ = transMgr.makeAST<AST>(SType::DesignatorList, @$, $1); }
    | DesignatorList Designator { $1->addChild($2) ; $$ = $1; }
    ;

Designator:
      OpLSB ConstExpr OpRSB { $$ = transMgr.makeAST<AST>(SType::Designator, @$, $1, $2, $3); }
    | OpDot IDWrapper { $$ = transMgr.makeAST<AST>(SType::Designator, @$, $1, $2); }

    | OpLSB ConstExpr error {}
    | OpDot error {}
    ;

FuncDef:
      DeclSpec FuncDecltr CompStmt { $$ = transMgr.makeAST<AST>(SType::FuncDef, @$, $1, $2, $3); transMgr.tryRegisterSymbol($$); }
    | FuncDecltr CompStmt { 
          SPLC_LOG_WARN(&@1, true) << "function is missing a specifier and will default to 'int'";
          auto declSpec = ASTHelper::makeDeclSpecifierTree(Location{@$.begin}, SType::IntTy);
          $$ = transMgr.makeAST<AST>(SType::FuncDef, @$, declSpec, $1, $2);
          transMgr.tryRegisterSymbol($$); 
      } 
    | DeclSpec FuncDecltr error {}
    ;

FuncDecl:
      FuncDecltr PSemi { 
          SPLC_LOG_WARN(&@1, true) << "function is missing a specifier and will default to 'int'";
          auto declSpec = ASTHelper::makeDeclSpecifierTree(Location{@$.begin}, SType::IntTy);
          $$ = transMgr.makeAST<AST>(SType::FuncDecl, @$, declSpec, $1);
           transMgr.tryRegisterSymbol($$); 
      } 
    | DeclSpec FuncDecltr PSemi { $$ = transMgr.makeAST<AST>(SType::FuncDecl, @$, $1, $2); transMgr.tryRegisterSymbol($$); }
    ;


/* Function: Function name and body. */
FuncDecltr: 
      DirFuncDecltr { $$ = transMgr.makeAST<AST>(SType::FuncDecltr, @$, $1); }
    | PtrDecltr DirFuncDecltr { 
        // Let PtrDecltr become the parent of this node.
        auto ptrDeclRoot = ASTHelper::getPtrDeclEndPoint(*$1);
        ptrDeclRoot->addChild($2);
        $$ = transMgr.makeAST<AST>(SType::FuncDecltr, @$, ptrDeclRoot);
    }
    ;

DirFuncDecltr:
      DirDecltrForFunc PLP ParamTypeList PRP { $$ = transMgr.makeAST<AST>(SType::DirFuncDecltr, @$, $1, $3); }
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
      ParamList { $$ = transMgr.makeAST<AST>(SType::ParamTypeList, @$, $1); }
    | ParamList OpComma OpEllipsis { $$ = transMgr.makeAST<AST>(SType::ParamTypeList, @$, $1, $3); }
    ;

ParamList:
      { $$ = transMgr.makeAST<AST>(SType::ParamList, @$); }
    | ParamDecltr { $$ = transMgr.makeAST<AST>(SType::ParamList, @$, $1); }
    | ParamList OpComma ParamDecltr { $1->addChild($3); $$ = $1; }

    | ParamList OpComma error {}
    ;

/* Parameter declaration */ 
ParamDecltr: 
      DeclSpec Decltr { $$ = transMgr.makeAST<AST>(SType::ParamDecltr, @$, $1, $2); }
    | DeclSpec AbsDecltr { $$ = transMgr.makeAST<AST>(SType::ParamDecltr, @$, $1, $2); }
    | DeclSpec { $$ = transMgr.makeAST<AST>(SType::ParamDecltr, @$, $1); }

    /* | error {} */
    ;

/* Compound statement: A new scope. */
CompStmt: 
      /* PLC general-statement-list PRC */
      PLC GeneralStmtList PRC { $$ = transMgr.makeAST<AST>(SType::CompStmt, @$, $2); }
    | PLC PRC { $$ = transMgr.makeAST<AST>(SType::CompStmt, @$); }

    | PLC GeneralStmtList error {}
    | PLC error {}
    ;

/* wrapper for C99 standard for statements */
GeneralStmtList: 
      Stmt { $$ = transMgr.makeAST<AST>(SType::GeneralStmtList, @$, $1); }
    | Decl { $$ = transMgr.makeAST<AST>(SType::GeneralStmtList, @$, $1); }
    /* | FuncDecl { $$ = transMgr.makeAST<AST>(SType::GeneralStmtList, @$, $1); } */
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
      PSemi { $$ = transMgr.makeAST<AST>(SType::Stmt, @$); }
    | CompStmt { $$ = transMgr.makeAST<AST>(SType::Stmt, @$, $1); }
    | ExprStmt { $$ = transMgr.makeAST<AST>(SType::Stmt, @$, $1); }
    | SelStmt { $$ = transMgr.makeAST<AST>(SType::Stmt, @$, $1); }
    | IterStmt { $$ = transMgr.makeAST<AST>(SType::Stmt, @$, $1); }
    | LabeledStmt { $$ = transMgr.makeAST<AST>(SType::Stmt, @$, $1); }
    | JumpStmt { $$ = transMgr.makeAST<AST>(SType::Stmt, @$, $1); }

    /* | error PSemi {} */
    ;

ExprStmt:
      Expr PSemi { $$ = transMgr.makeAST<AST>(SType::ExprStmt, @$, $1); }
    | Expr error {}
    ;

SelStmt:
      KwdIf PLP Expr PRP Stmt %prec KwdThen { $$ = transMgr.makeAST<AST>(SType::SelStmt, @$, $1, $3, $5); }

    | KwdIf error PRP Stmt %prec KwdThen {}
    | KwdIf PLP PRP Stmt %prec KwdThen {}
    | KwdIf PLP Expr PRP error %prec KwdThen {}
    | KwdIf PLP PRP error %prec KwdThen {}
    
    | KwdIf PLP Expr PRP Stmt KwdElse Stmt %prec KwdElse { $$ = transMgr.makeAST<AST>(SType::SelStmt, @$, $1, $3, $5, $6, $7); }

    | KwdIf error PRP Stmt KwdElse Stmt %prec KwdElse {}
    | KwdIf PLP Expr PRP Stmt KwdElse error %prec KwdElse {}
    | KwdIf PLP PRP Stmt KwdElse Stmt %prec KwdElse {}
    | KwdIf PLP PRP Stmt KwdElse error %prec KwdElse {}
    | KwdIf PLP Expr error %prec KwdElse {}
    | KwdElse Stmt {}

    | KwdSwitch PLP Expr PRP Stmt { $$ = transMgr.makeAST<AST>(SType::SelStmt, @$, $KwdSwitch, $Expr, $Stmt); }
    /* | KwdSwitch PLP expression statement {} */
    | KwdSwitch error PRP Stmt {}
    ;

LabeledStmt:
      IDWrapper OpColon Stmt { $$ = transMgr.makeAST<AST>(SType::LabeledStmt, @$, $1, $2, $3); }
    | KwdCase ConstExpr OpColon Stmt { $$ = transMgr.makeAST<AST>(SType::LabeledStmt, @$, $1, $2, $3, $4); }
    | KwdDefault OpColon Stmt { $$ = transMgr.makeAST<AST>(SType::LabeledStmt, @$, $1, $2, $3); }

    | OpColon Stmt {}
    ;

JumpStmt:
      KwdGoto IDWrapper PSemi { $$ = transMgr.makeAST<AST>(SType::JumpStmt, @$, $1, $2); }
    | KwdContinue PSemi { $$ = transMgr.makeAST<AST>(SType::JumpStmt, @$, $1); }
    | KwdBreak PSemi { $$ = transMgr.makeAST<AST>(SType::JumpStmt, @$, $1); }
    | KwdReturn Expr PSemi { $$ = transMgr.makeAST<AST>(SType::JumpStmt, @$, $1, $2); }
    | KwdReturn PSemi { $$ = transMgr.makeAST<AST>(SType::JumpStmt, @$, $1); }

    | KwdReturn Expr error {}
    | KwdReturn error {}
    ;

IterStmt:
      KwdWhile PLP Expr PRP Stmt { $$ = transMgr.makeAST<AST>(SType::IterStmt, @$, $KwdWhile, $Expr, $Stmt); }
    | KwdWhile error PRP Stmt {}
    | KwdWhile PLP Expr PRP error {}
    | KwdWhile PLP Expr error {}
    
    | KwdDo Stmt KwdWhile PLP Expr PRP PSemi { $$ = transMgr.makeAST<AST>(SType::IterStmt, @$, $KwdDo, $Stmt, $KwdWhile, $Expr); }
    | KwdDo Stmt KwdWhile PLP error PSemi {}

    | KwdFor PLP ForLoopBody PRP Stmt { $$ = transMgr.makeAST<AST>(SType::IterStmt, @$, $KwdFor, $ForLoopBody, $Stmt); }
    | KwdFor PLP ForLoopBody PRP error {}
    | KwdFor PLP ForLoopBody error {}
    ;

ForLoopBody: // TODO: add constant expressions 
      InitExpr PSemi Expr PSemi Expr { $$ = transMgr.makeAST<AST>(SType::ForLoopBody, @$, $1, $2, $3, $4, $5); }

    | PSemi Expr PSemi Expr { $$ = transMgr.makeAST<AST>(SType::ForLoopBody, @$, $1, $2, $3, $4); } 
    | InitExpr PSemi Expr PSemi { $$ = transMgr.makeAST<AST>(SType::ForLoopBody, @$, $1, $2, $3, $4); }
    | InitExpr PSemi PSemi Expr { $$ = transMgr.makeAST<AST>(SType::ForLoopBody, @$, $1, $2, $3, $4); }

    | PSemi Expr PSemi { $$ = transMgr.makeAST<AST>(SType::ForLoopBody, @$, $1, $2, $3); }
    | PSemi PSemi Expr { $$ = transMgr.makeAST<AST>(SType::ForLoopBody, @$, $1, $2, $3); }
    /* | definition PSemi {} */
    | InitExpr PSemi PSemi { $$ = transMgr.makeAST<AST>(SType::ForLoopBody, @$, $1, $2, $3); }
    
    | PSemi PSemi { $$ = transMgr.makeAST<AST>(SType::ForLoopBody, @$, $1, $2); }
    ;

ConstExpr: 
      CondExpr { $$ = transMgr.makeAST<AST>(SType::ConstExpr, @$, $1); }
    ;

Constant:
      UIntLiteral { $$ = transMgr.makeAST<AST>(SType::Constant, @$, $1); }
    | SIntLiteral { $$ = transMgr.makeAST<AST>(SType::Constant, @$, $1); }
    | FloatLiteral { $$ = transMgr.makeAST<AST>(SType::Constant, @$, $1); }
    | CharLiteral { $$ = transMgr.makeAST<AST>(SType::Constant, @$, $1); }
    /* | StrUnit {} */
    ;

PrimaryExpr:
      IDWrapper { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1); }
    | Constant { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1); }
    | StringLiteral { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1); }
    | PLP Expr PRP { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $2); }

    | PLP Expr error {}
    /* | PLP expression {} */
    ;

PostfixExpr:
      PrimaryExpr
    | PostfixExpr OpLSB Expr OpRSB { $$ = transMgr.makeAST<AST>(SType::SubscriptExpr, @$, $1, $2, $3, $4); }
    | PostfixExpr PLP ArgList PRP { $$ = transMgr.makeAST<AST>(SType::CallExpr, @$, $1, $3); }
    /* | postfix-expression PLP PRP {} */
    | PostfixExpr MemberAcessOp IDWrapper { $$ = transMgr.makeAST<AST>(SType::AccessExpr, @$, $1, $2, $3); }
    | PostfixExpr OpDPlus { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2); }
    | PostfixExpr OpDMinus { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2); }
    | PLP TypeName PRP PLC InitializerList PRC { $$ = transMgr.makeAST<AST>(SType::ExplicitCastExpr, @$, $1, $2, $3, $5); }
    | PLP TypeName PRP PLC InitializerList OpComma PRC { $$ = transMgr.makeAST<AST>(SType::ExplicitCastExpr, @$, $1, $2, $3, $5); }

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
    | OpDPlus UnaryExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2); }
    | OpDMinus UnaryExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2); }
    | OpBAnd CastExpr %prec OpUnaryPrec { $$ = transMgr.makeAST<AST>(SType::AddrOfExpr, @$, $1, $2); }
    | OpAstrk CastExpr %prec OpUnaryPrec { $$ = transMgr.makeAST<AST>(SType::DerefExpr, @$, $1, $2); }
    | UnaryArithOp CastExpr %prec OpUnaryPrec { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2); }
    | OpSizeOf UnaryExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2); }
    | OpSizeOf PLP TypeName PRP { $$ = transMgr.makeAST<AST>(SType::SizeOfExpr, @$, $1, $3); }

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
    | PLP TypeName PRP CastExpr { $$ = transMgr.makeAST<AST>(SType::ExplicitCastExpr, @$, $1, $2); }

    | PLP TypeName PRP error {}
    | PLP TypeName error {}
    ;

MulExpr:
      CastExpr
    | MulExpr MulOp CastExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }

    | MulExpr MulOp error {}
    | DivOp CastExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2); }
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
    | AddExpr AddOp MulExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }

    | AddExpr AddOp error {}
    ;

AddOp:
      OpPlus
    | OpMinus
    ;

ShiftExpr:
      AddExpr
    | ShiftExpr ShiftOp AddExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }

    | ShiftExpr ShiftOp error {}
    | ShiftOp AddExpr {}
    ;
  
ShiftOp:
      OpLShift
    | OpRShift
    ;

RelExpr:
      ShiftExpr
    | RelExpr RelOp ShiftExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }

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
    | EqualityExpr EqualityOp RelExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }

    | EqualityExpr EqualityOp error {}
    | EqualityOp RelExpr {}
    ;

EqualityOp:
      OpEQ
    | OpNE
    ;

OpBAndExpr:
      EqualityExpr
    | OpBAndExpr OpBAnd EqualityExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }

    | OpBAndExpr OpBAnd error {}
    ;

OpBXorExpr:
      OpBAndExpr
    | OpBXorExpr OpBXor OpBAndExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }

    | OpBXorExpr OpBXor error {}
    | OpBXor OpBAndExpr {}
    ;

OpBOrExpr:
      OpBXorExpr
    | OpBOrExpr OpBOr OpBXorExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }

    | OpBOrExpr OpBOr error {}
    | OpBOr OpBXorExpr {}
    ;

LogicalOpAndExpr:
      OpBOrExpr
    | LogicalOpAndExpr OpAnd OpBOrExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }

    | LogicalOpAndExpr OpAnd error {}
    | OpAnd OpBOrExpr {}
    ;

LogicalOpOrExpr:
      LogicalOpAndExpr
    | LogicalOpOrExpr OpOr LogicalOpAndExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }

    | LogicalOpOrExpr OpOr error {}
    | OpOr LogicalOpAndExpr {}
    ;

CondExpr:
      LogicalOpOrExpr
    | LogicalOpOrExpr OpQMark Expr OpColon CondExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3, $4, $5); }

    | LogicalOpOrExpr OpQMark OpColon CondExpr {}
    | LogicalOpOrExpr OpQMark Expr OpColon {}
    | OpQMark error {}
    ;

AssignExpr:
      CondExpr
    
    | CondExpr AssignOp AssignExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $2, $3); }
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
    | Expr OpComma AssignExpr { $$ = transMgr.makeAST<AST>(SType::Expr, @$, $1, $3); }

    | Expr OpComma error {}
    | OpComma AssignExpr {}
    ;
  
InitExpr:
      Expr { $$ = transMgr.makeAST<AST>(SType::InitExpr, @$, $1); }
    | DirDecl { $$ = transMgr.makeAST<AST>(SType::InitExpr, @$, $1); }
    ;

/* Argument: List of arguments */
ArgList: 
      { $$ = transMgr.makeAST<AST>(SType::ArgList, @$); }
    | ArgList OpComma AssignExpr { $1->addChild($3); $$ = $1; }
    | AssignExpr { $$ = transMgr.makeAST<AST>(SType::ArgList, @$, $1); }

    | ArgList OpComma error {}
    /* | error {} */
    ;

/* String intermediate expression. Allowing concatenation of strings. */
StringLiteral: 
      StrUnit { $$ = transMgr.makeAST<AST>(SType::StringLiteral, @$, $1); }
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