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
    class SPLCContext;
    class TranslationManager;
    class Type;

    namespace IO {
        class Driver;
        class Scanner;
        class Parser;
    } // namespace splc::IO

    } // namespace splc

    // TODO: finish all error recovery
}

%parse-param { TranslationManager  &transMgr }
%parse-param { SPLCContext         &tyCtx    }
%parse-param { Driver              &driver   }
%parse-param { Scanner             &scanner  }

%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    #include <algorithm>
    #include <ranges>
    
    // include for all driver functions
    #include "Core/splc.hh"
    #include "IO/Driver.hh"
    #include "AST/DerivedAST.hh"
    #include "Translation/TranslationManager.hh"

    using SymType = splc::ASTSymType;
    using SymbolEntry = splc::SymbolEntry;
    using Type = splc::Type;
    using CS = splc::utils::logging::ControlSeq;

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

%define api.symbol.prefix {S_} // The empty prefix is generally invalid, 
                               // but there is namespace in C++.
                               // However, mid-rule actions are named as symbols
                               // beginning with digits, and thus require a
                               // non-empty prefix.
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
%token VoidTy CharTy ShortTy IntTy SignedTy UnsignedTy LongTy FloatTy DoubleTy
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
    { transMgr.pushASTCtx(); }
    TransUnit {
        transMgr.setRootNode($TransUnit);
        SPLC_LOG_DEBUG(&@TransUnit, true) << "completed parsing";

        $TransUnit->setASTContext(transMgr.getASTCtxMgr()[0]);
        transMgr.popASTCtx();
    }
    ;

TransUnit:
      ExternDeclList { $$ = AST::make(tyCtx, SymType::TransUnit, @$, $1); }
    | { $$ = AST::make(tyCtx, SymType::TransUnit, @$); }
    ;

/* External definition list: Recursive definition */
ExternDeclList:
      ExternDecl { $$ = AST::make(tyCtx, SymType::ExternDeclList, @1, $1); }
    | ExternDeclList ExternDecl { $1->addChild($2); $$ = $1; }
    ;

/* External definition list: A single unit of one of {}. */
ExternDecl:
      PSemi { $$ = AST::make(tyCtx, SymType::ExternDecl, @$); }
    | Decl { $$ = AST::make(tyCtx, SymType::ExternDecl, @$, $1); }
    | FuncDef { $$ = AST::make(tyCtx, SymType::ExternDecl, @$, $1); }
    ;

/* Wrapper for registering types */
DeclSpecWrapper:
      DeclSpec { $$ = $1; }
    ;

DeclSpec:
      StorageSpec { $$ = AST::makeDerived<DeclSpecAST>(tyCtx, @$, $1); }
    | TypeSpec { $$ = AST::makeDerived<DeclSpecAST>(tyCtx, @$, $1); }
    | TypeQual { $$ = AST::makeDerived<DeclSpecAST>(tyCtx, @$, $1); }
    | FuncSpec { $$ = AST::makeDerived<DeclSpecAST>(tyCtx, @$, $1); }
    | DeclSpec TypeSpec { $1->addChild($2); $$ = $1; }
    | DeclSpec StorageSpec { $1->addChild($2); $$ = $1; }
    | DeclSpec TypeQual { $1->addChild($2); $$ = $1; }
    | DeclSpec FuncSpec { $1->addChild($2); $$ = $1; }
    ;

StorageSpec:
      KwdAuto { $$ = AST::make(tyCtx, SymType::StorageSpec, @$, $1); }
    | KwdExtern { $$ = AST::make(tyCtx, SymType::StorageSpec, @$, $1); }
    | KwdRegister { $$ = AST::make(tyCtx, SymType::StorageSpec, @$, $1); }
    | KwdStatic { $$ = AST::make(tyCtx, SymType::StorageSpec, @$, $1); }
    | KwdTypedef { $$ = AST::make(tyCtx, SymType::StorageSpec, @$, $1); }
    ;

SpecQualList:
      TypeSpec { $$ = AST::makeDerived<SpecQualListAST>(tyCtx, @$, $1); }
    | TypeQual { $$ = AST::makeDerived<SpecQualListAST>(tyCtx, @$, $1); }
    | SpecQualList TypeSpec { $1->addChild($2); $$ = $1; }
    | SpecQualList TypeQual { $1->addChild($2); $$ = $1; }
    ;

TypeSpec:
      BuiltinTypeSpec { $$ = AST::make(tyCtx, SymType::TypeSpec, @$, $1); }
    /* | identifier {} */
    | StructOrUnionSpec { $$ = AST::make(tyCtx, SymType::TypeSpec, @$, $1); }
    | EnumSpec { $$ = AST::make(tyCtx, SymType::TypeSpec, @$, $1); }
    | TypedefID { $$ = AST::make(tyCtx, SymType::TypeSpec, @$, $1); }
    ;

FuncSpec:
      KwdInline { $$ = AST::make(tyCtx, SymType::FuncSpec, @$, $1); }
    ;

