#include "ast.hh"

namespace splc::ast {

template <class... Children>
Ptr<ASTNode> ASTNode::createParentNode(TokenType type, const Location &loc,
                                       Children &...children)
{
    Ptr<ASTNode> parentNode = createPtr<ASTNode>(type, loc);
    (parentNode->children.push_back(children), ...);
    return parentNode;
}

template <class... Children>
Ptr<ASTNode> ASTNode::createParentNode(TokenType type, const Location &loc,
                                       Children &&...children)
{
    Ptr<ASTNode> parentNode = createPtr<ASTNode>(type, loc);
    (parentNode->children.push_back(std::forward<Children>(children)), ...);
    return parentNode;
}

} // namespace splc::ast