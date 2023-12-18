#ifndef __SPLC_AST_CONTEXT_MANAGER_HH__
#define __SPLC_AST_CONTEXT_MANAGER_HH__ 1

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "AST/ASTContext.hh"

namespace splc {

/// \brief This class handles context management during parsing.
/// In detail, this class holds context information related to file inclusion
/// and macro expansion.
class ASTContextManager {
  public:
    ASTContextManager();

    /// \brief Push a new file into context manager. If no such file exist,
    /// throw a runtime error. \param intrLocation interrupt location
    Ptr<ASTContext> pushContext(Location &intrLocation, std::string &fileName_);

    /// \brief Push a macro substitution into context manager, switching to
    /// macro substitution. \param intrLocation interrupt location
    Ptr<ASTContext> pushContext(Location &intrLocation, std::string &macroName_,
                                std::string &content_);

    /// Pop the topmost context.
    /// If there does not exist such context, or if all the contexts
    /// have already been popped off, return 1. Else, return 0.
    Ptr<ASTContext> popContext();

    bool isContextExistInStack(ASTContextBufferType type_,
                               std::string_view contextName_);

    inline auto &getContextStack() { return contextStack; }

    inline auto &getAllContexts() { return allContexts; }

    /// Provide a convenient way to access stack elements
    inline Ptr<ASTContext> operator[](size_t idx)
    {
        return contextStack[contextStack.size() - idx - 1];
    }

    inline Ptr<const ASTContext> operator[](size_t idx) const
    {
        return contextStack[contextStack.size() - idx - 1];
    }

  private:
    std::vector<Ptr<ASTContext>> contextStack; /// This will store macro
                                               /// contexts for checking
                                               /// repeated definitions
    std::vector<Ptr<ASTContext>> allContexts;  /// Store all definitions
};

} // namespace splc

#endif // __SPLC_AST_CONTEXT_MANAGER_HH__