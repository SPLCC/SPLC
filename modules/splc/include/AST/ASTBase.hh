#ifndef __SPLC_AST_ASTBASE_HH__
#define __SPLC_AST_ASTBASE_HH__ 1

#include <AST/ASTCommons.hh>
#include <AST/Value.hh>
#include <Core/splc.hh>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <variant>
#include <vector>

namespace splc {

// forward declarations
// Must sync with classes in DerivedAST.hh

class DeclSpecAST;
class DecltrAST;
class DirDecltrAST;
class WrappedDirDecltrAST;
class InitDecltrAST;
class FuncDecltrAST;
class DirFuncDecltrAST;
class ParamListAST;
class ParamDecltrAST;

///
/// \brief Class AST describes a single node in the Abstract Syntax Tree
/// (AST), and acts as the foundation of the parse tree.
///
/// AST accepts the following value types as constant values, if given:
/// - `Char`
/// - `Unsigned long long`
/// - `Signed long long`
/// - `Double`
/// - `String`
///
/// To make the template helper function `makeAST` work properly, subclasses of
/// AST must implement the existing implicit constructors inside class AST.
/// Namely, constructors that accept parameters of sequence (Ptr<SPLCContext>,
/// optional) ASTSymType, Location, T value. Each subclass of AST shall perform
/// the necessary actions in their constructor, such that no more actions inside
/// the parser would be needed. Despite that, each AST class must also support
/// explicit constructor/construtor that accepts no parameter, to facilitate
/// development without the actual IO library.
///
class AST : public std::enable_shared_from_this<AST> {
    //===----------------------------------------------------------------------===//
    // Helper classes
    friend class ASTHelper;
    friend class ASTProcessor;
    friend class ASTContext;
    friend class ASTContextManager;
    friend class Type;
    friend class Value;

    //===----------------------------------------------------------------------===//
    // Polymorphism
    friend class DeclSpecAST;

    //===----------------------------------------------------------------------===//
    // Constructors and Accessors
  public:
    ///
    /// \brief This constructor should be called by AST internal method
    ///
    explicit AST(ASTSymType symType_ = ASTSymType::YYEMPTY) noexcept
        : symType{symType_}
    {
    }

    template <IsValidASTValue T = ASTValueType>
    AST(const ASTSymType symType_, const Location &loc_,
        T &&value_ = {}) noexcept
        : symType{symType_}, loc{loc_}, value{value_}
    {
    }

    template <IsValidASTValue T = ASTValueType>
    AST(SPLCContext &C, const ASTSymType symType_, const Location &loc_,
        T &&value_ = {}) noexcept
        : context{&C}, symType{symType_}, loc{loc_}, value{value_}
    {
    }

    virtual ~AST() = default;

    virtual AST &operator=(const AST &other) = default;

    virtual AST &operator=(AST &&other) = default;

    template <AllArePtrAST... Children>
    static PtrAST make(ASTSymType type, const Location &loc,
                       Children &&...children)
    {
        PtrAST parentNode = makeSharedPtr<AST>(type, loc);
        parentNode->addChildren(std::forward<Children>(children)...);
        return parentNode;
    }

    template <IsValidASTValue T, AllArePtrAST... Children>
    static PtrAST make(ASTSymType type, const Location &loc, T &&value,
                       Children &&...children)
    {
        PtrAST parentNode =
            makeSharedPtr<AST>(type, loc, std::forward<T>(value));
        parentNode->addChildren(std::forward<Children>(children)...);
        return parentNode;
    }

    template <AllArePtrAST... Children>
    static PtrAST make(SPLCContext &C, ASTSymType type, const Location &loc,
                       Children &&...children)
    {
        PtrAST parentNode = makeSharedPtr<AST>(C, type, loc);
        parentNode->addChildren(std::forward<Children>(children)...);
        return parentNode;
    }

    template <IsValidASTValue T, AllArePtrAST... Children>
    static PtrAST make(SPLCContext &C, ASTSymType type, const Location &loc,
                       T &&value, Children &&...children)
    {
        PtrAST parentNode =
            makeSharedPtr<AST>(C, type, loc, std::forward<T>(value));
        parentNode->addChildren(std::forward<Children>(children)...);
        return parentNode;
    }

    template <IsBaseAST ASTTypeLike, AllArePtrAST... Children>
    static Ptr<ASTTypeLike> makeDerived(const Location &loc,
                                        Children &&...children)
    {
        Ptr<ASTTypeLike> parentNode = makeSharedPtr<ASTTypeLike>(loc);
        parentNode->addChildren(std::forward<Children>(children)...);
        return parentNode;
    }

    template <IsBaseAST ASTTypeLike, AllArePtrAST... Children>
    static Ptr<ASTTypeLike> makeDerived(SPLCContext &C, const Location &loc,
                                        Children &&...children)
    {
        Ptr<ASTTypeLike> parentNode = makeSharedPtr<ASTTypeLike>(C, loc);
        parentNode->addChildren(std::forward<Children>(children)...);
        return parentNode;
    }

