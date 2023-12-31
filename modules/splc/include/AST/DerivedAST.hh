#ifndef __SPLC_AST_AST_HH__
#define __SPLC_AST_AST_HH__ 1

#include "AST/ASTBase.hh"
#include "Core/splc.hh"

namespace splc {
//// YYNTOKENS-AST declaration slot

//// YYEMPTY-AST declaration slot

//// YYEOF-AST declaration slot

//// YYerror-AST declaration slot

//// YYUNDEF-AST declaration slot

//===----------------------------------------------------------------------===//
//                              Storage Qualifiers
//===----------------------------------------------------------------------===//
/// KwdAuto-AST declaration slot

/// KwdExtern-AST declaration slot

/// KwdRegister-AST declaration slot

/// KwdStatic-AST declaration slot

/// KwdTypedef-AST declaration slot

//===----------------------------------------------------------------------===//
//                              Type Qualifiers
//===----------------------------------------------------------------------===//
/// KwdConst-AST declaration slot

/// KwdRestrict-AST declaration slot

/// KwdVolatile-AST declaration slot

/// KwdInline-AST declaration slot

//===----------------------------------------------------------------------===//
//                              Type Specifiers
//===----------------------------------------------------------------------===//
// VoidTy
// CharTy
// ShortTy
// IntTy
// SignedTy
// UnsignedTy
// LongTy
// FloatTy
// DoubleTy

//===----------------------------------------------------------------------===//
//                           Aggregate Type Keywords
//===----------------------------------------------------------------------===//
/// KwdEnum-AST declaration slot

/// KwdStruct-AST declaration slot

/// KwdUnion-AST declaration slot

//===----------------------------------------------------------------------===//
//                            Control-Flow Keywords
//===----------------------------------------------------------------------===//
/// KwdIf-AST declaration slot

/// KwdElse-AST declaration slot

/// KwdSwitch-AST declaration slot

/// KwdWhile-AST declaration slot

/// KwdFor-AST declaration slot

/// KwdDo-AST declaration slot

/// KwdDefault-AST declaration slot

/// KwdCase-AST declaration slot

/// KwdGoto-AST declaration slot

/// KwdContinue-AST declaration slot

/// KwdBreak-AST declaration slot

/// KwdReturn-AST declaration slot

//===----------------------------------------------------------------------===//
//                                     IDs
//===----------------------------------------------------------------------===//
/// ID-AST declaration slot

/// TypedefID-AST declaration slot

//===----------------------------------------------------------------------===//
//                                   Operators
//===----------------------------------------------------------------------===//
/// OpAssign-AST declaration slot

/// OpMulAssign-AST declaration slot

/// OpDivAssign-AST declaration slot

/// OpModAssign-AST declaration slot

/// OpPlusAssign-AST declaration slot

/// OpMinusAssign-AST declaration slot

/// OpLShiftAssign-AST declaration slot

/// OpRShiftAssign-AST declaration slot

/// OpBAndAssign-AST declaration slot

/// OpBXorAssign-AST declaration slot

/// OpBOrAssign-AST declaration slot

/// OpAnd-AST declaration slot

/// OpOr-AST declaration slot

/// OpNot-AST declaration slot

/// OpLT-AST declaration slot

/// OpLE-AST declaration slot

/// OpGT-AST declaration slot

/// OpGE-AST declaration slot

/// OpNE-AST declaration slot

/// OpEQ-AST declaration slot

/// OpQMark-AST declaration slot

/// OpColon-AST declaration slot

/// OpLShift-AST declaration slot

/// OpRShift-AST declaration slot

/// OpBAnd-AST declaration slot

/// OpBOr-AST declaration slot

/// OpBNot-AST declaration slot

/// OpBXor-AST declaration slot

/// OpDPlus-AST declaration slot

/// OpDMinus-AST declaration slot

/// OpPlus-AST declaration slot

/// OpMinus-AST declaration slot

/// OpAstrk-AST declaration slot

/// OpDiv-AST declaration slot

/// OpMod-AST declaration slot

/// OpDot-AST declaration slot

/// OpRArrow-AST declaration slot

/// OpSizeOf-AST declaration slot

/// OpLSB-AST declaration slot

/// OpRSB-AST declaration slot

/// OpComma-AST declaration slot

/// OpEllipsis-AST declaration slot

//===----------------------------------------------------------------------===//
//                                  Punctuators
//===----------------------------------------------------------------------===//
/// PSemi-AST declaration slot

/// PLC-AST declaration slot

/// PRC-AST declaration slot

/// PLP-AST declaration slot

/// PRP-AST declaration slot

//===----------------------------------------------------------------------===//
//                                   Literals
//===----------------------------------------------------------------------===//
/// UIntLiteral-AST declaration slot

/// SIntLiteral-AST declaration slot

/// FloatLiteral-AST declaration slot

/// CharLiteral-AST declaration slot

/// StrUnit-AST declaration slot

//===----------------------------------------------------------------------===//
//                                 Expressions
//===----------------------------------------------------------------------===//
/// SubscriptExpr-AST declaration slot

/// CallExpr-AST declaration slot

/// AccessExpr-AST declaration slot

/// ExplicitCastExpr-AST declaration slot

/// AddrOfExpr-AST declaration slot

/// DerefExpr-AST declaration slot

/// SizeOfExpr-AST declaration slot

//===----------------------------------------------------------------------===//
//                                 Productions
//===----------------------------------------------------------------------===//
/// ParseRoot-AST declaration slot

/// TransUnit-AST declaration slot

/// ExternDeclList-AST declaration slot

/// ExternDecl-AST declaration slot

/// DeclSpec-AST declaration slot
class DeclSpecAST : public AST {
    Type *builtInComputeLangType() noexcept;

