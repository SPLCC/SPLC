#ifndef __SPLC_CONTEXT_HH__
#define __SPLC_CONTEXT_HH__ 1

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "core/splc.hh"

namespace splc::context {

enum BufferType {
    File,
    MacroExpansion,
};

class Context {
  public:
    Context();

  private:
    BufferType type;
    std::string name;
    Location intrLocation; // Interrupt Location
};

/** \brief This class handles context management during parsing.
 * In detail, this class holds context information related to file inclusion
 * and macro expansion.
 *
 */
class ContextManager {
  public:
    ContextManager();

    /**
     * \brief Push a new file into context manager. If no such file exist, throw
     * a runtime error.
    */
    int pushContext(std::string &fileName_);

    /** Push a macro substitution into context manager, switching to macro
     * substitution. */
    int pushContext(std::string &macroName, std::string &content);

    /** 
     * \brief Pop the topmost context.
     * If there does not exist such context, or if all the contexts
     * have already been popped off, return 1. Else, return 0.
     */
    int popContext();

    void isBufferExist(BufferType type, std::string_view contextName_);

    void reset();

  private:
    std::stack<Ptr<Context>> contextStack;
    std::vector<Ptr<Context>> existingContext;

    int warningCount;
    int errorCount;
};

} // namespace splc::context

#endif /* __SPLC_CONTEXT_HH__ */