    ///
    /// \deprecated This function serves no particular purpose.
    /// \brief Acquire a deep copy of the current PtrAST method.
    /// By default, all children are copied.
    ///
    virtual PtrAST copy(
        const std::function<bool(Ptr<const AST>)> &predicate =
            [](Ptr<const AST>) { return true; },
        const bool copyContext = true) const;

    virtual std::vector<ASTDeclEntityType> getNamedDeclEntities() const;

    Type *computeSimpleTypeSpec() const noexcept;

    Type *computeSimpleDecltrList() const noexcept;

    virtual void setLangType(Type *langType_) const noexcept
    {
        langType = langType_;
        isLangTypeSet_ = true;
    }

    bool isLangTypeSet() const noexcept { return isLangTypeSet_; }

    void setLangTypeSet(bool isLangTypeSet__) const noexcept
    {
        isLangTypeSet_ = isLangTypeSet__;
    }

    virtual Type *computeAndSetLangType(Type *baseType = nullptr) const noexcept
    {
        if (typeid(*this) == typeid(AST)) {
            splc_error()
                << ": computing type on primitive AST nodes is not allowed. "
                   "Check whether this is a programming mistake.";
        }
        return nullptr;
    }

    Type *getLangType() const noexcept { return langType; }

    bool isTypedef() const noexcept { return false; }

    void setTypedef(bool isTypedef__) const noexcept
    {
        isTypedef_ = isTypedef__;
    }

    static inline bool isASTAppendable(const AST &node) noexcept
    {
        return !node.isSymTypeOneOf(ASTSymType::YYEMPTY, ASTSymType::YYEOF,
                                    ASTSymType::YYerror);
    }

    virtual void addChild(PtrAST child) noexcept
    {
        children_.push_back(child);
        child->context = this->context;
        child->parent = shared_from_this();
        this->loc += child->loc; // TODO: check if required

        // Update type information
        setLangTypeSet(false);
    }

    template <AllArePtrAST... Children>
    void addChildren(Children &&...children)
    {
        ((children && isASTAppendable(*castToPtrASTBase(children))
              ? addChild(castToPtrASTBase(std::forward<Children>(children)))
              : void()),
         ...);
    }

    constexpr bool hasConstVal() const noexcept { return value.index() != 0; }

    template <IsValidASTValue T>
    auto &getConstVal() noexcept
    {
        return std::get<T>(value);
    }

    template <IsValidASTValue T>
    auto &getConstVal() const noexcept
    {
        return std::get<T>(value);
    }

    template <class Visitor>
    auto visitConstVal(Visitor &&vis) noexcept
    {
        return std::visit(vis, value);
    }

    template <class Visitor>
    auto visitConstVal(Visitor &&vis) const noexcept
    {
        return std::visit(vis, value);
    }

    template <IsValidASTValue T>
    void emplaceConstVal(T &&val) noexcept
    {
        value.emplace<T>(std::forward(val));
    }

    template <IsValidASTValue T>
    constexpr bool holdsConstType() const noexcept
    {
        return std::holds_alternative<T>(value);
    }

    void setContext(SPLCContext *context_) { context = context_; }

    auto getContext() { return context; }

    auto getContext() const { return context; }

    auto getSymType() const noexcept { return symType; }

    template <AllAreASTSymbolType... OtherTypes>
    bool isSymTypeOneOf(OtherTypes &&...othertypes) const noexcept
    {
        return isASTSymbolTypeOneOf(getSymType(),
                                    std::forward<OtherTypes>(othertypes)...);
    }

    auto &getContainedTys() { return containedTys; }

    auto &getContainedTys() const { return containedTys; }

    auto getParent() noexcept { return parent; }

    auto getParent() const noexcept { return parent; }

    auto getChildrenNum() const noexcept { return children_.size(); }

    auto isChildrenEmpty() const noexcept { return children_.empty(); }

    auto &getChildren() noexcept { return children_; }

    auto &getChildren() const noexcept { return children_; }

    auto &getLocation() noexcept { return loc; }

    auto &getLocation() const noexcept { return loc; }

    void setASTContext(Ptr<ASTContext> astContext_) noexcept
    {
        astContext = astContext_;
    }

    auto getASTContext() noexcept { return astContext; }

    auto getASTContext() const noexcept { return astContext; }

    auto &getVariant() noexcept { return value; }

  protected:
    SPLCContext *context = nullptr;
    ASTSymType symType;
    mutable Type *langType =
        nullptr; ///< type related to this AST, e.g., type for specifiers.
    mutable bool isLangTypeSet_ = false;
    mutable bool isTypedef_ = false;
    mutable std::vector<Type *> containedTys;
    WeakPtrAST parent;
    std::vector<PtrAST> children_;
    Location loc;
    Ptr<ASTContext> astContext;
    ASTValueType value;

