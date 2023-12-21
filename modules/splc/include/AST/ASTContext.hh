#include "Core/Base.hh"
#ifndef __SPLC_AST_ASTCONTEXT_HH__
#define __SPLC_AST_ASTCONTEXT_HH__ 1

#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "AST/ASTCommons.hh"
#include "AST/SymbolEntry.hh"

namespace splc {

/// \brief `ASTContext` describes declarations in a particular scope.
class ASTContext {
  public:
    ASTContext(ASTContextDepthType depth_) : depth{depth_} {}

    ASTSymbolMap &getSymbolMap() { return symbolMap; }
    
    const ASTSymbolMap &getSymbolMap() const { return symbolMap; }

  protected:
    ASTContextDepthType depth;
    ASTSymbolMap symbolMap;
    std::vector<WeakPtr<ASTContext>> parentContexts;

  public:
    friend class AST;
    friend class ASTProcessor;
    friend class ASTContextManager;
    friend class Type;
    friend class Value;
};

} // namespace splc

#endif // __SPLC_AST_ASTCONTEXT_HH__