    bool isTypedef_ = false;

  public:
    DeclSpecAST(const Location &loc_) noexcept : AST{ASTSymType::DeclSpec, loc_}
    {
    }

    DeclSpecAST(Ptr<SPLCContext> typeContext_, const Location &loc_) noexcept
        : AST{typeContext_, ASTSymType::DeclSpec, loc_}
    {
    }

    /// Compute the base type of this declaration specifier, and return the
    /// computed type entry.
    virtual void computeLangType() noexcept override
    {
        setLangType(builtInComputeLangType());
    }

    virtual bool isTypedef() const noexcept override { return isTypedef_; }

    virtual void setTypedef(bool isTyDef) noexcept override
    {
        isTypedef_ = isTyDef;
    }
};

/// StorageSpec-AST declaration slot

/// SpecQualList-AST declaration slot

/// TypeSpec-AST declaration slot

/// FuncSpec-AST declaration slot

/// TypeQual-AST declaration slot

/// TypeName-AST declaration slot

/// BuiltinTypeSpec-AST declaration slot

/// AbsDecltr-AST declaration slot

/// DirAbsDecltr-AST declaration slot

/// StructOrUnionSpec-AST declaration slot

/// StructOrUnion-AST declaration slot

/// StructDeclBody-AST declaration slot

/// StructDeclList-AST declaration slot

/// StructDecl-AST declaration slot

/// StructDecltrList-AST declaration slot

/// StructDecltr-AST declaration slot

/// EnumSpec-AST declaration slot

/// EnumBody-AST declaration slot

/// EnumeratorList-AST declaration slot

/// Enumerator-AST declaration slot

/// EnumConst-AST declaration slot

/// Decltr-AST declaration slot

class DecltrAST : public AST {
    Type *builtInComputeLangType() noexcept;

  public:
    DecltrAST(const Location &loc_) noexcept : AST{ASTSymType::Decltr, loc_} {}

    DecltrAST(Ptr<SPLCContext> typeContext_, const Location &loc_) noexcept
        : AST{typeContext_, ASTSymType::Decltr, loc_}
    {
    }

    virtual void computeLangType() noexcept override
    {
        setLangType(builtInComputeLangType());
    }
};

/// DirDecltr-AST declaration slot

class DirDecltrAST : public AST {
    Type *builtInComputeLangType() noexcept;

    Type *baseType{nullptr};

  public:
    DirDecltrAST(const Location &loc_) noexcept
        : AST{ASTSymType::DirDecltr, loc_}
    {
    }

    DirDecltrAST(Ptr<SPLCContext> typeContext_, const Location &loc_) noexcept
        : AST{typeContext_, ASTSymType::DirDecltr, loc_}
    {
    }

    template <IsPtrAST... Children>
    static Ptr<DirDecltrAST> make(Ptr<SPLCContext> tyContext_,
                                  const Location &loc,
                                  Children... children) noexcept
    {
        auto ptr = AST::makeDerived<DirDecltrAST>(
            tyContext_, loc, std::forward<Children>(children)...);
        ptr->computeLangType();
        return ptr;
    }

    template <IsPtrAST... Children>
    static Ptr<DirDecltrAST> make(Ptr<SPLCContext> tyContext_, Type *baseTy,
                                  const Location &loc,
                                  Children... children) noexcept
    {
        auto ptr = make(tyContext_, loc, std::forward<Children>(children)...);
        ptr->baseType = baseTy;
        ptr->computeLangType();
        return ptr;
    }