    //===----------------------------------------------------------------------===//
    // Runtime Polymorphism
  public:
    // clang-format off

    bool isYYNTOKENS() const noexcept { return getSymType() == ASTSymType::YYNTOKENS; }

    bool isYYEMPTY() const noexcept { return getSymType() == ASTSymType::YYEMPTY; }

    bool isYYEOF() const noexcept { return getSymType() == ASTSymType::YYEOF; }

    bool isYYerror() const noexcept { return getSymType() == ASTSymType::YYerror; }

    bool isYYUNDEF() const noexcept { return getSymType() == ASTSymType::YYUNDEF; }

    bool isKwdAuto() const noexcept { return getSymType() == ASTSymType::KwdAuto; }

    bool isKwdExtern() const noexcept { return getSymType() == ASTSymType::KwdExtern; }

    bool isKwdRegister() const noexcept { return getSymType() == ASTSymType::KwdRegister; }

    bool isKwdStatic() const noexcept { return getSymType() == ASTSymType::KwdStatic; }

    bool isKwdTypedef() const noexcept { return getSymType() == ASTSymType::KwdTypedef; }

    bool isKwdConst() const noexcept { return getSymType() == ASTSymType::KwdConst; }

    bool isKwdRestrict() const noexcept { return getSymType() == ASTSymType::KwdRestrict; }

    bool isKwdVolatile() const noexcept { return getSymType() == ASTSymType::KwdVolatile; }

    bool isKwdInline() const noexcept { return getSymType() == ASTSymType::KwdInline; }

    bool isVoidTy() const noexcept { return getSymType() == ASTSymType::VoidTy; }

    bool isCharTy() const noexcept { return getSymType() == ASTSymType::CharTy; }

    bool isShortTy() const noexcept { return getSymType() == ASTSymType::ShortTy; }

    bool isIntTy() const noexcept { return getSymType() == ASTSymType::IntTy; }

    bool isSignedTy() const noexcept { return getSymType() == ASTSymType::SignedTy; }

    bool isUnsignedTy() const noexcept { return getSymType() == ASTSymType::UnsignedTy; }

    bool isLongTy() const noexcept { return getSymType() == ASTSymType::LongTy; }

    bool isFloatTy() const noexcept { return getSymType() == ASTSymType::FloatTy; }

    bool isDoubleTy() const noexcept { return getSymType() == ASTSymType::DoubleTy; }

    bool isKwdEnum() const noexcept { return getSymType() == ASTSymType::KwdEnum; }

    bool isKwdStruct() const noexcept { return getSymType() == ASTSymType::KwdStruct; }

    bool isKwdUnion() const noexcept { return getSymType() == ASTSymType::KwdUnion; }

    bool isKwdIf() const noexcept { return getSymType() == ASTSymType::KwdIf; }

    bool isKwdElse() const noexcept { return getSymType() == ASTSymType::KwdElse; }

    bool isKwdSwitch() const noexcept { return getSymType() == ASTSymType::KwdSwitch; }

    bool isKwdWhile() const noexcept { return getSymType() == ASTSymType::KwdWhile; }

    bool isKwdFor() const noexcept { return getSymType() == ASTSymType::KwdFor; }

    bool isKwdDo() const noexcept { return getSymType() == ASTSymType::KwdDo; }

    bool isKwdDefault() const noexcept { return getSymType() == ASTSymType::KwdDefault; }

    bool isKwdCase() const noexcept { return getSymType() == ASTSymType::KwdCase; }

    bool isKwdGoto() const noexcept { return getSymType() == ASTSymType::KwdGoto; }

    bool isKwdContinue() const noexcept { return getSymType() == ASTSymType::KwdContinue; }

    bool isKwdBreak() const noexcept { return getSymType() == ASTSymType::KwdBreak; }

    bool isKwdReturn() const noexcept { return getSymType() == ASTSymType::KwdReturn; }

    bool isID() const noexcept { return getSymType() == ASTSymType::ID; }

    bool isTypedefID() const noexcept { return getSymType() == ASTSymType::TypedefID; }

    bool isOpAssign() const noexcept { return getSymType() == ASTSymType::OpAssign; }

    bool isOpMulAssign() const noexcept { return getSymType() == ASTSymType::OpMulAssign; }

    bool isOpDivAssign() const noexcept { return getSymType() == ASTSymType::OpDivAssign; }

    bool isOpModAssign() const noexcept { return getSymType() == ASTSymType::OpModAssign; }

    bool isOpPlusAssign() const noexcept { return getSymType() == ASTSymType::OpPlusAssign; }

    bool isOpMinusAssign() const noexcept { return getSymType() == ASTSymType::OpMinusAssign; }

    bool isOpLShiftAssign() const noexcept { return getSymType() == ASTSymType::OpLShiftAssign; }

    bool isOpRShiftAssign() const noexcept { return getSymType() == ASTSymType::OpRShiftAssign; }

