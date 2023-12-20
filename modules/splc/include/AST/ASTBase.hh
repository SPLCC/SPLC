#include <memory>
#include <utility>
#ifndef __SPLC_AST_ASTBASE_HH__
#define __SPLC_AST_ASTBASE_HH__ 1

#include <iostream>
#include <map>
#include <type_traits>
#include <variant>
#include <vector>

#include <AST/ASTToken.hh>
#include <Core/splc.hh>

namespace splc {

class AST;

template <class T>
concept IsASTType = (std::is_base_of_v<AST, std::remove_reference_t<T>>);

template <class T>
concept IsPtrAST = requires(T &&t)
{
    std::static_pointer_cast<AST>(std::forward<T>(t));
};

template <class... Children>
concept AllArePtrAST = (IsPtrAST<Children> && ...);

template <class T, class... Functors>
concept AllApplicableOnAST = IsASTType<T> &&
    (std::is_invocable_r_v<T &&, Functors, T &&> &&...);

template <typename T>
concept IsValidASTValue = (std::is_same_v<T, char> ||
                           std::is_same_v<T, unsigned long long> ||
                           std::is_same_v<T, double> ||
                           std::is_same_v<T, std::string>);

template <typename T>
auto castToPtrASTBase(T &&t)
{
    return std::dynamic_pointer_cast<AST>(std::forward(t));
}

// template <class Functor>
// concept IsValidASTValueVisitor =
//     (std::is_invocable_v<Functor, char> ||
//      std::is_invocable_v<Functor, unsigned long long> ||
//      std::is_invocable_v<Functor, double> ||
//      std::is_invocable_v<Functor, std::string> ||
//      std::is_invocable_v<Functor, const char> ||
//      std::is_invocable_v<Functor, const unsigned long long> ||
//      std::is_invocable_v<Functor, const double> ||
//      std::is_invocable_v<Functor, const std::string>);

/// \brief Class `AST` describes a single node in the Abstract Syntax Tree
/// (AST), and acts as the foundation of the parse tree.
///
/// `AST` accepts the following value types, if given:
/// - `Char`
/// - `Unsigned long long`
/// - `Double`
/// - `String`
class AST : public std::enable_shared_from_this<AST> {

  public:
    /// This constructor should be called by AST internal method
    AST() noexcept : type{ASTSymbolType::YYEMPTY} {}
    AST(const ASTSymbolType type_, const Location &loc_)
    noexcept : type{type_}, loc{loc_}
    {
    }

    template <class T>
    AST(const ASTSymbolType type_, const Location &loc_, T &&value_)
        : type{type_}, loc{loc_}, value{value_}
    {
    }

    virtual ~AST() = default;

    // TODO(IR): determine code generation
    // virtual void codeGen();

    static inline bool isASTAppendable(const AST &node)
    {
        return !isASTSymbolTypeOneOfThem(node.type, ASTSymbolType::YYEMPTY,
                                         ASTSymbolType::YYEOF,
                                         ASTSymbolType::YYerror);
    }

    void addChild(Ptr<AST> child)
    {
        children.push_back(child);
        child->parent = shared_from_this();
    }

    template <AllArePtrAST... Children>
    void addChildren(Children &&...children)
    {
        ((children && isASTAppendable(*std::static_pointer_cast<AST>(children))
              ? addChild(std::static_pointer_cast<AST>(
                    std::forward<Children>(children)))
              : void()),
         ...);
    }

    Ptr<AST> findFirstChild(ASTSymbolType type) const noexcept;

    constexpr bool hasValue() const noexcept
    {
        return !value.valueless_by_exception();
    }

    template <IsValidASTValue T>
    auto getValue()
    {
        return std::get<T>(value);
    }

    template <class Visitor>
    auto visitValue(Visitor &&vis) const
    {
        return std::visit(vis, value);
    }

    template <IsValidASTValue T>
    void emplaceValue(T &&val)
    {
        value.emplace<T>(std::forward(val));
    }

    template <IsValidASTValue T>
    constexpr bool holdsValueType() const noexcept
    {
        return std::holds_alternative<T>(value);
    }

    auto &getChildren() noexcept { return children; }