TypeQual:
      KwdConst { $$ = AST::make(tyCtx, SymType::TypeQual, @$, $1); }
    | KwdRestrict { $$ = AST::make(tyCtx, SymType::TypeQual, @$, $1); }
    | KwdVolatile { $$ = AST::make(tyCtx, SymType::TypeQual, @$, $1); }
    ;

TypeName:
      SpecQualList { $$ = AST::make(tyCtx, SymType::TypeName, @$, $1); }
    | SpecQualList AbsDecltr { $$ = AST::make(tyCtx, SymType::TypeQual, @$, $1, $2); }
    ;

BuiltinTypeSpec:
      VoidTy
    | CharTy
    | ShortTy
    | IntTy
    | SignedTy
    | UnsignedTy
    | LongTy
    | FloatTy
    | DoubleTy
    ;

AbsDecltr:
      OpAstrk { $$ = AST::make(tyCtx, SymType::AbsDecltr, @$, $1); }
    | DirAbsDecltr { $$ = AST::make(tyCtx, SymType::AbsDecltr, @$, $1); }
    | OpAstrk AbsDecltr { $$ = AST::make(tyCtx, SymType::AbsDecltr, @$, $1, $2); }
    | OpAstrk TypeQualList AbsDecltr { $$ = AST::make(tyCtx, SymType::AbsDecltr, @$, $1, $2, $3); }
    ;

DirAbsDecltr:
      PLP AbsDecltr PRP { $$ = AST::make(tyCtx, SymType::DirAbsDecltr, @$, $1); }
    | DirAbsDecltr OpLSB AssignExpr OpRSB { $$ = AST::make(tyCtx, SymType::DirAbsDecltr, @$, $1, $2, $3, $4); }
    | DirAbsDecltr OpLSB OpRSB { $$ = AST::make(tyCtx, SymType::DirAbsDecltr, @$, $1, $2, $3); }
    | DirAbsDecltr PLP ParamList PRP { $$ = AST::make(tyCtx, SymType::DirAbsDecltr, @$, $1, $3); }
    | PLP ParamList PRP { $$ = AST::make(tyCtx, SymType::DirAbsDecltr, @$, $2); }
    
    | DirAbsDecltr OpLSB error { SPLC_LOG_ERROR(&@3, true) << "Expect ']' here"; $$ = AST::make(tyCtx, SymType::DirAbsDecltr, @$, $1); yyerrok; }
    | DirAbsDecltr OpRSB { SPLC_LOG_ERROR(&@2, true) << "Expect '[' here"; $$ = AST::make(tyCtx, SymType::DirAbsDecltr, @$, $1); yyerrok; }
    ;

/* Specify a structure */
StructOrUnionSpec:
      StructOrUnion IDWrapper {
          $$ = AST::makeDerived<StructOrUnionSpecAST>(tyCtx, @$, $1, $2);
          // TODO: register struct
      }
    | StructOrUnion StructDeclBody {
          $$ = AST::makeDerived<StructOrUnionSpecAST>(tyCtx, @$, $1, $2);
          $$->setASTContext(transMgr.getASTCtxMgr()[0]);
          transMgr.popASTCtx();

          Type *structTy = $$->computeAndSetLangType();

          transMgr.tryRegisterSymbol(
              $1->isKwdStruct() ? SymEntryType::StructDecl :
                                  SymEntryType::UnionDecl,
              "",
              structTy,
              true, &$$->getLocation());
      }
    | StructOrUnion IDWrapper StructDeclBody {
          $$ = AST::makeDerived<StructOrUnionSpecAST>(tyCtx, @$, $1, $2, $3);
          $$->setASTContext(transMgr.getASTCtxMgr()[0]);
          transMgr.popASTCtx();

          Type *structTy = $$->computeAndSetLangType();

          transMgr.tryRegisterSymbol(
              $1->isKwdStruct() ? SymEntryType::StructDecl :
                                  SymEntryType::UnionDecl,
              $2->getRootID(),
              structTy,
              true, &$$->getLocation());
      }
    ;

StructOrUnion:
      KwdStruct
    | KwdUnion
    ;

StructDeclBody:
      StructDeclBodyBegin PLC PRC { $$ = AST::make(tyCtx, SymType::StructDeclBody, @$); }
    | StructDeclBodyBegin PLC StructDeclList PRC { $$ = AST::make(tyCtx, SymType::StructDeclBody, @$, $1); }

    | StructDeclBodyBegin PLC error { SPLC_LOG_ERROR(&@1, true) << "expect token '}'"; $$ = AST::make(tyCtx, SymType::StructDeclBody, @$); yyerrok; }
    | StructDeclBodyBegin PLC StructDeclList error { SPLC_LOG_ERROR(&@3, true) << "expect token '}'"; $$ = AST::make(tyCtx, SymType::StructDeclBody, @$, $2); yyerrok; }
    ;

StructDeclBodyBegin: 
      /* Empty */ {
          transMgr.pushASTCtx();
      }
    ;


