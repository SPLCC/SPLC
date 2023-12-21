#include "Core/Base.hh"
#include "Core/System.hh"

#include "IO/Parser.hh"

#include "Translation/TranslationManager.hh"

namespace splc {

void TranslationManager::startTranslationRecord()
{
    tunit = makeSharedPtr<TranslationUnit>();
}

void TranslationManager::endTranslationRecord() {}

void TranslationManager::reset() { tunit.reset(); }

void TranslationManager::pushASTContext()
{
    // TODO
}

void TranslationManager::popASTContext()
{
    // TODO
}

void TranslationManager::getCurrentASTContext()
{
    // TODO
}

Ptr<TranslationContext>
TranslationManager::pushTransFileContext(const Location *intrLoc_,
                                         std::string_view fileName_)
{
    Ptr<TranslationContext> context =
        tunit->translationContextManager.pushContext(intrLoc_, fileName_);
    return context;
}

Ptr<TranslationContext>
TranslationManager::pushTransMacroVarContext(const Location *intrLoc_,
                                             std::string_view macroVarName_)
{
    Ptr<TranslationContext> context =
        tunit->translationContextManager.pushMacroVarContext(intrLoc_,
                                                             macroVarName_);
    return context;
}

Ptr<TranslationContext> TranslationManager::popTransContext()
{
    Ptr<TranslationContext> context =
        tunit->translationContextManager.popContext();
    // scanner->yypop_buffer_state();
    return context;
}

bool TranslationManager::isTransMacroVarPresent(
    std::string_view macroVarName_) const
{
    return tunit->translationContextManager.isTransMacroVarPresent(
        macroVarName_);
}

/// \brief Get macro var context from the context manager.
MacroVarConstEntry TranslationManager::getTransMacroVarContext(
    std::string_view macroVarName_) const
{
    // TODO: catch
    return tunit->translationContextManager.getMacroVarContext(macroVarName_);
}

/// \brief Register a macro variable definition.
Ptr<TranslationContext>
TranslationManager::registerTransMacroVarContext(const Location *regLocation,
                                                 std::string_view macroVarName_,
                                                 std::string_view content_)
{
    // TODO: catch
    return tunit->translationContextManager.registerMacroVarContext(
        regLocation, macroVarName_, content_);
}

/// \brief Unregister a macro variable definition.
Ptr<TranslationContext> TranslationManager::unregisterTransMacroVarContext(
    const Location *unRegLoc, std::string_view macroVarName_)
{
    // TODO: catch
    return tunit->translationContextManager.unregisterMacroVarContext(
        unRegLoc, macroVarName_);
}

Ptr<TranslationUnit> TranslationManager::getTransUnit() { return tunit; }

} // namespace splc