    bool isOpBAndAssign() const noexcept { return getSymType() == ASTSymType::OpBAndAssign; }

    bool isOpBXorAssign() const noexcept { return getSymType() == ASTSymType::OpBXorAssign; }

    bool isOpBOrAssign() const noexcept { return getSymType() == ASTSymType::OpBOrAssign; }

    bool isOpAnd() const noexcept { return getSymType() == ASTSymType::OpAnd; }

    bool isOpOr() const noexcept { return getSymType() == ASTSymType::OpOr; }

    bool isOpNot() const noexcept { return getSymType() == ASTSymType::OpNot; }

    bool isOpLT() const noexcept { return getSymType() == ASTSymType::OpLT; }

    bool isOpLE() const noexcept { return getSymType() == ASTSymType::OpLE; }

    bool isOpGT() const noexcept { return getSymType() == ASTSymType::OpGT; }

    bool isOpGE() const noexcept { return getSymType() == ASTSymType::OpGE; }

    bool isOpNE() const noexcept { return getSymType() == ASTSymType::OpNE; }

    bool isOpEQ() const noexcept { return getSymType() == ASTSymType::OpEQ; }

    bool isOpQMark() const noexcept { return getSymType() == ASTSymType::OpQMark; }

    bool isOpColon() const noexcept { return getSymType() == ASTSymType::OpColon; }

    bool isOpLShift() const noexcept { return getSymType() == ASTSymType::OpLShift; }

    bool isOpRShift() const noexcept { return getSymType() == ASTSymType::OpRShift; }

    bool isOpBAnd() const noexcept { return getSymType() == ASTSymType::OpBAnd; }

    bool isOpBOr() const noexcept { return getSymType() == ASTSymType::OpBOr; }

    bool isOpBNot() const noexcept { return getSymType() == ASTSymType::OpBNot; }

    bool isOpBXor() const noexcept { return getSymType() == ASTSymType::OpBXor; }

    bool isOpDPlus() const noexcept { return getSymType() == ASTSymType::OpDPlus; }

    bool isOpDMinus() const noexcept { return getSymType() == ASTSymType::OpDMinus; }

    bool isOpPlus() const noexcept { return getSymType() == ASTSymType::OpPlus; }

    bool isOpMinus() const noexcept { return getSymType() == ASTSymType::OpMinus; }

    bool isOpAstrk() const noexcept { return getSymType() == ASTSymType::OpAstrk; }

    bool isOpDiv() const noexcept { return getSymType() == ASTSymType::OpDiv; }

    bool isOpMod() const noexcept { return getSymType() == ASTSymType::OpMod; }

    bool isOpDot() const noexcept { return getSymType() == ASTSymType::OpDot; }

    bool isOpRArrow() const noexcept { return getSymType() == ASTSymType::OpRArrow; }

    bool isOpSizeOf() const noexcept { return getSymType() == ASTSymType::OpSizeOf; }

    bool isOpLSB() const noexcept { return getSymType() == ASTSymType::OpLSB; }

    bool isOpRSB() const noexcept { return getSymType() == ASTSymType::OpRSB; }

    bool isOpComma() const noexcept { return getSymType() == ASTSymType::OpComma; }

    bool isOpEllipsis() const noexcept { return getSymType() == ASTSymType::OpEllipsis; }

    bool isPSemi() const noexcept { return getSymType() == ASTSymType::PSemi; }

    bool isPLC() const noexcept { return getSymType() == ASTSymType::PLC; }

    bool isPRC() const noexcept { return getSymType() == ASTSymType::PRC; }

    bool isPLP() const noexcept { return getSymType() == ASTSymType::PLP; }

    bool isPRP() const noexcept { return getSymType() == ASTSymType::PRP; }

    bool isUIntLiteral() const noexcept { return getSymType() == ASTSymType::UIntLiteral; }

    bool isSIntLiteral() const noexcept { return getSymType() == ASTSymType::SIntLiteral; }

    bool isFloatLiteral() const noexcept { return getSymType() == ASTSymType::FloatLiteral; }

    bool isCharLiteral() const noexcept { return getSymType() == ASTSymType::CharLiteral; }

    bool isStrUnit() const noexcept { return getSymType() == ASTSymType::StrUnit; }

    bool isSubscriptExpr() const noexcept { return getSymType() == ASTSymType::SubscriptExpr; }

    bool isCallExpr() const noexcept { return getSymType() == ASTSymType::CallExpr; }

    bool isAccessExpr() const noexcept { return getSymType() == ASTSymType::AccessExpr; }

    bool isExplicitCastExpr() const noexcept { return getSymType() == ASTSymType::ExplicitCastExpr; }

    bool isAddrOfExpr() const noexcept { return getSymType() == ASTSymType::AddrOfExpr; }

    bool isDerefExpr() const noexcept { return getSymType() == ASTSymType::DerefExpr; }

    bool isSizeOfExpr() const noexcept { return getSymType() == ASTSymType::SizeOfExpr; }

