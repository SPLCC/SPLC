#include "AST/ASTCommons.hh"
#include "Core/Base.hh"
#include "Core/System.hh"

#include "Core/Utils/Logging.hh"
#include "Translation/TranslationManager.hh"

namespace splc {

void TranslationManager::startTranslationRecord()
{
    tunit = makeSharedPtr<TranslationUnit>();
}

void TranslationManager::endTranslationRecord() {}

void TranslationManager::reset() { tunit.reset(); }

SymbolEntry TranslationManager::getSymbol(SymEntryType symEntTy,
                                          std::string_view name_)
{
    auto ent = tunit->astCtxtMgr.getSymbol(symEntTy, name_);
    return ent;
}

SymbolEntry TranslationManager::registerSymbol(
    SymEntryType summary_, std::string_view name_, Type *type_, bool defined_,
    const Location *location_, ASTValueType value_, PtrAST body_)
{
    auto ent = tunit->astCtxtMgr.registerSymbol(
        summary_, name_, type_, defined_, location_, value_, body_);
    return ent;
}

Ptr<TranslationContext>
TranslationManager::pushTransFileContext(const Location *intrLoc_,
                                         std::string_view fileName_)
{
    Ptr<TranslationContext> context =
        tunit->transCtxtMgr.pushContext(intrLoc_, fileName_);
    return context;
}

Ptr<TranslationContext>
TranslationManager::pushTransMacroVarContext(const Location *intrLoc_,
                                             std::string_view macroVarName_)
{
    Ptr<TranslationContext> context =
        tunit->transCtxtMgr.pushMacroVarContext(intrLoc_, macroVarName_);
    return context;
}

bool TranslationManager::isContextExistInStack(
    TranslationContextBufferType type_,
    std::string_view contextName_) const noexcept
{
    return tunit->transCtxtMgr.isContextExistInStack(type_, contextName_);
}

/// \brief Get macro var context from the context manager.
MacroVarConstEntry TranslationManager::getTransMacroVarContext(
    std::string_view macroVarName_) const
{
    auto ent = tunit->transCtxtMgr.getMacroVarContext(macroVarName_);
    return ent;
}

/// \brief Register a macro variable definition.
Ptr<TranslationContext>
TranslationManager::registerTransMacroVarContext(const Location *regLocation,
                                                 std::string_view macroVarName_,
                                                 std::string_view content_)
{
    auto context = tunit->transCtxtMgr.registerMacroVarContext(
        regLocation, macroVarName_, content_);
    return context;
}

/// \brief Unregister a macro variable definition.
Ptr<TranslationContext> TranslationManager::unregisterTransMacroVarContext(
    const Location *unRegLoc, std::string_view macroVarName_)
{
    auto context =
        tunit->transCtxtMgr.unregisterMacroVarContext(unRegLoc, macroVarName_);
    return context;
}

Ptr<TranslationUnit> TranslationManager::getTransUnit() { return tunit; }

} // namespace splc