StructDeclList:
      StructDecl { $$ = AST::make(tyCtx, SymType::StructDeclBody, @$, $1); }
    | StructDeclList StructDecl { $1->addChild($2); $$ = $1; }
    ;

StructDecl:
      SpecQualList PSemi {
          $$ = AST::make(tyCtx, SymType::StructDecl, @$, $1);
          $1->computeAndSetLangType();
      }
    | SpecQualList StructDecltrList PSemi {
          $$ = AST::make(tyCtx, SymType::StructDecl, @$, $1, $2);
          $1->computeAndSetLangType();

          for (auto &child :$2->getChildren()) {
              child->computeAndSetLangType($1->getLangType());

              auto IDNode = child->getRootIDNode();

              transMgr.tryRegisterSymbol(
                  SymEntryType::Variable,
                  IDNode->getRootID(),
                  IDNode->getRootIDLangType(),
                  true, &child->getLocation());
          }
      }

    | SpecQualList error {}
    | SpecQualList StructDecltrList error {}
    ;

StructDecltrList:
      StructDecltr { $$ = AST::make(tyCtx, SymType::StructDecltrList, @$, $1); }
    | StructDecltrList OpComma StructDecltr { $1->addChild($3); $$ = $1; }

    | StructDecltrList OpComma error {}
    ;

StructDecltr:
      Decltr { $$ = AST::makeDerived<StructDecltrAST>(tyCtx, @$, $1); }
    | OpColon ConstExpr { $$ = AST::makeDerived<StructDecltrAST>(tyCtx, @$, $1, $2); }
    | Decltr OpColon ConstExpr { $$ = AST::makeDerived<StructDecltrAST>(tyCtx, @$, $1, $2, $3); }

    | OpColon error {}
    | Decltr OpColon error {}
    ;

EnumSpec:
      KwdEnum IDWrapper {
          $$ = AST::make(tyCtx, SymType::EnumSpec, @$, $1, $2);
          // TODO(future): register enum
      }
    | KwdEnum EnumBody {
          $$ = AST::make(tyCtx, SymType::EnumSpec, @$, $1, $2);
          // TODO(future): register enum
      }
    | KwdEnum IDWrapper EnumBody {
          $$ = AST::make(tyCtx, SymType::EnumSpec, @$, $1, $2, $3);
          // TODO(future): register enum
      }
    
    | KwdEnum error {}
    ;

EnumBody:
      PLC PRC { $$ = AST::make(tyCtx, SymType::EnumBody, @$); }
    | PLC EnumeratorList PRC { $$ = AST::make(tyCtx, SymType::EnumBody, @$, $2); }
    | PLC EnumeratorList OpComma PRC { $$ = AST::make(tyCtx, SymType::EnumBody, @$, $2); }

    | PLC error {}
    | PLC EnumeratorList error {}
    ;

EnumeratorList:
      Enumerator { $$ = AST::make(tyCtx, SymType::EnumeratorList, @$, $1); }
    | EnumeratorList OpComma Enumerator { $1->addChild($3); $$ = $1; }

    | OpComma Enumerator {}
    ;

Enumerator:
      EnumConst { $$ = AST::make(tyCtx, SymType::Enumerator, @$, $1); }
    | EnumConst OpAssign ConstExpr { $$ = AST::make(tyCtx, SymType::Enumerator, @$, $1, $2, $3); }

    | EnumConst OpAssign error {}
    ;

EnumConst:
      IDWrapper { $$ = AST::make(tyCtx, SymType::EnumConst, @$, $1); }
    ;

/* Single variable declaration */
Decltr:
      DirDecltr { $$ = AST::makeDerived<DecltrAST>(tyCtx, @$, $1); }
    | OpAstrk Decltr { $$ = AST::makeDerived<DecltrAST>(tyCtx, @$, $1, $2); }
    | OpAstrk TypeQualList Decltr { $$ = AST::makeDerived<DecltrAST>(tyCtx, @$, $1, $2, $3); }
    ;

DirDecltr:
      IDWrapper {
          $$ = AST::makeDerived<DirDecltrAST>(tyCtx, @$, $1);
      }
    | WrappedDirDecltr {
          $$ = AST::makeDerived<DirDecltrAST>(tyCtx, @$, $1);
      }
    | DirDecltr OpLSB AssignExpr OpRSB {
          $$ = AST::makeDerived<DirDecltrAST>(tyCtx, @$, $1, $2, $3, $4);
      }
    | DirDecltr OpLSB OpRSB {
          $$ = AST::makeDerived<DirDecltrAST>(tyCtx, @$, $1, $2, $3);
      }
    | WrappedDirDecltr PLP ParamList PRP {
          $$ = AST::makeDerived<DirDecltrAST>(tyCtx, @$, $1, $3); 
      }
    | DirDecltr OpLSB AssignExpr error {}
    /* | direct-declarator error {}  */
    | DirDecltr OpRSB {}
    ;

WrappedDirDecltr:
      PLP Decltr PRP { $$ = AST::makeDerived<WrappedDirDecltrAST>(tyCtx, @$, $2); }
    ;

