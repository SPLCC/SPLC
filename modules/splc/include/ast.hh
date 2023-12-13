#ifndef __SPLC_AST_HH__
#define __SPLC_AST_HH__ 1

#include "ast_token.hh"

#include <core/splc.hh>
#include <iostream>
#include <map>
#include <vector>

namespace splc::ast {

class ASTNode {
  public:
    ASTNode() : type{TokenType::YYEMPTY} {}
    ASTNode(const TokenType type_, const location &loc_)
        : type{type_}, loc{loc_}
    {
    }

    template <class... Children>
    static Ptr<ASTNode> createParentNode(TokenType type, const location &loc,
                                         Children &...children);

    template <class... Children>
    static Ptr<ASTNode> createParentNode(TokenType type, const location &loc,
                                         Children &&...children);

    Ptr<ASTNode> getFirstChild(TokenType type);

    // TODO(AST): add symbol table
    friend std::ostream &operator<<(std::ostream &os, ASTNode &node);

    Ptr<ASTNode> getParent() { return parent; }
    std::vector<Ptr<ASTNode>> &getChildren() { return children; }
    location &getLocation() { return loc; }

  private:
    TokenType type;
    Ptr<ASTNode> parent;
    std::vector<Ptr<ASTNode>> children;
    location loc;
};

} // namespace splc::ast

#endif /* __SPLC_AST_HH__ */