    virtual void computeLangType() noexcept override
    {
        setLangType(builtInComputeLangType());
    }
};

/// WrappedDirDecltr-AST declaration slot

class WrappedDirDecltrAST : public AST {
    Type *builtInComputeLangType() noexcept;

  public:
    WrappedDirDecltrAST(const Location &loc_) noexcept
        : AST{ASTSymType::WrappedDirDecltr, loc_}
    {
    }

    WrappedDirDecltrAST(Ptr<SPLCContext> typeContext_,
                        const Location &loc_) noexcept
        : AST{typeContext_, ASTSymType::WrappedDirDecltr, loc_}
    {
    }

    virtual void computeLangType() noexcept override
    {
        setLangType(builtInComputeLangType());
    }
};

/// TypeQualList-AST declaration slot

/// Decl-AST declaration slot

/// DirDecl-AST declaration slot

/// InitDecltrList-AST declaration slot

/// InitDecltr-AST declaration slot

/// Initializer-AST declaration slot

/// InitializerList-AST declaration slot

/// Designation-AST declaration slot

/// DesignatorList-AST declaration slot

/// Designator-AST declaration slot

/// FuncDef-AST declaration slot

/// FuncDecl-AST declaration slot

/// FuncDecltr-AST declaration slot

/// DirFuncDecltr-AST declaration slot

/// DirDecltrForFunc-AST declaration slot

/// ParamTypeList-AST declaration slot

/// ParamList-AST declaration slot

class ParamListAST : public AST {
    void builtInComputeLangType() noexcept;

  public:
    ParamListAST(const Location &loc_) noexcept
        : AST{ASTSymType::ParamList, loc_}
    {
    }

    ParamListAST(Ptr<SPLCContext> typeContext_, const Location &loc_) noexcept
        : AST{typeContext_, ASTSymType::ParamList, loc_}
    {
    }

    virtual void addChild(PtrAST child) noexcept override;
};

/// ParamDecltr-AST declaration slot

class ParamDecltrAST : public AST {
    Type *builtInComputeLangType() noexcept;

  public:
    ParamDecltrAST(const Location &loc_) noexcept
        : AST{ASTSymType::ParamDecltr, loc_}
    {
    }

    ParamDecltrAST(Ptr<SPLCContext> typeContext_, const Location &loc_) noexcept
        : AST{typeContext_, ASTSymType::ParamDecltr, loc_}
    {
    }

    virtual void computeLangType() noexcept override
    {
        setLangType(builtInComputeLangType());
    }
};

/// CompStmt-AST declaration slot

/// GeneralStmtList-AST declaration slot

/// Stmt-AST declaration slot

/// ExprStmt-AST declaration slot

/// SelStmt-AST declaration slot

/// LabeledStmt-AST declaration slot

/// JumpStmt-AST declaration slot

/// IterStmt-AST declaration slot

/// ForLoopBody-AST declaration slot

/// ConstExpr-AST declaration slot

/// Constant-AST declaration slot

/// PrimaryExpr-AST declaration slot

/// PostfixExpr-AST declaration slot

/// MemberAccessOp-AST declaration slot

/// UnaryExpr-AST declaration slot

/// UnaryArithOp-AST declaration slot

/// CastExpr-AST declaration slot

/// MulExpr-AST declaration slot

/// MulOp-AST declaration slot

/// DivOp-AST declaration slot

/// AddExpr-AST declaration slot

/// AddOp-AST declaration slot

/// ShiftExpr-AST declaration slot

/// ShiftOp-AST declaration slot

/// RelExpr-AST declaration slot

/// RelOp-AST declaration slot

/// EqualityExpr-AST declaration slot

/// EqualityOp-AST declaration slot

/// OpBAndExpr-AST declaration slot

/// OpBXorExpr-AST declaration slot

/// OpBOrExpr-AST declaration slot

/// LogicalOpAndExpr-AST declaration slot

/// LogicalOpOrExpr-AST declaration slot

/// CondExpr-AST declaration slot

/// AssignExpr-AST declaration slot

/// AssignOp-AST declaration slot

/// Expr-AST declaration slot

/// InitExpr-AST declaration slot

/// ArgList-AST declaration slot

/// StringLiteral-AST declaration slot

/// IDWrapper-AST declaration slot

} // namespace splc

#endif // __SPLC_AST_AST_HH__