/* PtrDecltr:
      OpAstrk { $$ = AST::make(tyCtx, SymType::PtrDecltr, @$, $1); }
    | OpAstrk TypeQualList { $$ = AST::make(tyCtx, SymType::PtrDecltr, @$, $1, $2); }
    | OpAstrk PtrDecltr { $$ = AST::make(tyCtx, SymType::PtrDecltr, @$, $1, $2); }
    | OpAstrk TypeQualList PtrDecltr { $$ = AST::make(tyCtx, SymType::PtrDecltr, @$, $1, $2, $3); }
    ; */

TypeQualList:
      TypeQual { $$ = AST::make(tyCtx, SymType::TypeQualList, @$, $1); }
    | TypeQualList TypeQual { $1->addChild($2); $$ = $1; }
    ;

/* Definition: List of definitions. Recursive definition. */
/* declaration-list:
      declaration {}
    | declaration-list declaration {}
    ; */

/* Definition: Base */
Decl:
      DirDecl PSemi { $$ = AST::make(tyCtx, SymType::Decl, @$, $1); }
    | DirDecl error {}
    ;

DirDecl:
      DeclSpecWrapper {
          $$ = AST::make(tyCtx, SymType::DirDecl, @$, $1);
          // $1->computeAndSetLangType();
      }
    | DeclSpecWrapper InitDecltrList {
          $$ = AST::make(tyCtx, SymType::DirDecl, @$, $1, $2);
          $1->computeAndSetLangType();

          for (auto &child : $2->getChildren()) {
              // dispatch: check whether it is function or declaration
              if (auto decltrNode = child->findFirstChild(SymType::Decltr); decltrNode != nullptr) {
                  child->computeAndSetLangType($1->getLangType());

                  auto IDNode = child->getRootIDNode();

                  transMgr.tryRegisterSymbol(
                      $1->isTypedef() ? SymEntryType::Typedef :
                                        SymEntryType::Variable,
                      IDNode->getRootID(),
                      IDNode->getRootIDLangType(),
                      true, &child->getLocation());
              }
              else {
                  // push all parameters
                  decltrNode = child->findFirstChild(SymType::FuncDecltr);
                  auto paramTypeNode = decltrNode->findFirstChildBFS(SymType::ParamTypeList);

                  for (auto &innerChild : paramTypeNode->getChildren()[0]->getChildren()) {
                      auto IDNode = innerChild->getRootIDNode();
                      transMgr.tryRegisterSymbol(
                          SymEntryType::Paramater, IDNode->getRootID(),
                          IDNode->getRootIDLangType(),
                          true, &innerChild->getLocation());
                  }

                  // register function
                  decltrNode->computeAndSetLangType($1->computeAndSetLangType());
                  auto node = decltrNode->getRootIDNode();

                  transMgr.tryRegisterSymbol(
                      SymEntryType::Function, node->getRootID(),
                      node->getRootIDLangType(),
                      false, &decltrNode->getLocation());
              }
          }
      }
    ;

/* Definition: Declaration of multiple variable.  */ 
InitDecltrList:
      InitDecltr { $$ = AST::make(tyCtx, SymType::InitDecltrList, @$, $1); }
    | InitDecltrList OpComma InitDecltr { $1->addChild($3); $$ = $1; }

    | InitDecltrList OpComma {}
    | OpComma InitDecltr {}
    | OpComma {}
    ;

/* Definition: Single declaration unit. */
InitDecltr:
      Decltr { $$ = AST::makeDerived<InitDecltrAST>(tyCtx, @$, $1); }
    | FuncDecltr { $$ = AST::makeDerived<InitDecltrAST>(tyCtx, @$, $1); transMgr.popASTCtx(); }
    | Decltr OpAssign Initializer { $$ = AST::makeDerived<InitDecltrAST>(tyCtx, @$, $1, $2, $3); }
    | Decltr OpAssign error {}
    ;

Initializer:
      AssignExpr { $$ = AST::make(tyCtx, SymType::Initializer, @$, $1); }
    | PLC InitializerList PRC { $$ = AST::make(tyCtx, SymType::Initializer, @$, $2); }
    | PLC InitializerList OpComma PRC { $$ = AST::make(tyCtx, SymType::Initializer, @$, $2); }

    | PLC InitializerList error {}
    ;

InitializerList:
      Initializer { $$ = AST::make(tyCtx, SymType::InitializerList, @$, $1); }
    | Designation Initializer { $$ = AST::make(tyCtx, SymType::InitializerList, @$, $1, $2); }
    | InitializerList OpComma Designation Initializer { $1->addChildren($3, $4) ; $$ = $1; }
    | InitializerList OpComma Initializer { $1->addChild($3) ; $$ = $1; }

    | Designation error {}
    | InitializerList OpComma error {}
    ;

Designation:
      DesignatorList OpAssign { $$ = AST::make(tyCtx, SymType::Designation, @$, $1, $2); }
    ;

DesignatorList:
      Designator { $$ = AST::make(tyCtx, SymType::DesignatorList, @$, $1); }
    | DesignatorList Designator { $1->addChild($2) ; $$ = $1; }
    ;

