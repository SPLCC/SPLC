#ifndef __SPLC_AST_ASTBASE_HH__
#define __SPLC_AST_ASTBASE_HH__ 1

#include <AST/ASTCommons.hh>
#include <AST/TypeContext.hh>
#include <AST/Value.hh>
#include <Core/splc.hh>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <variant>
#include <vector>

namespace splc {
///
/// \brief Class `AST` describes a single node in the Abstract Syntax Tree
/// (AST), and acts as the foundation of the parse tree.
///
/// `AST` accepts the following value types, if given:
/// - `Char`
/// - `Unsigned long long`
/// - `Double`
/// - `String`
///
class AST : public std::enable_shared_from_this<AST> {
    friend class ASTHelper;
    friend class ASTProcessor;
    friend class ASTContext;
    friend class ASTContextManager;
    friend class Type;
    friend class Value;

    //===----------------------------------------------------------------------===//
    // Runtime Polymorphism
  public:
    /// ID denote the variant of the AST instance. Each subclass of AST behaves
    /// differently, e.g., an ExprAST will deduce its type from its children. ID
    /// is used to determine the type AST and convert them at runtime.
    enum class ASTID {
        Universal,
        TransUnit,
        ExternDeclList,
        ExternDecl,
        FuncDef,
        Expr,
        CompStmt,
        Stmt,
        Decl,
        DirDecl,
        DeclSpec,
        InitDecltrList,
        InitDecltr,
        Decltr,
        AbsDecltr,
        Initializer,
        InitializerList,
        Designation,
        DesignatorList,
    };

    //===----------------------------------------------------------------------===//
    // Constructors and Accessors
  public:
    ///
    /// \brief This constructor should be called by AST internal method
    ///
    explicit AST() noexcept : symType{ASTSymType::YYEMPTY} {}

    template <IsValidASTValue T = ASTValueType>
    AST(const ASTSymType symType, const Location &loc_,
        T &&value_ = {}) noexcept
        : symType{symType}, loc{loc_}, value{value_}
    {
    }

    template <IsValidASTValue T = ASTValueType>
    AST(Ptr<TypeContext> typeContext, const ASTSymType symType,
        const Location &loc_, T &&value_ = {}) noexcept
        : tyContext{typeContext}, symType{symType}, loc{loc_}, value{value_}
    {
    }

    virtual ~AST() = default;

    virtual AST &operator=(const AST &other) = default;

    virtual AST &operator=(AST &&other) = default;

    ///
    /// \deprecated This function serves no particular purpose.
    /// \brief Acquire a deep copy of the current PtrAST method.
    /// By default, all children are copied.
    ///
    virtual PtrAST copy(
        const std::function<bool(Ptr<const AST>)> &predicate =
            [](Ptr<const AST>) { return true; },
        const bool copyContext = true) const;

    /// \brief [Experimental] You should call this only on FuntionDef/Decl
    /// nodes.
    virtual std::vector<Type *> getContainedTys() const;

    virtual std::vector<ASTDeclEntityType> getNamedDeclEntities() const;

    /// \brief [Experimental] You should call this only on FuntionDef/Decl
    /// nodes.
    auto getVarType() const { return varType; }

    // TODO(sem): semantic analysis generation
    virtual Value evaluate();

    static inline bool isASTAppendable(const AST &node) noexcept
    {
        return !node.isSymTypeOneOf(ASTSymType::YYEMPTY, ASTSymType::YYEOF,
                                    ASTSymType::YYerror);
    }

    virtual void addChild(PtrAST child) noexcept
    {
        children_.push_back(child);
        child->tyContext = this->tyContext;
        child->parent = shared_from_this();
        this->loc += child->loc; // TODO: check if required
    }

    template <AllArePtrAST... Children>
    void addChildren(Children &&...children)
    {
        ((children && isASTAppendable(*castToPtrASTBase(children))
              ? addChild(castToPtrASTBase(std::forward<Children>(children)))
              : void()),
         ...);
    }

    PtrAST findFirstChild(ASTSymType type) const noexcept;

    constexpr bool hasConstVal() const noexcept { return value.index() != 0; }

    template <IsValidASTValue T>
    auto getConstVal() noexcept
    {
        return std::get<T>(value);
    }

    template <IsValidASTValue T>
    auto getConstVal() const noexcept
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

    void setTyContext(Ptr<TypeContext> typeContext_)
    {
        tyContext = typeContext_;
    }

