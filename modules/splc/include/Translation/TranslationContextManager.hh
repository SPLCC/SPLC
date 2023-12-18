#ifndef __SPLC_TRANSLATION_TRANSLATIONCONTEXTMANAGER_HH__
#define __SPLC_TRANSLATION_TRANSLATIONCONTEXTMANAGER_HH__ 1

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "Translation/TranslationBase.hh"
#include "Translation/TranslationContext.hh"

namespace splc {

/// \brief This class handles context management during parsing.
/// In detail, this class holds context information related to file inclusion
/// and macro expansion.
class TranslationContextManager {
  public:
    TranslationContextManager();

    /// \brief Push a new file into context manager. If no such file exist,
    /// throw a runtime error.
    /// \param intrLocation interrupt location
    Ptr<TranslationContext> pushContext(Location &intrLocation,
                                        std::string &fileName_);

    /// \brief Push a macro substitution into context manager, switching to
    /// macro substitution.
    /// \param intrLocation interrupt location
    Ptr<TranslationContext> pushContext(Location &intrLocation,
                                        std::string &macroName_,
                                        std::string &content_);

    /// \brief Pop the topmost context.
    /// If there does not exist such context, or if all the contexts
    /// have already been popped off, return 1. Else, return 0.
    Ptr<TranslationContext> popContext();

    bool isContextExistInStack(TranslationContextBufferType type_,
                               std::string_view contextName_);

    auto &getContextStack() { return contextStack; }

    auto &getAllContexts() { return allContexts; }

    /// Provide a convenient way to access stack elements
    Ptr<TranslationContext> operator[](size_t idx)
    {
        return contextStack[contextStack.size() - idx - 1];
    }

    Ptr<const TranslationContext> operator[](size_t idx) const
    {
        return contextStack[contextStack.size() - idx - 1];
    }

  private:
    /// This will store macro contexts for checking repeated definitions
    std::vector<Ptr<TranslationContext>> contextStack;

    /// Store all contexts
    std::vector<Ptr<TranslationContext>> allContexts;
};

} // namespace splc

#endif // __SPLC_TRANSLATION_TRANSLATIONCONTEXTMANAGER_HH__