Designator:
      OpLSB ConstExpr OpRSB { $$ = AST::make(tyCtx, SymType::Designator, @$, $1, $2, $3); }
    | OpDot IDWrapper { $$ = AST::make(tyCtx, SymType::Designator, @$, $1, $2); }

    | OpLSB ConstExpr error {}
    | OpDot error {}
    ;

FuncDef:
      FuncProto CompStmt {
          transMgr.popASTCtx();

          $$ = AST::make(tyCtx, SymType::FuncDef, @$, $1, $2);
          // TODO: register function body
          auto decltrNode = $1->getChildren()[1];
          Type *ty = decltrNode->getRootIDLangType();
          auto node = decltrNode->getRootIDNode();

          SymbolEntry ent = transMgr.getSymbol(SymEntryType::Function, node->getRootID());
          
          if (ent.type == ty) {
              transMgr.tryRegisterSymbol(
                  SymEntryType::Function, node->getRootID(),
                  ty,
                  true, &ent.location, $$);
          }
          else {
              SPLC_LOG_ERROR(&decltrNode->getLocation(), true) << "function does not match the previous definition: given: " << *ty;
              SPLC_LOG_NOTE(&ent.location, true) << "previously declared here: given: " << *ent.type;
          }
      }
    /* | FuncDecltr CompStmt {
          SPLC_LOG_WARN(&@1, true) << "function is missing a specifier and will default to 'int'";
          auto declSpec = ASTHelper::makeDeclSpecifierTree(Location{@$.begin}, SymType::IntTy);
          $$ = AST::make(tyCtx, SymType::FuncDef, @$, declSpec, $1, $2);
      }  */
    ;

FuncProto:
      /* FuncDecltr PSemi {
          SPLC_LOG_WARN(&@1, true) << "function is missing a specifier and will default to 'int'";
          auto declSpec = ASTHelper::makeDeclSpecifierTree(Location{@$.begin}, SymType::IntTy);
          $$ = AST::make(tyCtx, SymType::FuncProto, @$, declSpec, $1);
      }  */
      DeclSpecWrapper FuncDecltr {
          auto ctx = transMgr.getASTCtxMgr()[0]; // Pop the context temporarily to push function definition.
          transMgr.popASTCtx();

          $$ = AST::make(tyCtx, SymType::FuncProto, @$, $1, $2);

          // push all parameters
          auto paramTypeNode = $2->findFirstChildBFS(SymType::ParamTypeList);

          for (auto &child : paramTypeNode->getChildren()[0]->getChildren()) {
              auto IDNode = child->getRootIDNode();
              transMgr.tryRegisterSymbol(
                  SymEntryType::Paramater, IDNode->getRootID(),
                  IDNode->getRootIDLangType(),
                  true, &child->getLocation());
          }

          // register function
          $2->computeAndSetLangType($1->computeAndSetLangType());
          auto node = $2->getRootIDNode();

          transMgr.tryRegisterSymbol(
              SymEntryType::Function, node->getRootID(),
              node->getRootIDLangType(),
              false, &@2);

          transMgr.pushASTCtx(ctx);
          $$->setASTContext(ctx);
      }
    ;


/* Function: Function name and body. */
FuncDecltr:
      DirFuncDecltr { $$ = AST::makeDerived<FuncDecltrAST>(tyCtx, @$, $1); }
    | OpAstrk FuncDecltr {
          $$ = AST::makeDerived<FuncDecltrAST>(tyCtx, @$, $1, $2);
      }
    | OpAstrk TypeQualList FuncDecltr {
          $$ = AST::makeDerived<FuncDecltrAST>(tyCtx, @$, $1, $2, $3);
      }
    ;

DirFuncDecltr:
      DirDecltrForFunc DirFuncDecltrBegin PLP ParamTypeList PRP { 
          $$ = AST::makeDerived<DirFuncDecltrAST>(tyCtx, @$, $DirDecltrForFunc, $ParamTypeList); 
      }
    /* | direct-declarator-for-function PLP PRP {} */

    /* | direct-declarator-for-function PLP error {} */
    | DirDecltrForFunc DirFuncDecltrBegin PLP ParamTypeList error {}
    /* | direct-declarator-for-function PLP error {} */

    | PLP ParamTypeList PRP {}
    /* | PLP PRP {} */
    ;

DirFuncDecltrBegin:
      /* Empty */ {
          transMgr.pushASTCtx();
      }
    ;

DirDecltrForFunc:
      IDWrapper {
          $$ = $1;
      }
    ;

/* List of variables names */
ParamTypeList:
      ParamList { $$ = AST::make(tyCtx, SymType::ParamTypeList, @$, $1); }
    | ParamList OpComma OpEllipsis { $$ = AST::make(tyCtx, SymType::ParamTypeList, @$, $1, $3); }
    ;

