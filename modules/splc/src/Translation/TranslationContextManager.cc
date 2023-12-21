#include <filesystem>
#include <sstream>

#include "Core/System.hh"
#include "Core/Utils/ControlSequence.hh"

#include "Translation/TranslationContextManager.hh"

namespace splc {

TranslationContextManager::TranslationContextManager() : contextID{0} {}

Ptr<TranslationContext>
TranslationContextManager::pushContext(Ptr<TranslationContext> context)
{
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
        using ControlSeq = utils::logging::ControlSeq;
        SPLC_LOG_ERROR(nullptr) << ControlSeq::Bold << "no such file: '"
                                << fileName_ << ControlSeq::Reset;
        return {};
    }

    Ptr<TranslationContext> context = makeSharedPtr<TranslationContext>(
        contextID++, TranslationContextBufferType::File, fileName_,
        contextStack.empty() ? nullptr : contextStack.back(), intrLoc,
        inputStream);
    contextStack.push_back(context);
    allContexts.push_back(context);
    return context;
}

Ptr<TranslationContext>
TranslationContextManager::pushContext(const Location *intrLoc,
                                       std::string_view macroName_,
                                       std::string_view content_)
{
    Ptr<std::istream> inputStream =
        makeSharedPtr<std::istringstream>(std::string{content_});

    Ptr<TranslationContext> context = makeSharedPtr<TranslationContext>(
        contextID++, TranslationContextBufferType::MacroExpansion, macroName_,
        contextStack.empty() ? nullptr : contextStack.back(), intrLoc, content_,
        inputStream);
    contextStack.push_back(context);
    allContexts.push_back(context);
    return context;
}

Ptr<TranslationContext> TranslationContextManager::popContext()
{
    SPLC_ASSERT(!contextStack.empty()) << "stack is empty.";
    Ptr<TranslationContext> context = contextStack.back();
    contextStack.pop_back();
    return context;
}

bool TranslationContextManager::isContextExistInStack(
    TranslationContextBufferType type_, std::string_view contextName_) const
{
    for (auto &contextPtr : contextStack) {
        if (contextPtr->type == type_ && contextPtr->name == contextName_)
            return true;
    }
    return false;
}

} // namespace splc