    auto &getLocation() noexcept { return loc; }

    auto &getVariant() noexcept { return value; }

  protected:
    ASTSymbolType type;
    WeakPtr<AST> parent;
    std::vector<Ptr<AST>> children;
    Location loc;
    // TODO: add symbol table
    std::variant<unsigned long long, double, std::string> value;

  public:
    /// \brief Create a new node, and add all following children `Ptr<AST>`
    /// to the list of its children.
    template <IsASTType ASTType, AllArePtrAST... Children>
    friend Ptr<ASTType> createAST(ASTSymbolType type, const Location &loc,
                                  Children &&...children);

    ///
    /// Allow stream-like operation on ASTs for processing.
    ///
    template <IsASTType T, class Functor>
    requires AllApplicableOnAST<T, Functor>
    friend T &&operator>>(T &&node, Functor &&functor);

    /// Apply all `functors` on the target `node`, returning the transformed
    /// version. Transforms are applied in a sequential manner as given in the
    /// parameters.
    template <IsASTType T, class... Functors>
    requires AllApplicableOnAST<T, Functors...>
    friend T &&applyASTTransform(T &&node, Functors &&...functors);

    /// Print information of this single node.
    friend std::ostream &operator<<(std::ostream &os, const AST &node);

    class ASTRecursivePrintManipulator {
      public:
        friend ASTRecursivePrintManipulator treePrintTransform(const AST &node);

        friend std::ostream &
        operator<<(std::ostream &os,
                   const AST::ASTRecursivePrintManipulator &m);

      private:
        ASTRecursivePrintManipulator(const AST &node_) : node{node_} {}
        const AST &node;
    };

    friend void recursivePrintNode(std::ostream &os, const AST &node,
                                   size_t depth);
}; // class: AST

template <IsASTType ASTType, AllArePtrAST... Children>
inline Ptr<ASTType> createAST(ASTSymbolType type, const Location &loc,
                              Children &&...children)
{
    Ptr<ASTType> parentNode = createPtr<ASTType>(type, loc);
    parentNode->addChildren(std::forward<Children>(children)...);
    return parentNode;
}

template <IsASTType T, class Functor>
requires AllApplicableOnAST<T, Functor>
inline T &&operator>>(T &&node, Functor &&functor)
{
    return std::forward<Functor>(functor)(std::forward<T>(node));
}

template <IsASTType T, class... Functors>
requires AllApplicableOnAST<T, Functors...>
inline T &&applyASTTransform(T &&node, Functors &&...functors)
{
    return (functors(std::forward<T>(node)), ...);
}

inline std::ostream &operator<<(std::ostream &os, const AST &node)
{
    using ControlSeq = utils::logging::ControlSeq;

    // print node type
    os << ControlSeq::Bold << getASTSymbolColor(node.type)
       << getASTSymbolName(node.type) << ControlSeq::Reset;

    // TODO: print node address (allocated)

    // print node location
    os << ControlSeq::BrightYellow << " <" << node.loc << ">"
       << ControlSeq::Reset;

    // print node content
    // TODO: print symbol table

    // print node value
    if (node.hasValue()) {
        // template magic
        // TODO: add support for other types
        os << " ";
        node.visitValue(overloaded{
            [&](const auto arg) { os << arg; },
            [&](char arg) { os << ControlSeq::BrightCyan << arg; },
            [&](const unsigned long long arg) {
                os << ControlSeq::Green << arg;
            },
            [&](const double arg) { os << ControlSeq::Green << arg; },
            [&](std::string_view arg) {
                os << ControlSeq::BrightCyan << arg;
            }});
        os << ControlSeq::Reset;
    }

    return os;
}

inline AST::ASTRecursivePrintManipulator treePrintTransform(const AST &node)
{
    return {node};
}

inline void recursivePrintNode(std::ostream &os, const AST &node, size_t depth)
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

inline std::ostream &operator<<(std::ostream &os,
                                const AST::ASTRecursivePrintManipulator &m)
{
    os << m.node << "\n";
    recursivePrintNode(os, m.node, 0);
    return os;
}

} // namespace splc

#endif // __SPLC_AST_ASTBASE_HH__