    bool isKwdThen() const noexcept { return getSymType() == ASTSymType::KwdThen; }

    bool isDecltrPrec() const noexcept { return getSymType() == ASTSymType::DecltrPrec; }

    bool isFuncDeclPrec() const noexcept { return getSymType() == ASTSymType::FuncDeclPrec; }

    bool isOpUnaryPrec() const noexcept { return getSymType() == ASTSymType::OpUnaryPrec; }

    bool isPLParen() const noexcept { return getSymType() == ASTSymType::PLParen; }

    bool isPRParen() const noexcept { return getSymType() == ASTSymType::PRParen; }

    bool isPLSBracket() const noexcept { return getSymType() == ASTSymType::PLSBracket; }

    bool isPRSBracket() const noexcept { return getSymType() == ASTSymType::PRSBracket; }

    bool isYYACCEPT() const noexcept { return getSymType() == ASTSymType::YYACCEPT; }

    bool isParseRoot() const noexcept { return getSymType() == ASTSymType::ParseRoot; }

    bool isTransUnit() const noexcept { return getSymType() == ASTSymType::TransUnit; }

    bool isExternDeclList() const noexcept { return getSymType() == ASTSymType::ExternDeclList; }

    bool isExternDecl() const noexcept { return getSymType() == ASTSymType::ExternDecl; }

    bool isDeclSpec() const noexcept { return getSymType() == ASTSymType::DeclSpec; }

    bool isStorageSpec() const noexcept { return getSymType() == ASTSymType::StorageSpec; }

    bool isSpecQualList() const noexcept { return getSymType() == ASTSymType::SpecQualList; }

    bool isTypeSpec() const noexcept { return getSymType() == ASTSymType::TypeSpec; }

    bool isFuncSpec() const noexcept { return getSymType() == ASTSymType::FuncSpec; }

    bool isTypeQual() const noexcept { return getSymType() == ASTSymType::TypeQual; }

    bool isTypeName() const noexcept { return getSymType() == ASTSymType::TypeName; }

    bool isBuiltinTypeSpec() const noexcept { return getSymType() == ASTSymType::BuiltinTypeSpec; }

    bool isAbsDecltr() const noexcept { return getSymType() == ASTSymType::AbsDecltr; }

    bool isDirAbsDecltr() const noexcept { return getSymType() == ASTSymType::DirAbsDecltr; }

    bool isStructOrUnionSpec() const noexcept { return getSymType() == ASTSymType::StructOrUnionSpec; }

    bool isStructOrUnion() const noexcept { return getSymType() == ASTSymType::StructOrUnion; }

    bool isStructDeclBody() const noexcept { return getSymType() == ASTSymType::StructDeclBody; }

    bool isStructDeclList() const noexcept { return getSymType() == ASTSymType::StructDeclList; }

    bool isStructDecl() const noexcept { return getSymType() == ASTSymType::StructDecl; }

    bool isStructDecltrList() const noexcept { return getSymType() == ASTSymType::StructDecltrList; }

    bool isStructDecltr() const noexcept { return getSymType() == ASTSymType::StructDecltr; }

    bool isEnumSpec() const noexcept { return getSymType() == ASTSymType::EnumSpec; }

    bool isEnumBody() const noexcept { return getSymType() == ASTSymType::EnumBody; }

    bool isEnumeratorList() const noexcept { return getSymType() == ASTSymType::EnumeratorList; }

    bool isEnumerator() const noexcept { return getSymType() == ASTSymType::Enumerator; }

    bool isEnumConst() const noexcept { return getSymType() == ASTSymType::EnumConst; }

    bool isDecltr() const noexcept { return getSymType() == ASTSymType::Decltr; }

    bool isDirDecltr() const noexcept { return getSymType() == ASTSymType::DirDecltr; }

    bool isWrappedDirDecltr() const noexcept { return getSymType() == ASTSymType::WrappedDirDecltr; }

    bool isTypeQualList() const noexcept { return getSymType() == ASTSymType::TypeQualList; }

    bool isDecl() const noexcept { return getSymType() == ASTSymType::Decl; }

    bool isDirDecl() const noexcept { return getSymType() == ASTSymType::DirDecl; }

    bool isInitDecltrList() const noexcept { return getSymType() == ASTSymType::InitDecltrList; }

    bool isInitDecltr() const noexcept { return getSymType() == ASTSymType::InitDecltr; }

    bool isInitializer() const noexcept { return getSymType() == ASTSymType::Initializer; }

    bool isInitializerList() const noexcept { return getSymType() == ASTSymType::InitializerList; }

    bool isDesignation() const noexcept { return getSymType() == ASTSymType::Designation; }

    bool isDesignatorList() const noexcept { return getSymType() == ASTSymType::DesignatorList; }

    bool isDesignator() const noexcept { return getSymType() == ASTSymType::Designator; }

    bool isFuncDef() const noexcept { return getSymType() == ASTSymType::FuncDef; }

