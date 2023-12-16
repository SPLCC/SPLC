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

/** This class handles context management during parsing.
 * In detail, this class holds context information related to file inclusion
 * and macro expansion.
 *
 */
class ASTContextManager {
  public:
    ASTContextManager();

    /**
     * Push a new file into context manager. If no such file exist, throw
     * a runtime error.
     */
    int pushContext(std::string &fileName_);

    /** Push a macro substitution into context manager, switching to macro
     * substitution. */
    int pushContext(std::string &macroName_, std::string &content_);

    /**
     * Pop the topmost context.
     * If there does not exist such context, or if all the contexts
     * have already been popped off, return 1. Else, return 0.
     */
    int popContext();

    bool isContextExist(ASTContextBufferType type_, std::string_view contextName_);

    auto &getTopContext() { return *contextStack.rbegin(); }

    auto &getContextStack() { return contextStack; }

    auto &getAllFileContext() { return allFileContexts; }

  private:
    std::vector<Ptr<ASTContext>> contextStack;    // This will store macro contexts
                                               // for checking
    std::vector<Ptr<ASTContext>> allFileContexts; // Ideally, no macro substitution
                                               // would be stored in there
};

} // namespace splc

#endif /* __SPLC_AST_CONTEXT_MANAGER_HH__ */