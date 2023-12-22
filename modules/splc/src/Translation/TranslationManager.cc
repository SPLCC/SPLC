#include "AST/ASTCommons.hh"
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

SymbolEntry TranslationManager::getSymbol(std::string_view name_)
{
    // TODO: catch
    auto ent = tunit->astContextManager.getSymbol(name_);
    return ent;
}

SymbolEntry
TranslationManager::registerSymbol(SymbolEntry::EntrySummary summary_,
                                   std::string_view name_, Type *type_,
                                   bool defined_, const Location *location_,
                                   ASTValueType value_, Ptr<AST> body_)
{
    // TODO: catch
    auto ent = tunit->astContextManager.registerSymbol(
        summary_, name_, type_, defined_, location_, value_, body_);
    return ent;
}

Ptr<TranslationContext>
TranslationManager::pushTransFileContext(const Location *intrLoc_,
                                         std::string_view fileName_)
{
    // TODO: catch
    Ptr<TranslationContext> context =
        tunit->translationContextManager.pushContext(intrLoc_, fileName_);
    return context;
}

Ptr<TranslationContext>
TranslationManager::pushTransMacroVarContext(const Location *intrLoc_,
                                             std::string_view macroVarName_)
{
    // TODO: catch
    Ptr<TranslationContext> context =
        tunit->translationContextManager.pushMacroVarContext(intrLoc_,
                                                             macroVarName_);
    return context;
}

/// \brief Get macro var context from the context manager.
MacroVarConstEntry TranslationManager::getTransMacroVarContext(
    std::string_view macroVarName_) const
{
    // TODO: catch
    auto ent =
        tunit->translationContextManager.getMacroVarContext(macroVarName_);
    return ent;
}

/// \brief Register a macro variable definition.
Ptr<TranslationContext>
TranslationManager::registerTransMacroVarContext(const Location *regLocation,
                                                 std::string_view macroVarName_,
                                                 std::string_view content_)
{
    // TODO: catch
    auto context = tunit->translationContextManager.registerMacroVarContext(
        regLocation, macroVarName_, content_);
    return context;
}

/// \brief Unregister a macro variable definition.
Ptr<TranslationContext> TranslationManager::unregisterTransMacroVarContext(
    const Location *unRegLoc, std::string_view macroVarName_)
{
    // TODO: catch
    auto context = tunit->translationContextManager.unregisterMacroVarContext(
        unRegLoc, macroVarName_);
    return context;
}

Ptr<TranslationUnit> TranslationManager::getTransUnit() { return tunit; }

} // namespace splc