    bool isFuncDecl() const noexcept { return getSymType() == ASTSymType::FuncDecl; }

    bool isFuncDecltr() const noexcept { return getSymType() == ASTSymType::FuncDecltr; }

    bool isDirFuncDecltr() const noexcept { return getSymType() == ASTSymType::DirFuncDecltr; }

    bool isDirDecltrForFunc() const noexcept { return getSymType() == ASTSymType::DirDecltrForFunc; }

    bool isParamTypeList() const noexcept { return getSymType() == ASTSymType::ParamTypeList; }

    bool isParamList() const noexcept { return getSymType() == ASTSymType::ParamList; }

    bool isParamDecltr() const noexcept { return getSymType() == ASTSymType::ParamDecltr; }

    bool isCompStmt() const noexcept { return getSymType() == ASTSymType::CompStmt; }

    bool isGeneralStmtList() const noexcept { return getSymType() == ASTSymType::GeneralStmtList; }

    bool isStmt() const noexcept { return getSymType() == ASTSymType::Stmt; }

    bool isExprStmt() const noexcept { return getSymType() == ASTSymType::ExprStmt; }

    bool isSelStmt() const noexcept { return getSymType() == ASTSymType::SelStmt; }

    bool isLabeledStmt() const noexcept { return getSymType() == ASTSymType::LabeledStmt; }

    bool isJumpStmt() const noexcept { return getSymType() == ASTSymType::JumpStmt; }

    bool isIterStmt() const noexcept { return getSymType() == ASTSymType::IterStmt; }

    bool isForLoopBody() const noexcept { return getSymType() == ASTSymType::ForLoopBody; }

    bool isConstExpr() const noexcept { return getSymType() == ASTSymType::ConstExpr; }

    bool isConstant() const noexcept { return getSymType() == ASTSymType::Constant; }

    bool isPrimaryExpr() const noexcept { return getSymType() == ASTSymType::PrimaryExpr; }

    bool isPostfixExpr() const noexcept { return getSymType() == ASTSymType::PostfixExpr; }

    bool isMemberAccessOp() const noexcept { return getSymType() == ASTSymType::MemberAccessOp; }

    bool isUnaryExpr() const noexcept { return getSymType() == ASTSymType::UnaryExpr; }

    bool isUnaryArithOp() const noexcept { return getSymType() == ASTSymType::UnaryArithOp; }

    bool isCastExpr() const noexcept { return getSymType() == ASTSymType::CastExpr; }

    bool isMulExpr() const noexcept { return getSymType() == ASTSymType::MulExpr; }

    bool isMulOp() const noexcept { return getSymType() == ASTSymType::MulOp; }

    bool isDivOp() const noexcept { return getSymType() == ASTSymType::DivOp; }

    bool isAddExpr() const noexcept { return getSymType() == ASTSymType::AddExpr; }

    bool isAddOp() const noexcept { return getSymType() == ASTSymType::AddOp; }

    bool isShiftExpr() const noexcept { return getSymType() == ASTSymType::ShiftExpr; }

    bool isShiftOp() const noexcept { return getSymType() == ASTSymType::ShiftOp; }

    bool isRelExpr() const noexcept { return getSymType() == ASTSymType::RelExpr; }

    bool isRelOp() const noexcept { return getSymType() == ASTSymType::RelOp; }

    bool isEqualityExpr() const noexcept { return getSymType() == ASTSymType::EqualityExpr; }

    bool isEqualityOp() const noexcept { return getSymType() == ASTSymType::EqualityOp; }

    bool isOpBAndExpr() const noexcept { return getSymType() == ASTSymType::OpBAndExpr; }

    bool isOpBXorExpr() const noexcept { return getSymType() == ASTSymType::OpBXorExpr; }

    bool isOpBOrExpr() const noexcept { return getSymType() == ASTSymType::OpBOrExpr; }

    bool isLogicalOpAndExpr() const noexcept { return getSymType() == ASTSymType::LogicalOpAndExpr; }

    bool isLogicalOpOrExpr() const noexcept { return getSymType() == ASTSymType::LogicalOpOrExpr; }

    bool isCondExpr() const noexcept { return getSymType() == ASTSymType::CondExpr; }

    bool isAssignExpr() const noexcept { return getSymType() == ASTSymType::AssignExpr; }

    bool isAssignOp() const noexcept { return getSymType() == ASTSymType::AssignOp; }

    bool isExpr() const noexcept { return getSymType() == ASTSymType::Expr; }

    bool isInitExpr() const noexcept { return getSymType() == ASTSymType::InitExpr; }

    bool isArgList() const noexcept { return getSymType() == ASTSymType::ArgList; }

    bool isStringLiteral() const noexcept { return getSymType() == ASTSymType::StringLiteral; }

    bool isIDWrapper() const noexcept { return getSymType() == ASTSymType::IDWrapper; }
    // clang-format on

