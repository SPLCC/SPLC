#ifndef __SPLC_AST_AST_HH__
#define __SPLC_AST_AST_HH__ 1

#include "Core/splc.hh"

#include "AST/ASTBase.hh"

namespace splc {

class ASTHelper {
  public:
    /// Transform Ptr<AST> containing Ptr, such that
    /// The pointers form a rightmost tree hierarchy.
    /// At the rightmost child of the tree shall the declarator lie.
    static Ptr<AST> getPtrDeclEndPoint(Ptr<AST> ptrRoot) noexcept;
};

class ExprAST : public AST {
};

} // namespace splc

#endif // __SPLC_AST_AST_HH__