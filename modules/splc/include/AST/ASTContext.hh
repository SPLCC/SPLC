#ifndef __SPLC_AST_ASTCONTEXT_HH__
#define __SPLC_AST_ASTCONTEXT_HH__ 1

#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "AST/ASTCommons.hh"

namespace splc {

/// \brief `ASTContext` describes declarations in a particular scope.
class ASTContext {
  public:
    ASTContext() = default;

  protected:
    
};

} // namespace splc

#endif // __SPLC_AST_ASTCONTEXT_HH__