    bool isPrimitiveType() const noexcept
    {
        return isVoidTy() || isCharTy() || isShortTy() || isIntTy() ||
               isUnsignedTy() || isSignedTy() || isLongTy() || isFloatTy() ||
               isDoubleTy();
    }

    //===----------------------------------------------------------------------===//
    // Casts. These methods are implemented in DerivedAST.cc

    Ptr<DeclSpecAST> toDeclSpecAST() noexcept;

    Ptr<ParamListAST> toParamListAST() noexcept;

    //===----------------------------------------------------------------------===//
    // Helper Methods
  public:
    ///
    /// Allow stream-like operation on ASTs for processing.
    ///
    template <IsBaseAST T, class Functor>
        requires AllApplicableOnAST<T, Functor>
    friend T &&operator>>(T &&node, Functor &&functor);

    ///
    /// \brief Apply all `functors` on the target `node`, returning the
    /// transformed version. Transforms are applied in a sequential manner as
    /// given in the parameters.
    ///
    template <IsBaseAST T, class... Functors>
        requires AllApplicableOnAST<T, Functors...>
    friend T &&applyASTTransform(T &&node, Functors &&...functors);

    template <AllAreASTSymbolType... OtherTypes>
    Ptr<AST> findFirstChild(OtherTypes &&...otherTypes) noexcept
    {
        auto it = std::find_if(
            getChildren().begin(), getChildren().end(),
            [&](const auto &p) { return p->isSymTypeOneOf(otherTypes...); });
        return it == getChildren().end() ? nullptr : *it;
    }

    template <AllAreASTSymbolType... OtherTypes>
    Ptr<const AST> findFirstChild(OtherTypes &&...otherTypes) const noexcept
    {
        auto it = std::find_if(
            getChildren().begin(), getChildren().end(),
            [&](const auto &p) { return p->isSymTypeOneOf(otherTypes...); });
        return it == getChildren().end() ? nullptr : *it;
    }

    template <AllAreASTSymbolType... OtherTypes>
    Ptr<AST> findFirstChildDFS(OtherTypes &&...otherTypes) noexcept
    {
        auto it0 = std::find_if(
            getChildren().begin(), getChildren().end(),
            [&](const auto p) { return p->isSymTypeOneOf(otherTypes...); });
        if (it0 != getChildren().end()) {
            return (*it0);
        }
        for (auto &child : getChildren()) {
            auto p = child->findFirstChildDFS(otherTypes...);
            if (p != nullptr) {
                return p;
            }
        }
        return nullptr;
    }

    template <AllAreASTSymbolType... OtherTypes>
    Ptr<const AST> findFirstChildDFS(OtherTypes &&...otherTypes) const noexcept
    {
        auto it0 = std::find_if(
            getChildren().begin(), getChildren().end(),
            [&](const auto p) { return p->isSymTypeOneOf(otherTypes...); });
        if (it0 != getChildren().end()) {
            return (*it0);
        }
        for (auto &child : getChildren()) {
            auto p = child->findFirstChildDFS(otherTypes...);
            if (p != nullptr) {
                return p;
            }
        }
        return nullptr;
    }

    template <AllAreASTSymbolType... OtherTypes>
    Ptr<AST> findFirstChildBFS(OtherTypes &&...otherTypes) noexcept
    {
        std::vector<AST *> nodes = {this};

        // Simulate BFS
        while (!nodes.empty()) {
            std::vector<AST *> childNodes;
            for (auto node : nodes) {
                for (auto node : nodes) {
                    childNodes.reserve(childNodes.size() +
                                       node->getChildrenNum());
                    std::transform(node->getChildren().begin(),
                                   node->getChildren().end(),
                                   std::back_inserter(childNodes),
                                   [&](auto &ptr) { return ptr.get(); });
                }
            }
            nodes = childNodes;

            auto it0 =
                std::find_if(nodes.begin(), nodes.end(), [&](const auto p) {
                    return p->isSymTypeOneOf(
                        std::forward<OtherTypes>(otherTypes)...);
                });
            if (it0 != nodes.end()) {
                return (*it0)->shared_from_this();
            }
        }

        return nullptr;
    }

    template <AllAreASTSymbolType... OtherTypes>
    Ptr<const AST> findFirstChildBFS(OtherTypes &&...otherTypes) const noexcept
    {
        std::vector<const AST *> nodes = {this};

        // Simulate BFS
        while (!nodes.empty()) {
            std::vector<const AST *> childNodes;
            for (auto node : nodes) {
                for (auto node : nodes) {
                    childNodes.reserve(childNodes.size() +
                                       node->getChildrenNum());
                    std::transform(node->getChildren().begin(),
                                   node->getChildren().end(),
                                   std::back_inserter(childNodes),
                                   [&](auto &ptr) { return ptr.get(); });
                }
            }
            nodes = childNodes;

            auto it0 =
                std::find_if(nodes.begin(), nodes.end(), [&](const auto p) {
                    return p->isSymTypeOneOf(
                        std::forward<OtherTypes>(otherTypes)...);
                });
            if (it0 != nodes.end()) {
                return (*it0)->shared_from_this();
            }
        }

        return nullptr;
    }

