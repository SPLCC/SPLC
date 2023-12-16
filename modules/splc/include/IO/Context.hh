#ifndef __SPLC_IO_CONTEXT_HH__
#define __SPLC_IO_CONTEXT_HH__ 1

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "AST/ASTNode.hh"

namespace splc::IO {

enum BufferType {
    File,
    MacroExpansion,
};

class Context {
  public:
    Context() = delete;
    Context(BufferType type_, const std::string &name_,
            const Location &intrLocation);

    const auto &getType() { return type; }
    std::string_view getName() { return {name}; }
    const auto &getIntrLocation() { return intrLocation; }
    std::string_view getContent() { return {content}; }

  private:
    const BufferType type;
    const std::string name;
    const Location intrLocation; // Interrupt Location
    const std::string content;
};

/** This class handles context management during parsing.
 * In detail, this class holds context information related to file inclusion
 * and macro expansion.
 *
 */
class ContextManager {
  public:
    ContextManager();

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

    bool isContextExist(BufferType type_, std::string_view contextName_);

    auto &getTopContext() { return *contextStack.rbegin(); }

    auto &getContextStack() { return contextStack; }

    auto &getAllFileContext() { return allFileContexts; }

  private:
    std::vector<Ptr<Context>> contextStack;    // This will store macro contexts
                                               // for checking
    std::vector<Ptr<Context>> allFileContexts; // Ideally, no macro substitution
                                               // would be stored in there

    int warningCount;
    int errorCount;
};

} // namespace splc::IO

#endif /* __SPLC_IO_CONTEXT_HH__ */