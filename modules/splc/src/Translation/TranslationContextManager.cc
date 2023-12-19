#include <sstream>

#include "Core/System.hh"
#include "Core/Utils/ControlSequence.hh"

#include "Translation/TranslationContextManager.hh"

namespace splc {

TranslationContextManager::TranslationContextManager() {}

Ptr<TranslationContext>
TranslationContextManager::pushContext(Location &intrLoc,
                                       std::string &fileName_)
{
    Ptr<std::istream> inputStream = createPtr<std::ifstream>(fileName_);
    if (!inputStream) {
        using ControlSeq = utils::logging::ControlSeq;
        SPLC_LOG_ERROR(nullptr) << ControlSeq::Bold << "no such file: '" << fileName_ << ControlSeq::Reset;
        return {};
    }
    Ptr<TranslationContext> context = createPtr<TranslationContext>(
        TranslationContextBufferType::File, fileName_, intrLoc, inputStream);
    contextStack.push_back(context);
    allContexts.push_back(context);
    return context;
}

Ptr<TranslationContext> TranslationContextManager::pushContext(
    Location &intrLoc, std::string &macroName_, std::string &content_)
{
    Ptr<std::istream> inputStream = createPtr<std::istringstream>(content_);
    Ptr<TranslationContext> context = createPtr<TranslationContext>(
        TranslationContextBufferType::MacroExpansion, macroName_, intrLoc, content_, inputStream);
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