    /// Call this function on function declarators or declarators to get the
    /// the deepest ID node.
    Ptr<AST> getRootIDNode() noexcept;

    /// Call this function on function declarators or declarators to get the
    /// the deepest ID node.
    Ptr<const AST> getRootIDNode() const noexcept;

    /// Call this function on function declarators or declarators to get the
    /// type of the deepest ID node.
    Type *getRootIDLangType() const noexcept;

    /// Call this function on function declarators or declarators to get the
    /// ID of the deepest ID node.
    std::string_view getRootID() const noexcept;

    ///
    /// Print information of this single node.
    ///
    friend std::ostream &operator<<(std::ostream &os, const AST &node) noexcept;

    class ASTRecursivePrintManipulator {
      public:
        friend ASTRecursivePrintManipulator
        treePrintTransform(const AST &node) noexcept;

        friend std::ostream &
        operator<<(std::ostream &os,
                   const AST::ASTRecursivePrintManipulator &m) noexcept;

      private:
        ASTRecursivePrintManipulator(const AST &node_) : node{node_} {}
        const AST &node;
    };

    friend void recursivePrintNode(std::ostream &os, const AST &node,
                                   const std::string &prefix) noexcept;
    friend std::ostream &
    operator<<(std::ostream &os,
               const AST::ASTRecursivePrintManipulator &m) noexcept;
}; // class: AST

template <IsBaseAST T, class Functor>
    requires AllApplicableOnAST<T, Functor>
inline T &&operator>>(T &&node, Functor &&functor)
{
    return std::forward<Functor>(functor)(std::forward<T>(node));
}

template <IsBaseAST T, class... Functors>
    requires AllApplicableOnAST<T, Functors...>
inline T &&applyASTTransform(T &&node, Functors &&...functors)
{
    return (functors(std::forward<T>(node)), ...);
}

inline AST::ASTRecursivePrintManipulator
treePrintTransform(const AST &node) noexcept
{
    return {node};
}

inline void recursivePrintNode(std::ostream &os, const AST &node,
                               const std::string &prefix) noexcept
{
    using namespace std::string_view_literals;
    using utils::logging::ControlSeq;

    static const std::string treeMidArrow = "|-";
    static const std::string midSegment = "| ";
    static const std::string treeEndArrow = "`-";
    static const std::string noSegment = "  ";

    std::string newPrefix;

    for (auto &child : node.children_) {
        os << ControlSeq::Blue;
        if (child == node.children_.back())
            newPrefix = prefix + treeEndArrow;
        else
            newPrefix = prefix + treeMidArrow;

        os << newPrefix;
        os << ControlSeq::Reset;

        os << *child << "\n";

        if (child == node.children_.back())
            newPrefix = prefix + noSegment;
        else
            newPrefix = prefix + midSegment;

        if (!child->children_.empty()) {
            recursivePrintNode(os, *child, newPrefix);
        }
    }
}

inline std::ostream &
operator<<(std::ostream &os,
           const AST::ASTRecursivePrintManipulator &m) noexcept
{
    os << m.node << "\n";
    astPrintMap.insert(m.node.loc.begin.contextID);
    recursivePrintNode(os, m.node, "");
    astPrintMap.clear();
    return os;
}

class ASTHelper {
  public:
    // TODO: document them

    static PtrAST makeDeclSpecifierTree(const Location &loc,
                                        ASTSymType specSymbolType);

    static Type *getBaseTySpec(const AST &root) noexcept;

    static std::vector<Type *> processInitDeclList(const AST &root,
                                                   Type *base) noexcept;

    static std::vector<Type *> getDeclTy(const AST &root) noexcept;

    static Type *getFuncTy(const AST &root) noexcept;

    static std::vector<Type *> getTypeHelperDispatch(const AST &root);

    static void getIDRecursive(std::vector<ASTDeclEntityType> &vec,
                               const AST &root) noexcept;

    static std::vector<ASTDeclEntityType>
    getNamedDeclRecursive(const AST &root) noexcept;

    static Type *getBaseTySpecRecursive(const AST &root) noexcept;

    static Type *processParamDeclRecursive(const AST &root) noexcept;

    static Type *processDecltrSubDispatch(const AST &root, Type *base) noexcept;

    static Type *processDecltrRecursive(const AST &root, Type *base) noexcept;

    static Type *processInitDecltr(const AST &root, Type *base) noexcept;

    // /// Transform Ptr<AST> containing Ptr, such that
    // /// The pointers form a rightmost tree hierarchy.
    // /// At the rightmost child of the tree shall the declarator lie.
    // static PtrAST getPtrDeclEndPoint(AST &root) noexcept;
};

} // namespace splc

#endif // __SPLC_AST_ASTBASE_HH__