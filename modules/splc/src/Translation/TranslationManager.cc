#include "Translation/TranslationManager.hh"
#include "AST/ASTCommons.hh"
#include "AST/DerivedTypes.hh"
#include "AST/SymbolEntry.hh"

using namespace splc;

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
    SymEntryType symEntTy, std::string_view name_, Type *type_, bool defined_,
    const Location *location_, ASTValueType value_, PtrAST body_)
{
    auto ent = tunit->astCtxtMgr.registerSymbol(
        symEntTy, name_, type_, defined_, location_, value_, body_);
    return ent;
}

void TranslationManager::tryRegisterSymbol(PtrAST root)
{
    std::vector<Type *> rootTysFetched = root->getType();
    std::vector<ASTDeclEntityType> ids = root->getNamedDeclEntities();
    std::vector<Type *> rootTys;

    for (Type *ty : rootTysFetched) {
        rootTys.push_back(ty);
        if (ty->isFunctionTy()) {
            std::copy(ty->subtype_begin() + 1, ty->subtype_end(),
                      std::back_inserter(rootTys));
        }
    }

    splc_dbgassert(rootTys.size() == ids.size());

    size_t numChild = ids.size();

    for (size_t i = 0; i < numChild; ++i) {
        Type *ty = rootTys[i];
        ASTDeclEntityType &idLoc = ids[i];
        auto [declID, declLoc, declVal] = idLoc;

        SymEntryType symEntTy;
        bool defined = false;
        PtrAST body;

        if (ty->isFunctionTy()) {
            symEntTy = SymEntryType::Function;
            if (auto it = std::find_if(
                    root->getChildren().begin(), root->getChildren().end(),
                    [](const PtrAST &node) {
                        return node->getSymbolType() == ASTSymbolType::CompStmt;
                    });
                it != root->getChildren().end()) {
                defined = true;
                body = *it;
            }
        }
        else if (ty->isFirstClassType()) {
            symEntTy = SymEntryType::Variable;
        }
        else {
            splc_error() << "unknown type to be registered: " << *ty;
        }
        try {
            // TODO: revise
            auto ent = registerSymbol(symEntTy, declID, ty, defined, &declLoc,
                                      declVal, body);
            SPLC_LOG_DEBUG(&declLoc, false)
                << "registered identifier " << declID << ", type: " << *ty;
        }
        catch (SemanticError e) {
            SPLC_LOG_ERROR(&declLoc, true) << e.what();
            SPLC_LOG_NOTE(&e.loc, false) << "previously defined here";
        }
    }
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
