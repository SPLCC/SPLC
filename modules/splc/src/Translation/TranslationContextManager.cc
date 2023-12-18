#include "Translation/TranslationContextManager.hh"

namespace splc {

TranslationContextManager::TranslationContextManager() {}

Ptr<TranslationContext>
TranslationContextManager::pushContext(Location &intrLoc,
                                       std::string &fileName_)
{
    Ptr<TranslationContext> context = createPtr<TranslationContext>(
        TranslationContextBufferType::File, fileName_, intrLoc);
    contextStack.push_back(context);
    allContexts.push_back(context);
    return context;
}

Ptr<TranslationContext> TranslationContextManager::pushContext(
    Location &intrLoc, std::string &macroName_, std::string &content_)
{
    Ptr<TranslationContext> context = createPtr<TranslationContext>(
        TranslationContextBufferType::File, macroName_, intrLoc, content_);
    contextStack.push_back(context);
    allContexts.push_back(context);
    return context;
}

Ptr<TranslationContext> TranslationContextManager::popContext()
{
    Ptr<TranslationContext> context = contextStack.back();
    contextStack.pop_back();
    return context;
}

bool TranslationContextManager::isContextExistInStack(
    TranslationContextBufferType type_, std::string_view contextName_)
{
    for (auto &contextPtr : contextStack) {
        if (contextPtr->type == type_ && contextPtr->name == contextName_)
            return true;
    }
    return false;
}

} // namespace splc