    auto getTyContext() { return tyContext; }

    auto getTyContext() const { return tyContext; }

    auto getSymType() const noexcept { return symType; }

    template <AllAreASTSymbolType... OtherTypes>
    bool isSymTypeOneOf(OtherTypes &&...othertypes) const noexcept
    {
        return isASTSymbolTypeOneOf(getSymType(),
                                    std::forward<OtherTypes>(othertypes)...);
    }

    auto getParent() noexcept { return parent; }

    auto getParent() const noexcept { return parent; }

    auto getChildrenNum() const noexcept { return children_.size(); }

    auto isChildrenEmpty() const noexcept { return children_.empty(); }

    auto &children() noexcept { return children_; }

    auto &children() const noexcept { return children_; }

    auto &location() noexcept { return loc; }

    auto &location() const noexcept { return loc; }

    void setContext(Ptr<ASTContext> astContext_) noexcept
    {
        context_ = astContext_;
    }

    auto getContext() noexcept { return context_; }

    auto getContext() const noexcept { return context_; }

    auto &getVariant() noexcept { return value; }

  protected:
    Ptr<TypeContext> tyContext;
    ASTID astID;
    ASTSymType symType;
    Type *varType;
    WeakPtrAST parent;
    std::vector<PtrAST> children_;
    Location loc;
    Ptr<ASTContext> context_;
    ASTValueType value;

    //===----------------------------------------------------------------------===//
    // Runtime Polymorphism
  public:
    ASTID getASTID() const noexcept { return astID; }

    bool isTransUnit() const noexcept { return getASTID() == ASTID::TransUnit; }

    bool isExternDeclList() const noexcept
    {
        return getASTID() == ASTID::ExternDeclList;
    }

    bool isExternDecl() const noexcept
    {
        return getASTID() == ASTID::ExternDecl;
    }

    bool isFuncDef() const noexcept { return getASTID() == ASTID::FuncDef; }

    bool isExpr() const noexcept { return getASTID() == ASTID::Expr; }

    bool isCompStmt() const noexcept { return getASTID() == ASTID::CompStmt; }

    bool isStmt() const noexcept { return getASTID() == ASTID::Stmt; }

    bool isDecl() const noexcept { return getASTID() == ASTID::Decl; }

    bool isDirDecl() const noexcept { return getASTID() == ASTID::DirDecl; }

    bool isDeclSpec() const noexcept { return getASTID() == ASTID::DeclSpec; }

    bool isInitDecltrList() const noexcept
    {
        return getASTID() == ASTID::InitDecltrList;
    }

    bool isInitDecltr() const noexcept
    {
        return getASTID() == ASTID::InitDecltr;
    }

    bool isDecltr() const noexcept { return getASTID() == ASTID::Decltr; }

    bool isAbsDecltr() const noexcept { return getASTID() == ASTID::AbsDecltr; }

    bool isInitializer() const noexcept
    {
        return getASTID() == ASTID::Initializer;
    }

    bool isInitializerList() const noexcept
    {
        return getASTID() == ASTID::InitializerList;
    }

    bool isDesignation() const noexcept
    {
        return getASTID() == ASTID::Designation;
    }

    bool isDesignatorList() const noexcept
    {
        return getASTID() == ASTID::DesignatorList;
    }

    //===----------------------------------------------------------------------===//
    // Helper Methods
  public:
    ///
    /// \brief Create a new node, and add all following children `PtrAST`
    /// to the list of its children.
    ///
    template <IsBaseAST ASTType, AllArePtrAST... Children>
    friend Ptr<ASTType> makeAST(ASTSymType type, const Location &loc,
                                Children &&...children);

    template <IsBaseAST ASTType, AllArePtrAST... Children>
    friend Ptr<ASTType> makeAST(Ptr<TypeContext> typeContext, ASTSymType type,
                                const Location &loc, Children &&...children);

    ///
    /// \brief Create a new node, and add all following children `PtrAST`
    /// to the list of its children.
    ///
    template <IsBaseAST ASTType, IsValidASTValue T, AllArePtrAST... Children>
    friend Ptr<ASTType> makeAST(ASTSymType type, const Location &loc, T &&value,
                                Children &&...children);

    template <IsBaseAST ASTType, IsValidASTValue T, AllArePtrAST... Children>
    friend Ptr<ASTType> makeAST(Ptr<TypeContext> typeContext, ASTSymType type,
                                const Location &loc, T &&value,
                                Children &&...children);

