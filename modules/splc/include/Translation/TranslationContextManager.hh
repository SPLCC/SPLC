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

///
/// \brief This class handles context management during parsing.
/// In detail, this class holds context information related to file
/// inclusion and macro expansion.
///
class TranslationContextManager {
  public:
    TranslationContextManager();

    ~TranslationContextManager() = default;

    ///
    /// \brief Push `Ptr<TranslationContext` into context manager, switching to
    ///        this context.
    /// \warning This method will not add the given context to the context
    /// stack, as the user is assumed to call this method IFF they are
    /// revisiting existing contexts.
    /// \param intrLocation interrupt location
    ///
    Ptr<TranslationContext> pushContext(const Location *intrLocation,
                                        Ptr<TranslationContext> context);

    ///
    /// \brief Push a new file into context manager. If no such file exist,
    /// throw `Semantic Error`.
    /// \param intrLocation interrupt location
    ///
    Ptr<TranslationContext> pushContext(const Location *intrLocation,
                                        std::string_view fileName_);

    ///
    /// \brief Push a macro substitution into context manager, switching to
    /// macro substitution. If no such macro exist, throw `Semantic Error`.
    /// \param intrLocation interrupt location
    ///
    Ptr<TranslationContext> pushMacroVarContext(const Location *intrLocation,
                                                std::string_view macroVarName_);

    ///
    /// \brief Pop the topmost context.
    /// If there does not exist such context, or if all the contexts
    /// have already been popped off, throw `SemanticError`
    ///
    Ptr<TranslationContext> popContext();

    bool isTransMacroVarPresent(std::string_view macroVarName_) const noexcept;

    ///
    /// \brief Get macro var context from the context manager. If no such macro
    ///        exist, throw `Semantic Error`.
    ///
    MacroVarConstEntry getMacroVarContext(std::string_view macroVarName_) const;

    ///
    /// \brief Register a macro variable definition.
    ///
    Ptr<TranslationContext>
    registerMacroVarContext(const Location *regLocation,
                            std::string_view macroVarName_,
                            std::string_view content_);

    ///
    /// \brief Unregister a macro variable definition.
    ///
    Ptr<TranslationContext>
    unregisterMacroVarContext(const Location *unRegLoc,
                              std::string_view macroVarName_);

    MacroVarMap &getMacroMap() noexcept { return macroVarMap; }

    const MacroVarMap &getMacroMap() const noexcept { return macroVarMap; }

    bool isContextExistInStack(TranslationContextBufferType type_,
                               std::string_view contextName_) const noexcept;

    bool contextStackEmpty() const noexcept { return contextStack.empty(); }

    size_t contextStackSize() const noexcept { return contextStack.size(); }

    ///
    /// \brief Get the current (i.e., top) context
    /// If the top context does not exists, return `nullptr`.
    ///
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
        return contextStack.back()->getKey();
    }

    /// \brief Provide a convenient way to access stack elements
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
    TranslationContextIDType contextID; ///< Internal index

    std::vector<Ptr<TranslationContext>> allContexts; ///< Store all contexts

    std::vector<Ptr<TranslationContext>>
        contextStack; ///< Store contexts for checking repeated inclusions

    MacroVarMap macroVarMap; ///< Store macro variables that might be used.

  public:
    friend class TranslationUnit;
    friend class TranslationManager;
    friend class TranslationLogger;
};

} // namespace splc

#endif // __SPLC_TRANSLATION_TRANSLATIONCONTEXTMANAGER_HH__