ParamList:
      { $$ = AST::makeDerived<ParamListAST>(tyCtx, @$); }
    | ParamDecltr { $$ = AST::makeDerived<ParamListAST>(tyCtx, @$, $1); }
    | ParamList OpComma ParamDecltr { $1->addChild($3); $$ = $1; }

    | ParamList OpComma error {}
    ;

/* Parameter declaration */ 
ParamDecltr:
      DeclSpecWrapper Decltr {
          $$ = AST::makeDerived<ParamDecltrAST>(tyCtx, @$, $1, $2);
          $$->computeAndSetLangType();
      }
    | DeclSpecWrapper AbsDecltr {
          $$ = AST::makeDerived<ParamDecltrAST>(tyCtx, @$, $1, $2);
          $$->computeAndSetLangType();
      }
    | DeclSpecWrapper {
          $$ = AST::makeDerived<ParamDecltrAST>(tyCtx, @$, $1);
          $$->computeAndSetLangType();
      }

    /* | error {} */
    ;

/* Compound statement: A new scope. */
CompStmt:
      /* PLC general-statement-list PRC */
      PLC ComptStmtBegin GeneralStmtList PRC {
          $$ = AST::make(tyCtx, SymType::CompStmt, @$, $GeneralStmtList);
          auto ctx = transMgr.getASTCtxMgr()[0];
          transMgr.popASTCtx();
          $$->setASTContext(ctx);
      }
    | PLC ComptStmtBegin PRC { $$ = AST::make(tyCtx, SymType::CompStmt, @$); }

    | PLC ComptStmtBegin GeneralStmtList error {}
    | PLC ComptStmtBegin error {}
    ;

ComptStmtBegin:
      /* Empty */ {
          transMgr.pushASTCtx();
      }
    ;

/* wrapper for C99 standard for statements */
GeneralStmtList:
      Stmt { $$ = AST::make(tyCtx, SymType::GeneralStmtList, @$, $1); }
    | Decl { $$ = AST::make(tyCtx, SymType::GeneralStmtList, @$, $1); }
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
      PSemi { $$ = AST::make(tyCtx, SymType::Stmt, @$); }
    | CompStmt { $$ = AST::make(tyCtx, SymType::Stmt, @$, $1); }
    | ExprStmt { $$ = AST::make(tyCtx, SymType::Stmt, @$, $1); }
    | SelStmt { $$ = AST::make(tyCtx, SymType::Stmt, @$, $1); }
    | IterStmt { $$ = AST::make(tyCtx, SymType::Stmt, @$, $1); }
    | LabeledStmt { $$ = AST::make(tyCtx, SymType::Stmt, @$, $1); }
    | JumpStmt { $$ = AST::make(tyCtx, SymType::Stmt, @$, $1); }

    /* | error PSemi {} */
    ;

ExprStmt:
      Expr PSemi { $$ = AST::make(tyCtx, SymType::ExprStmt, @$, $1); }
    | Expr error {}
    ;

SelStmt:
      KwdIf PLP Expr PRP Stmt %prec KwdThen { $$ = AST::make(tyCtx, SymType::SelStmt, @$, $1, $3, $5); }

    | KwdIf error PRP Stmt %prec KwdThen {}
    | KwdIf PLP PRP Stmt %prec KwdThen {}
    | KwdIf PLP Expr PRP error %prec KwdThen {}
    | KwdIf PLP PRP error %prec KwdThen {}
    
    | KwdIf PLP Expr PRP Stmt KwdElse Stmt %prec KwdElse { $$ = AST::make(tyCtx, SymType::SelStmt, @$, $1, $3, $5, $6, $7); }

    | KwdIf error PRP Stmt KwdElse Stmt %prec KwdElse {}
    | KwdIf PLP Expr PRP Stmt KwdElse error %prec KwdElse {}
    | KwdIf PLP PRP Stmt KwdElse Stmt %prec KwdElse {}
    | KwdIf PLP PRP Stmt KwdElse error %prec KwdElse {}
    | KwdIf PLP Expr error %prec KwdElse {}
    | KwdElse Stmt {}

    | KwdSwitch PLP Expr PRP Stmt { $$ = AST::make(tyCtx, SymType::SelStmt, @$, $KwdSwitch, $Expr, $Stmt); }
    /* | KwdSwitch PLP expression statement {} */
    | KwdSwitch error PRP Stmt {}
    ;

LabeledStmt:
      IDWrapper OpColon Stmt { $$ = AST::make(tyCtx, SymType::LabeledStmt, @$, $1, $2, $3); }
    | KwdCase ConstExpr OpColon Stmt { $$ = AST::make(tyCtx, SymType::LabeledStmt, @$, $1, $2, $3, $4); }
    | KwdDefault OpColon Stmt { $$ = AST::make(tyCtx, SymType::LabeledStmt, @$, $1, $2, $3); }

    | OpColon Stmt {}
    ;

