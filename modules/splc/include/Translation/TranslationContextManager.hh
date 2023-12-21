#include "IO/Parser.hh"
#ifndef __SPLC_TRANSLATION_TRANSLATIONCONTEXTMANAGER_HH__
#define __SPLC_TRANSLATION_TRANSLATIONCONTEXTMANAGER_HH__ 1

#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "Translation/TranslationBase.hh"
#include "Translation/TranslationContext.hh"

namespace splc {

/// \brief This class handles context management during parsing.
/// In detail, this class holds context information related to file
/// inclusion and macro expansion.
class TranslationContextManager {
  public:
    TranslationContextManager();

    ~TranslationContextManager() = default;

    // TODO: allow create and retrieve macro contexts

    /// \brief Push `Ptr<TranslationContext` into context manager, switching to
    ///        this context.
    /// \warning This method will not add the given context to the context
    /// stack, as the user is assumed to call this method IFF they are
    /// revisiting existing contexts.
    /// \param intrLocation interrupt location
    Ptr<TranslationContext> pushContext(Ptr<TranslationContext> context);

    /// \brief Push a new file into context manager. If no such file exist,
    /// throw a runtime error.
    /// \param intrLocation interrupt location
    Ptr<TranslationContext> pushContext(const Location *intrLocation,
                                        std::string_view fileName_);

    /// \brief Push a macro substitution into context manager, switching to
    /// macro substitution.
    /// \param intrLocation interrupt location
    Ptr<TranslationContext> pushContext(const Location *intrLocation,
                                        std::string_view macroName_,
                                        std::string_view ontent_);

    /// \brief Pop the topmost context.
    /// If there does not exist such context, or if all the contexts
    /// have already been popped off, return 1. Else, return 0.
    Ptr<TranslationContext> popContext();

    bool isContextExistInStack(TranslationContextBufferType type_,
                               std::string_view contextName_) const;

    bool contextStackEmpty() const { return contextStack.empty(); }

    size_t contextStackSize() const { return contextStack.size(); }

    /// \brief Get the current (i.e., top) context
    /// If the top context does not exists, return `nullptr`.
    Ptr<TranslationContext> getCurrentContext()
    {
        if (contextStackEmpty()) {
            return {};
        }
        else {
            return contextStack.back();
        }
    }

    TranslationContextKeyType getCurrentContextKey() const noexcept
    {
        auto key = contextStack.back()->getKey();
        return key;
    }

    /// Provide a convenient way to access stack elements
    Ptr<TranslationContext> operator[](size_t idx)
    {
        return contextStack[contextStack.size() - idx - 1];
    }

    Ptr<const TranslationContext> operator[](size_t idx) const
    {
        return contextStack[contextStack.size() - idx - 1];
    }

    std::vector<Ptr<TranslationContext>> &getContextStack()
    {
        return contextStack;
    }

    bool isAllContextsEmpty() const { return allContexts.empty(); }

    size_t allContextsSize() const { return allContexts.size(); }

    std::vector<Ptr<TranslationContext>> &getAllContexts()
    {
        return allContexts;
    }

  protected:
    /// Internal index
    TranslationContextIDType contextID;

    /// Store all contexts
    std::vector<Ptr<TranslationContext>> allContexts;

    /// Store contexts for checking repeated inclusions
    std::vector<Ptr<TranslationContext>> contextStack;

    /// Store macro variables that might be used.
    MacroVarMap macroVarMap;

  public:
    friend class TranslationUnit;
    friend class TranslationManager;
    friend class TranslationLogger;
};

} // namespace splc

#endif // __SPLC_TRANSLATION_TRANSLATIONCONTEXTMANAGER_HH__