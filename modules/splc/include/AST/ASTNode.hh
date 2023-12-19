#include <type_traits>
#ifndef __SPLC_AST_ASTNODE_HH__
#define __SPLC_AST_ASTNODE_HH__ 1

#include <iostream>
#include <map>
#include <variant>
#include <vector>

#include <AST/ASTToken.hh>
#include <Core/splc.hh>

namespace splc {

class ASTNode;

template <class T>
concept IsASTNode = (std::is_same_v<ASTNode, std::remove_cvref_t<T>>);

template <class... Children>
concept AllArePtrASTNode =
    (std::is_same_v<Ptr<ASTNode>,
                    typename std::remove_reference<Children>::type> &&
     ...);

template <class T, class... Functors>
concept AllApplicableOnASTNode = IsASTNode<T> &&
    (std::is_invocable_r_v<T &&, Functors, T &&> &&...);

template <typename T>
concept IsValidASTNodeValue = (std::is_same_v<T, char> ||
                               std::is_same_v<T, unsigned long long> ||
                               std::is_same_v<T, double> ||
                               std::is_same_v<T, std::string>);

// template <class Functor>
// concept IsValidASTNodeValueVisitor =
//     (std::is_invocable_v<Functor, char> ||
//      std::is_invocable_v<Functor, unsigned long long> ||
//      std::is_invocable_v<Functor, double> ||
//      std::is_invocable_v<Functor, std::string> ||
//      std::is_invocable_v<Functor, const char> ||
//      std::is_invocable_v<Functor, const unsigned long long> ||
//      std::is_invocable_v<Functor, const double> ||
//      std::is_invocable_v<Functor, const std::string>);

/// \brief Class `ASTNode` describes a single node in the Abstract Syntax Tree
/// (AST), and acts as the foundation of the parse tree.
///
/// `ASTNode` accepts the following value types, if given:
/// - `Char`
/// - `Unsigned long long`
/// - `Double`
/// - `String`
class ASTNode {

  public:
    /// This constructor should be called by AST internal method
    ASTNode() noexcept : type{ASTSymbolType::YYEMPTY} {}
    ASTNode(const ASTSymbolType type_, const Location &loc_) noexcept
        : type{type_}, loc{loc_}
    {
    }

    template <class T>
    ASTNode(const ASTSymbolType type_, const Location &loc_, T &&value_)
        : type{type_}, loc{loc_}, value{value_}
    {
    }

    Ptr<ASTNode> findFirstChild(ASTSymbolType type) const noexcept;

    constexpr bool hasValue() const noexcept
    {
        return value.valueless_by_exception();
    }

    template <class T>
    requires IsValidASTNodeValue<T>
    auto getValue() { return std::get<T>(value); }

    template <class Visitor>
    auto visitValue(Visitor &&vis) const
    {
        return std::visit(vis, value);
    }

    template <class T>
    requires IsValidASTNodeValue<T>
    void emplaceValue(T &&val) { value.emplace<T>(std::forward(val)); }

    template <class T>
    requires IsValidASTNodeValue<T>
    constexpr bool holdsValueType() const noexcept
    {
        return std::holds_alternative<T>(value);
    }

    auto &getChildren() noexcept { return children; }

    auto &getLocation() noexcept { return loc; }

    auto &getVariant() noexcept { return value; }

  protected:
    ASTSymbolType type;
    std::vector<Ptr<ASTNode>> children;
    Location loc;
    // TODO: add symbol table
    std::variant<unsigned long long, double, std::string> value;

  public:
    /// Check if an ASTNode is appendable.
    friend bool isASTNodeAppendable(const Ptr<const ASTNode> &node);

    /// \brief Create a new node, and add all following children `Ptr<ASTNode>`
    /// to the list of its children.
    template <class... Children>
    requires AllArePtrASTNode<Children...>
    friend Ptr<ASTNode> createASTNode(ASTSymbolType type, const Location &loc,
                                      Children &&...children);
    ///
    /// Allow stream-like operation on ASTs for processing.
    ///
    template <class T, class Functor>
    requires IsASTNode<T> && AllApplicableOnASTNode<T, Functor>
    friend T &&operator>>(T &&node, Functor &&functor);

    /// Apply all `functors` on the target `node`, returning the transformed
    /// version. Transforms are applied in a sequential manner as given in the
    /// parameters.
    template <class T, class... Functors>
    requires IsASTNode<T> && AllApplicableOnASTNode<T, Functors...>
    friend ASTNode &applyASTTransform(ASTNode &node, Functors &&...functors);

    /// Print information of this single node.
    friend std::ostream &operator<<(std::ostream &os, const ASTNode &node);

    class ASTNodeRecursivePrintManipulator {
      public:
        friend ASTNodeRecursivePrintManipulator
        treePrintTransform(const ASTNode &node);

        friend std::ostream &
        operator<<(std::ostream &os,
                   const ASTNode::ASTNodeRecursivePrintManipulator &m);

      private:
        ASTNodeRecursivePrintManipulator(const ASTNode &node_) : node{node_} {}
        const ASTNode &node;
    };

    friend void recursivePrintNode(std::ostream &os, const ASTNode &node,
                                   size_t depth);
}; // class: ASTNode

inline bool isASTNodeAppendable(const Ptr<const ASTNode> &node)
{
    return node != nullptr && !isASTSymbolTypeOneOfThem(
                                  node->type, ASTSymbolType::YYEMPTY,
                                  ASTSymbolType::YYEOF, ASTSymbolType::YYerror);
}

template <class... Children>
requires AllArePtrASTNode<Children...>
inline Ptr<ASTNode> createASTNode(ASTSymbolType type, const Location &loc,
                                  Children &&...children)
{
    Ptr<ASTNode> parentNode = createPtr<ASTNode>(type, loc);
    ((isASTNodeAppendable(children)
          ? parentNode->children.push_back(std::forward<Children>(children))
          : void()),
     ...);
    return parentNode;
}

template <class T, class Functor>
requires IsASTNode<T> && AllApplicableOnASTNode<T, Functor>
inline T &&operator>>(T &&node, Functor &&functor)
{
    return std::forward<Functor>(functor)(std::forward<T>(node));
}

template <class T, class... Functors>
requires IsASTNode<T> && AllApplicableOnASTNode<T, Functors...>
inline T &&applyASTTransform(T &&node, Functors &&...functors)
{
    return (functors(std::forward<T>(node)), ...);
}

inline std::ostream &operator<<(std::ostream &os, const ASTNode &node)
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

inline ASTNode::ASTNodeRecursivePrintManipulator
treePrintTransform(const ASTNode &node)
{
    return {node};
}

inline void recursivePrintNode(std::ostream &os, const ASTNode &node,
                               size_t depth)
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
operator<<(std::ostream &os, const ASTNode::ASTNodeRecursivePrintManipulator &m)
{
    os << m.node << "\n";
    recursivePrintNode(os, m.node, 0);
    return os;
}

} // namespace splc

#endif // __SPLC_AST_ASTNODE_HH__