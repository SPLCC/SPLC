#include "Core/Base.hh"
#ifndef __SPLC_AST_ASTBASE_HH__
#define __SPLC_AST_ASTBASE_HH__ 1

#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <variant>
#include <vector>

#include <Core/splc.hh>

#include <AST/ASTCommons.hh>
#include <AST/TypeContext.hh>
#include <AST/Value.hh>

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

  public:
    ///
    /// \brief This constructor should be called by AST internal method
    ///
    explicit AST() noexcept : symbolType{ASTSymbolType::YYEMPTY} {}

    AST(const ASTSymbolType symbolType, const Location &loc_) noexcept
        : symbolType{symbolType}, loc{loc_}
    {
    }

    AST(Ptr<TypeContext> typeContext_, const ASTSymbolType symbolType,
        const Location &loc_) noexcept
        : typeContext{typeContext_}, symbolType{symbolType}, loc{loc_}
    {
    }

    template <IsValidASTValue T>
    AST(const ASTSymbolType symbolType, const Location &loc_,
        T &&value_) noexcept
        : symbolType{symbolType}, loc{loc_}, value{value_}
    {
    }

    template <IsValidASTValue T>
    AST(Ptr<TypeContext> typeContext_, const ASTSymbolType symbolType,
        const Location &loc_, T &&value_) noexcept
        : typeContext{typeContext_}, symbolType{symbolType}, loc{loc_},
          value{value_}
    {
    }

    virtual ~AST() = default;

    virtual AST &operator=(const AST &other) = default;

    virtual AST &operator=(AST &&other) = default;

    ///
    /// \deprecated This function serves no particular purpose.
    /// \brief Acquire a deep copy of the current Ptr<AST> method.
    /// By default, all children are copied.
    ///
    virtual Ptr<AST> copy(
        const std::function<bool(Ptr<const AST>)> &predicate =
            [](Ptr<const AST>) { return true; },
        const bool copyContext = true) const;

    virtual Type *getType() const;

    // TODO(sem): semantic analysis generation
    virtual Value evaluate();

    static inline bool isASTAppendable(const AST &node) noexcept
    {
        return !isASTSymbolTypeOneOfThem(
            node.symbolType, ASTSymbolType::YYEMPTY, ASTSymbolType::YYEOF,
            ASTSymbolType::YYerror);
    }

    void addChild(Ptr<AST> child)
    {
        children.push_back(child);
        child->parent = shared_from_this();
        child->typeContext = this->typeContext;
    }

    template <AllArePtrAST... Children>
    void addChildren(Children &&...children)
    {
        ((children && isASTAppendable(*castToPtrASTBase(children))
              ? addChild(castToPtrASTBase(std::forward<Children>(children)))
              : void()),
         ...);
    }

    Ptr<AST> findFirstChild(ASTSymbolType type) const noexcept;

    constexpr bool hasValue() const noexcept
    {
        return !value.valueless_by_exception();
    }

    template <IsValidASTValue T>
    auto getValue() noexcept
    {
        return std::get<T>(value);
    }

    template <class Visitor>
    auto visitValue(Visitor &&vis) const noexcept
    {
        return std::visit(vis, value);
    }

    template <IsValidASTValue T>
    void emplaceValue(T &&val) noexcept
    {
        value.emplace<T>(std::forward(val));
    }

    template <IsValidASTValue T>
    constexpr bool holdsValueType() const noexcept
    {
        return std::holds_alternative<T>(value);
    }

    void setTypeContext(Ptr<TypeContext> typeContext_)
    {
        typeContext = typeContext_;
    }

    auto getTypeContext() { return typeContext; }

    auto getSymbolType() const noexcept { return symbolType; }

    auto getParent() noexcept { return parent; }

    auto &getChildren() noexcept { return children; }

    auto &getLocation() noexcept { return loc; }

    void setASTContext(Ptr<ASTContext> astContext_) noexcept
    {
        astContext = astContext_;
    }

    auto getASTContext() noexcept { return astContext; }

    auto &getVariant() noexcept { return value; }

  protected:
    Ptr<TypeContext> typeContext;
    ASTSymbolType symbolType;
    WeakPtr<AST> parent;
    std::vector<Ptr<AST>> children;
    Location loc;
    Ptr<ASTContext> astContext;
    ASTValueType value;

  public:
    ///
    /// \brief Create a new node, and add all following children `Ptr<AST>`
    /// to the list of its children.
    ///
    template <IsBaseAST ASTType, AllArePtrAST... Children>
    friend Ptr<ASTType> makeAST(ASTSymbolType type, const Location &loc,
                                Children &&...children);

    template <IsBaseAST ASTType, AllArePtrAST... Children>
    friend Ptr<ASTType> makeAST(Ptr<TypeContext> typeContext,
                                ASTSymbolType type, const Location &loc,
                                Children &&...children);

    ///
    /// \brief Create a new node, and add all following children `Ptr<AST>`
    /// to the list of its children.
    ///
    template <IsBaseAST ASTType, IsValidASTValue T, AllArePtrAST... Children>
    friend Ptr<ASTType> makeAST(ASTSymbolType type, const Location &loc,
                                T &&value, Children &&...children);

    template <IsBaseAST ASTType, IsValidASTValue T, AllArePtrAST... Children>
    friend Ptr<ASTType> makeAST(Ptr<TypeContext> typeContext,
                                ASTSymbolType type, const Location &loc,
                                T &&value, Children &&...children);

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
                                   size_t depth) noexcept;

    friend class ASTProcessor;
    friend class ASTContext;
    friend class ASTContextManager;
    friend class Type;
    friend class Value;
}; // class: AST