JumpStmt:
      KwdGoto IDWrapper PSemi { $$ = AST::make(tyCtx, SymType::JumpStmt, @$, $1, $2); }
    | KwdContinue PSemi { $$ = AST::make(tyCtx, SymType::JumpStmt, @$, $1); }
    | KwdBreak PSemi { $$ = AST::make(tyCtx, SymType::JumpStmt, @$, $1); }
    | KwdReturn Expr PSemi { $$ = AST::make(tyCtx, SymType::JumpStmt, @$, $1, $2); }
    | KwdReturn PSemi { $$ = AST::make(tyCtx, SymType::JumpStmt, @$, $1); }

    | KwdReturn Expr error {}
    | KwdReturn error {}
    ;

IterStmt:
      KwdWhile PLP Expr PRP Stmt { $$ = AST::make(tyCtx, SymType::IterStmt, @$, $KwdWhile, $Expr, $Stmt); }
    | KwdWhile error PRP Stmt {}
    | KwdWhile PLP Expr PRP error {}
    | KwdWhile PLP Expr error {}
    
    | KwdDo Stmt KwdWhile PLP Expr PRP PSemi { $$ = AST::make(tyCtx, SymType::IterStmt, @$, $KwdDo, $Stmt, $KwdWhile, $Expr); }
    | KwdDo Stmt KwdWhile PLP error PSemi {}

    | KwdFor ForLoopCtxBegin PLP ForLoopBody PRP Stmt {
          $$ = AST::make(tyCtx, SymType::IterStmt, @$, $KwdFor, $ForLoopBody, $Stmt);
      }
    | KwdFor ForLoopCtxBegin PLP ForLoopBody PRP error {}
    | KwdFor ForLoopCtxBegin PLP ForLoopBody error {}
    ;

ForLoopCtxBegin:
      /* Empty */{
          transMgr.pushASTCtx();
      }
    ;

ForLoopBody: // TODO: add constant expressions 
      InitExpr PSemi Expr PSemi Expr { $$ = AST::make(tyCtx, SymType::ForLoopBody, @$, $1, $2, $3, $4, $5); }

    | PSemi Expr PSemi Expr { $$ = AST::make(tyCtx, SymType::ForLoopBody, @$, $1, $2, $3, $4); }
    | InitExpr PSemi Expr PSemi { $$ = AST::make(tyCtx, SymType::ForLoopBody, @$, $1, $2, $3, $4); }
    | InitExpr PSemi PSemi Expr { $$ = AST::make(tyCtx, SymType::ForLoopBody, @$, $1, $2, $3, $4); }

    | PSemi Expr PSemi { $$ = AST::make(tyCtx, SymType::ForLoopBody, @$, $1, $2, $3); }
    | PSemi PSemi Expr { $$ = AST::make(tyCtx, SymType::ForLoopBody, @$, $1, $2, $3); }
    /* | definition PSemi {} */
    | InitExpr PSemi PSemi { $$ = AST::make(tyCtx, SymType::ForLoopBody, @$, $1, $2, $3); }
    
    | PSemi PSemi { $$ = AST::make(tyCtx, SymType::ForLoopBody, @$, $1, $2); }
    ;

ConstExpr:
      CondExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1); }
    ;

Constant:
      UIntLiteral { $$ = AST::make(tyCtx, SymType::Constant, @$, $1); }
    | SIntLiteral { $$ = AST::make(tyCtx, SymType::Constant, @$, $1); }
    | FloatLiteral { $$ = AST::make(tyCtx, SymType::Constant, @$, $1); }
    | CharLiteral { $$ = AST::make(tyCtx, SymType::Constant, @$, $1); }
    /* | StrUnit {} */
    ;

PrimaryExpr:
      IDWrapper { $$ = AST::make(tyCtx, SymType::Expr, @$, $1); }
    | Constant { $$ = AST::make(tyCtx, SymType::Expr, @$, $1); }
    | StringLiteral { $$ = AST::make(tyCtx, SymType::Expr, @$, $1); }
    | PLP Expr PRP { $$ = AST::make(tyCtx, SymType::Expr, @$, $2); }

    | PLP Expr error {}
    /* | PLP expression {} */
    ;

PostfixExpr:
      PrimaryExpr
    | PostfixExpr OpLSB Expr OpRSB { $$ = AST::make(tyCtx, SymType::SubscriptExpr, @$, $1, $2, $3, $4); }
    | PostfixExpr PLP ArgList PRP { $$ = AST::make(tyCtx, SymType::CallExpr, @$, $1, $3); }
    /* | postfix-expression PLP PRP {} */
    | PostfixExpr MemberAccessOp IDWrapper { $$ = AST::make(tyCtx, SymType::AccessExpr, @$, $1, $2, $3); }
    | PostfixExpr OpDPlus { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2); }
    | PostfixExpr OpDMinus { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2); }
    | PLP TypeName PRP PLC InitializerList PRC { $$ = AST::make(tyCtx, SymType::ExplicitCastExpr, @$, $1, $2, $3, $5); }
    | PLP TypeName PRP PLC InitializerList OpComma PRC { $$ = AST::make(tyCtx, SymType::ExplicitCastExpr, @$, $1, $2, $3, $5); }

    | PostfixExpr OpLSB Expr error {}
    | PostfixExpr PLP ArgList error {}
    | PostfixExpr MemberAccessOp {}
    | OpRArrow IDWrapper {}
    | PLP TypeName PRP PLC InitializerList error {}
    ;

