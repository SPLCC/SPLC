#ifndef __SPLC_AST_HH__
#define __SPLC_AST_HH__ 1

#include <iostream>
#include <map>
#include <variant>
#include <vector>

#include <ast_token.hh>
#include <core/splc.hh>

namespace splc::ast {

/**
 * \brief Class `Node` describes a single node in the Abstract Syntax Tree
 * (AST), and acts as the foundation of the parse tree.
 *
 * `Node` accepts the following value types, if given
 */
class Node {

  public:
    /**
     * This constructor should be called by AST internal method
     */
    Node() noexcept : type{SymbolType::YYEMPTY} {}
    Node(const SymbolType type_, const Location &loc_) noexcept
        : type{type_}, loc{loc_}
    {
    }

    template <class T>
    Node(const SymbolType type_, const Location &loc_, T &&value_)
        : type{type_}, loc{loc_}, value{value_}
    {
    }

    Ptr<Node> findFirstChild(SymbolType type) noexcept;

    std::vector<Ptr<Node>> &getChildren() noexcept { return children; }
    Location &getLocation() noexcept { return loc; }

    auto &getVariant() noexcept { return value; }

    inline constexpr bool hasValue() const noexcept
    {
        return value.valueless_by_exception();
    }
    
    template <class T>
    auto getValue()
    {
        return std::get<T>(value);
    }

    template <class T>
    inline void emplaceValue(T &&val)
    {
        value.emplace<T>(std::forward(val));
    }

    template <class T>
    constexpr bool holdsValueType() const noexcept
    {
        return std::holds_alternative<T>(value);
    }

  private:
    SymbolType type;
    std::vector<Ptr<Node>> children;
    Location loc;
    // TODO: add symbol table
    std::variant<unsigned long long, double, std::string> value;

  public:
    template <class... Children>
    friend Ptr<Node> createNode(SymbolType type, const Location &loc,
                                Children &&...children);
    /**
     * Print information of this single node.
     */
    friend std::ostream &operator<<(std::ostream &os, Node &node);

    class ASTNodeRecursiveManipulator {
      public:
        friend ASTNodeRecursiveManipulator treePrint(Ptr<Node> node);
        Ptr<Node> node;

      private:
        ASTNodeRecursiveManipulator(Ptr<Node> node_) : node{node_} {}
    };
};

/**
 * \brief Create a new node, and add all following children `Ptr<Node>` to the
 * list of its children.
 */
template <class... Children>
inline Ptr<Node> createNode(SymbolType type, const Location &loc,
                            Children &&...children)
{
    Ptr<Node> parentNode = createPtr<Node>(type, loc);
    (parentNode->children.push_back(std::forward<Children>(children)), ...);
    return parentNode;
}

inline std::ostream &operator<<(std::ostream &os, Node &node)
{
    // TODO:
    return os;
}

inline Node::ASTNodeRecursiveManipulator treePrint(Ptr<Node> node)
{
    return {node};
}

static inline std::ostream &operator<<(std::ostream &os,
                                       Node::ASTNodeRecursiveManipulator &m)
{
    using ControlSeq = utils::logging::ControlSeq;

    // TODO:
    os << ControlSeq::Bold;

    return os;
}

} // namespace splc::ast

#endif /* __SPLC_AST_HH__ */