template <IsBaseAST ASTType, AllArePtrAST... Children>
inline Ptr<ASTType> makeAST(ASTSymbolType type, const Location &loc,
                            Children &&...children)
{
    Ptr<ASTType> parentNode = makeSharedPtr<ASTType>(type, loc);
    parentNode->addChildren(std::forward<Children>(children)...);
    return parentNode;
}

template <IsBaseAST ASTType, AllArePtrAST... Children>
inline Ptr<ASTType> makeAST(Ptr<TypeContext> typeContext, ASTSymbolType type,
                            const Location &loc, Children &&...children)
{
    Ptr<ASTType> parentNode = makeSharedPtr<ASTType>(typeContext, type, loc);
    parentNode->addChildren(std::forward<Children>(children)...);
    return parentNode;
}

template <IsBaseAST ASTType, IsValidASTValue T, AllArePtrAST... Children>
inline Ptr<ASTType> makeAST(ASTSymbolType type, const Location &loc, T &&value,
                            Children &&...children)
{
    Ptr<ASTType> parentNode =
        makeSharedPtr<ASTType>(type, loc, std::forward<T>(value));
    parentNode->addChildren(std::forward<Children>(children)...);
    return parentNode;
}

template <IsBaseAST ASTType, IsValidASTValue T, AllArePtrAST... Children>
inline Ptr<ASTType> makeAST(Ptr<TypeContext> typeContext, ASTSymbolType type,
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
    using ControlSeq = utils::logging::ControlSeq;

    // print node type
    os << ControlSeq::Bold << getASTSymbolColor(node.symbolType)
       << getASTSymbolName(node.symbolType) << ControlSeq::Reset;

    // TODO: print node address (allocated)

    // print node location
    os << " <" << ControlSeq::BrightYellow;
    if (node.loc.begin.contextName) {
        // TODO: print contextname if appeared for the first time
        os << node.loc;
    }
    else {
        os << "<invalid sloc>";
    }
    os << ControlSeq::Reset << ">";

    // print node content
    // TODO: print symbol table

    // print node value
    if (node.hasValue()) {
        // template magic
        // TODO: add support for other types
        os << " ";
        node.visitValue(overloaded{
            [&](const auto arg) { os << arg; },
            [&](ASTCharType arg) { os << ControlSeq::Green << arg; },
            [&](ASTSignedIntegralType arg) { os << ControlSeq::Blue << arg; },
            [&](ASTUnsignedIntegralType arg) { os << ControlSeq::Blue << arg; },
            [&](ASTFloatType arg) { os << ControlSeq::Green << arg; },
            [&](const ASTIDType &arg) { os << ControlSeq::Green << arg; }});
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
                               size_t depth) noexcept
{
    using namespace std::string_view_literals;
    using ControlSeq = utils::logging::ControlSeq;

    constexpr std::string_view treeMidArrow = "|-"sv;
    constexpr std::string_view midSegment = "| "sv;
    constexpr std::string_view treeEndArrow = "`-"sv;
    constexpr std::string_view noSegment = "  "sv;

    for (auto child : node.children) {
        os << ControlSeq::Blue;
        if (child == node.children.back()) {
            for (size_t i = 0; i < depth; ++i)
                os << noSegment;
            os << treeEndArrow;
        }
        else {
            for (size_t i = 0; i < depth; ++i)
                os << midSegment;
            os << treeMidArrow;
        }
        os << ControlSeq::Reset;

        os << *child << "\n";

        if (!child->children.empty()) {
            recursivePrintNode(os, *child, depth + 1);
        }
    }
}

inline std::ostream &
operator<<(std::ostream &os,
           const AST::ASTRecursivePrintManipulator &m) noexcept
{
    os << m.node << "\n";
    recursivePrintNode(os, m.node, 0);
    return os;
}

} // namespace splc

#endif // __SPLC_AST_ASTBASE_HH__