MemberAccessOp:
      OpDot
    | OpRArrow
    ;

UnaryExpr:
      PostfixExpr
    | OpDPlus UnaryExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2); }
    | OpDMinus UnaryExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2); }
    | OpBAnd CastExpr %prec OpUnaryPrec { $$ = AST::make(tyCtx, SymType::AddrOfExpr, @$, $1, $2); }
    | OpAstrk CastExpr %prec OpUnaryPrec { $$ = AST::make(tyCtx, SymType::DerefExpr, @$, $1, $2); }
    | UnaryArithOp CastExpr %prec OpUnaryPrec { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2); }
    | OpSizeOf UnaryExpr { $$ = AST::make(tyCtx, SymType::SizeOfExpr, @$, $1, $2); }
    | OpSizeOf PLP TypeName PRP { $$ = AST::make(tyCtx, SymType::SizeOfExpr, @$, $1, $3); }

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
    | PLP TypeName PRP CastExpr { $$ = AST::make(tyCtx, SymType::ExplicitCastExpr, @$, $1, $2); }

    | PLP TypeName PRP error {}
    | PLP TypeName error {}
    ;

MulExpr:
      CastExpr
    | MulExpr MulOp CastExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }

    | MulExpr MulOp error {}
    | DivOp CastExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2); }
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
    | AddExpr AddOp MulExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }

    | AddExpr AddOp error {}
    ;

AddOp:
      OpPlus
    | OpMinus
    ;

ShiftExpr:
      AddExpr
    | ShiftExpr ShiftOp AddExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }

    | ShiftExpr ShiftOp error {}
    | ShiftOp AddExpr {}
    ;
  
ShiftOp:
      OpLShift
    | OpRShift
    ;

RelExpr:
      ShiftExpr
    | RelExpr RelOp ShiftExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }

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
    | EqualityExpr EqualityOp RelExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }

    | EqualityExpr EqualityOp error {}
    | EqualityOp RelExpr {}
    ;

EqualityOp:
      OpEQ
    | OpNE
    ;

OpBAndExpr:
      EqualityExpr
    | OpBAndExpr OpBAnd EqualityExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }

    | OpBAndExpr OpBAnd error {}
    ;

OpBXorExpr:
      OpBAndExpr
    | OpBXorExpr OpBXor OpBAndExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }

    | OpBXorExpr OpBXor error {}
    | OpBXor OpBAndExpr {}
    ;

OpBOrExpr:
      OpBXorExpr
    | OpBOrExpr OpBOr OpBXorExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }

    | OpBOrExpr OpBOr error {}
    | OpBOr OpBXorExpr {}
    ;

LogicalOpAndExpr:
      OpBOrExpr
    | LogicalOpAndExpr OpAnd OpBOrExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }

    | LogicalOpAndExpr OpAnd error {}
    | OpAnd OpBOrExpr {}
    ;

LogicalOpOrExpr:
      LogicalOpAndExpr
    | LogicalOpOrExpr OpOr LogicalOpAndExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }

    | LogicalOpOrExpr OpOr error {}
    | OpOr LogicalOpAndExpr {}
    ;

CondExpr:
      LogicalOpOrExpr
    | LogicalOpOrExpr OpQMark Expr OpColon CondExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3, $4, $5); }

    | LogicalOpOrExpr OpQMark OpColon CondExpr {}
    | LogicalOpOrExpr OpQMark Expr OpColon {}
    | OpQMark error {}
    ;

AssignExpr:
      CondExpr  {
          // Always wrap the output with Expr
          if ($1->isExpr()) {
              $$ = $1;
          }
          else {
              $$ = AST::make(tyCtx, SymType::Expr, @$, $1); 
          }
      }
    
    | CondExpr AssignOp AssignExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $2, $3); }
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
      AssignExpr // Already wrapped at AssignExpr
    | Expr OpComma AssignExpr { $$ = AST::make(tyCtx, SymType::Expr, @$, $1, $3); }

    | Expr OpComma error {}
    | OpComma AssignExpr {}
    ;

InitExpr:
      Expr { $$ = AST::make(tyCtx, SymType::InitExpr, @$, $1); }
    | DirDecl { $$ = AST::make(tyCtx, SymType::InitExpr, @$, $1); }
    ;

/* Argument: List of arguments */
ArgList:
      { $$ = AST::make(tyCtx, SymType::ArgList, @$); }
    | ArgList OpComma AssignExpr { $1->addChild($3); $$ = $1; }
    | AssignExpr { $$ = AST::make(tyCtx, SymType::ArgList, @$, $1); }

    | ArgList OpComma error {}
    /* | error {} */
    ;

/* String intermediate expression. Allowing concatenation of strings. */
StringLiteral:
      StrUnit { $$ = AST::make(tyCtx, SymType::StringLiteral, @$, $1); }
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