    template <AllArePtrAST... Children>
    static PtrAST make(ASTSymType type, const Location &loc,
                       Children &&...children)
    {
        return makeAST<AST>(type, loc, children...);
    }

    template <IsValidASTValue T, AllArePtrAST... Children>
    static PtrAST make(ASTSymType type, const Location &loc, T &&value,
                       Children &&...children)
    {
        return makeAST<AST>(type, loc, value, children...);
    }

    template <AllArePtrAST... Children>
    static PtrAST make(Ptr<TypeContext> typeContext, ASTSymType type,
                       const Location &loc, Children &&...children)
    {
        return makeAST<AST>(typeContext, type, loc, children...);
    }

    template <IsValidASTValue T, AllArePtrAST... Children>
    static PtrAST make(Ptr<TypeContext> typeContext, ASTSymType type,
                       const Location &loc, T &&value, Children &&...children)
    {
        return makeAST<AST>(typeContext, type, loc, value, children...);
    }

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

    ///
    /// Print information of this single node.
    ///
    friend std::ostream &operator<<(std::ostream &os, const AST &node);

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

template <IsBaseAST ASTType, AllArePtrAST... Children>
inline Ptr<ASTType> makeAST(ASTSymType type, const Location &loc,
                            Children &&...children)
{
    Ptr<ASTType> parentNode = makeSharedPtr<ASTType>(type, loc);
    parentNode->addChildren(std::forward<Children>(children)...);
    return parentNode;
}

template <IsBaseAST ASTType, AllArePtrAST... Children>
inline Ptr<ASTType> makeAST(Ptr<TypeContext> typeContext, ASTSymType type,
                            const Location &loc, Children &&...children)
{
    Ptr<ASTType> parentNode = makeSharedPtr<ASTType>(typeContext, type, loc);
    parentNode->addChildren(std::forward<Children>(children)...);
    return parentNode;
}

template <IsBaseAST ASTType, IsValidASTValue T, AllArePtrAST... Children>
inline Ptr<ASTType> makeAST(ASTSymType type, const Location &loc, T &&value,
                            Children &&...children)
{
    Ptr<ASTType> parentNode =
        makeSharedPtr<ASTType>(type, loc, std::forward<T>(value));
    parentNode->addChildren(std::forward<Children>(children)...);
    return parentNode;
}

template <IsBaseAST ASTType, IsValidASTValue T, AllArePtrAST... Children>
inline Ptr<ASTType> makeAST(Ptr<TypeContext> typeContext, ASTSymType type,
                            const Location &loc, T &&value,
                            Children &&...children)
{
    Ptr<ASTType> parentNode =
        makeSharedPtr<ASTType>(typeContext, type, loc, std::forward<T>(value));
    parentNode->addChildren(std::forward<Children>(children)...);
    return parentNode;
}

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

inline std::ostream &operator<<(std::ostream &os, const AST &node)
{
    using utils::logging::ControlSeq;

    // print node type
    os << ControlSeq::Bold << getASTSymbolColor(node.symType)
       << getASTSymbolName(node.symType) << ControlSeq::Reset;

    // TODO: print node address (allocated)

    // print node location
    os << " <" << ControlSeq::BrightYellow;
    if (auto cid = node.loc.begin.contextID;
        cid != Location::invalidContextID) {
        // TODO: revise this
        if (!astPrintMap.contains(cid)) {
            astPrintMap.insert(cid);
            os << node.loc;
        }
        else {
            os << node.loc.begin.line << "." << node.loc.begin.column << "-";
            os << node.loc.end.line << "." << node.loc.end.column;
        }
    }
    else {
        os << "<invalid sloc>";
    }
    os << ControlSeq::Reset << ">";

    // print node content
    // TODO: print symbol table

    // print node value
    if (node.hasConstVal()) {
        // template magic
        // TODO: add support for other types
        os << " ";
        os << getASTSymbolColor(node.symType);
        node.visitConstVal(overloaded{
            [&](const auto arg) {}, [&](ASTCharType arg) { os << arg; },
            [&](ASTSIntType arg) { os << arg; },
            [&](ASTUIntType arg) { os << arg; },
            [&](ASTFloatType arg) { os << arg; },
            [&](const ASTIDType &arg) { os << arg; }});
        os << ControlSeq::Reset;
    }

    return os;
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