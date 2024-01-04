#include "Translation/TranslationManager.hh"
#include "AST/ASTCommons.hh"
#include "AST/SymbolEntry.hh"
#include "Basic/DerivedTypes.hh"

using namespace splc;

using CS = splc::utils::logging::ControlSeq;

void TranslationManager::startTranslationRecord()
{
    tunit = makeSharedPtr<TranslationUnit>();
}

void TranslationManager::endTranslationRecord() {}

void TranslationManager::reset() { tunit.reset(); }

SymbolEntry TranslationManager::getSymbol(SymEntryType symEntTy,
                                          std::string_view name_)
{
    auto ent = tunit->astCtxMgr.getSymbol(symEntTy, name_);
    return ent;
}

SymbolEntry TranslationManager::registerSymbol(SymEntryType symEntTy,
                                               std::string_view name_,
                                               Type *type_, bool defined_,
                                               const Location *location_,
                                               PtrAST body_)
{
    auto ent = tunit->astCtxMgr.registerSymbol(symEntTy, name_, type_, defined_,
                                               location_, body_);
    return ent;
}

void TranslationManager::tryRegisterSymbol(SymEntryType symEntTy,
                                           std::string_view name_, Type *type_,
                                           bool defined_,
                                           const Location *location_,
                                           PtrAST body_)
{
    try {
        // TODO: revise
        auto ent =
            registerSymbol(symEntTy, name_, type_, defined_, location_, body_);
        SPLC_LOG_DEBUG(location_, false) << "registered identifier " << name_;
        if (type_ != nullptr) {
            SPLC_LOG_DEBUG(location_, false)
                << "the type of the identifier is: " << *type_;
        }
        else {
            SPLC_LOG_DEBUG(location_, false)
                << "the type of the identifier is: " << CS::BrightRed
                << "undefined" << CS::Reset;
        }
    }
    catch (SemanticError e) {
        SPLC_LOG_ERROR(location_, true) << e.what();
        SPLC_LOG_NOTE(&e.loc, false) << "previously defined here";
    }
}

Ptr<TranslationContext>
TranslationManager::pushTransFileContext(const Location *intrLoc_,
                                         std::string_view fileName_)
{
    Ptr<TranslationContext> context =
        tunit->transCtxMgr.pushContext(intrLoc_, fileName_);
    return context;
}

Ptr<TranslationContext>
TranslationManager::pushTransMacroVarContext(const Location *intrLoc_,
                                             std::string_view macroVarName_)
{
    Ptr<TranslationContext> context =
        tunit->transCtxMgr.pushMacroVarContext(intrLoc_, macroVarName_);
    return context;
}

bool TranslationManager::isContextExistInStack(
    TranslationContextBufferType type_,
    std::string_view contextName_) const noexcept
{
    return tunit->transCtxMgr.isContextExistInStack(type_, contextName_);
}

/// \brief Get macro var context from the context manager.
MacroVarConstEntry TranslationManager::getTransMacroVarContext(
    std::string_view macroVarName_) const
{
    auto ent = tunit->transCtxMgr.getMacroVarContext(macroVarName_);
    return ent;
}

/// \brief Register a macro variable definition.
Ptr<TranslationContext>
TranslationManager::registerTransMacroVarContext(const Location *regLocation,
                                                 std::string_view macroVarName_,
                                                 std::string_view content_)
{
    auto context = tunit->transCtxMgr.registerMacroVarContext(
        regLocation, macroVarName_, content_);
    return context;
}

/// \brief Unregister a macro variable definition.
Ptr<TranslationContext> TranslationManager::unregisterTransMacroVarContext(
    const Location *unRegLoc, std::string_view macroVarName_)
{
    auto context =
        tunit->transCtxMgr.unregisterMacroVarContext(unRegLoc, macroVarName_);
    return context;
}

Ptr<TranslationUnit> TranslationManager::getTransUnit() const noexcept
{
    return tunit;
}
