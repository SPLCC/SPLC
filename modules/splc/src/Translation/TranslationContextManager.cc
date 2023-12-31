#include <filesystem>
#include <sstream>
#include <string>
#include <utility>

#include "Core/System.hh"
#include "Core/Utils/ControlSequence.hh"

#include "Translation/TranslationBase.hh"
#include "Translation/TranslationContextManager.hh"

namespace splc {

TranslationContextManager::TranslationContextManager() : contextID{0} {}

Ptr<TranslationContext>
TranslationContextManager::pushContext(const Location *intrLocation,
                                       Ptr<TranslationContext> context)
{
    context->intrLocation =
        intrLocation == nullptr ? Location{} : *intrLocation;
    contextStack.push_back(context);
    return context;
}

Ptr<TranslationContext>
TranslationContextManager::pushContext(const Location *intrLoc,
                                       std::string_view fileName_)
{
    Ptr<std::istream> inputStream =
        makeSharedPtr<std::ifstream>(std::string{fileName_});
    if (!inputStream) {
        throw SemanticError{intrLoc, "no such file"};
    }

    TranslationContextIDType newID = contextID++;
    Ptr<TranslationContext> context = makeSharedPtr<TranslationContext>(
        newID, TranslationContextBufferType::File, fileName_,
        contextStack.empty() ? nullptr : contextStack.back(), intrLoc,
        inputStream);
    contextStack.push_back(context);
    allContexts.push_back(context);
    return context;
}

Ptr<TranslationContext>
TranslationContextManager::pushMacroVarContext(const Location *intrLoc,
                                               std::string_view macroVarName_)
{
    // TODO: FIX
    auto it = macroVarMap.find(macroVarName_);
    if (it == macroVarMap.end()) {
        throw SemanticError(intrLoc, "requesting undefined macro variable");
    }
    Ptr<TranslationContext> context = it->second.second;
    Ptr<std::istream> inputStream =
        makeSharedPtr<std::istringstream>(std::string{context->content});

    contextStack.push_back(context);
    return context;
}

Ptr<TranslationContext> TranslationContextManager::popContext()
{
    Ptr<TranslationContext> context = contextStack.back();
    contextStack.pop_back();
    return context;
}

bool TranslationContextManager::isContextExistInStack(
    TranslationContextBufferType type_,
    std::string_view contextName_) const noexcept
{
    for (auto &contextPtr : contextStack) {
        if (contextPtr->bufferType == type_ && contextPtr->name == contextName_)
            return true;
    }
    return false;
}

bool TranslationContextManager::isTransMacroVarPresent(
    std::string_view macroVarName_) const noexcept
{
    return (macroVarMap.find(macroVarName_) != macroVarMap.end());
}

/// \brief Get macro var context from the context manager.
MacroVarConstEntry TranslationContextManager::getMacroVarContext(
    std::string_view macroVarName_) const
{
    // If can be found
    if (auto it = macroVarMap.find(macroVarName_); it != macroVarMap.end()) {
        return it->second;
    }
    throw SemanticError{nullptr, "undefined variable"};
}

/// \brief Register a macro variable definition.
Ptr<TranslationContext> TranslationContextManager::registerMacroVarContext(
    const Location *regLocation, std::string_view macroVarName_,
    std::string_view content_)
{
    if (auto it = macroVarMap.find(macroVarName_); it != macroVarMap.end()) {
        throw SemanticError{&it->second.first, "redefining variable"};
    }

    TranslationContextIDType newID = contextID++;
    Ptr<TranslationContext> context = makeSharedPtr<TranslationContext>(
        newID, TranslationContextBufferType::MacroVarExpansion, macroVarName_,
        contextStack.empty() ? nullptr : contextStack.back(), regLocation,
        content_, nullptr);
    allContexts.push_back(context);
    macroVarMap.insert(
        {std::string{macroVarName_},
         {regLocation == nullptr ? Location{} : *regLocation, context}});
    return context;
}

/// \brief Unregister a macro variable definition.
Ptr<TranslationContext> TranslationContextManager::unregisterMacroVarContext(
    const Location *unRegLoc, std::string_view macroVarName_)
{
    auto it = macroVarMap.find(macroVarName_);
    if (it == macroVarMap.end()) {
        throw SemanticError{unRegLoc, "undefining non-existing macro variable"};
    }
    Ptr<TranslationContext> context = it->second.second;

    macroVarMap.erase(it);

    return context;
}

} // namespace splc