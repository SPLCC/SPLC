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
TranslationManager::pushTranslationContext(const Location *intrLoc_,
                                           std::string_view fileName_)
{
    Ptr<TranslationContext> context =
        tunit->translationContextManager.pushContext(intrLoc_, fileName_);
    return context;
}

Ptr<TranslationContext>
TranslationManager::pushTranslationContext(const Location *intrLoc_,
                                           std::string_view macroName_,
                                           std::string_view content_)
{
    Ptr<TranslationContext> context =
        tunit->translationContextManager.pushContext(intrLoc_, macroName_,
                                                     content_);
    // yy_buffer_state *newState =
    // scanner->yy_create_buffer(context->inputStream.get(), SPLC_BUF_SIZE);
    // scanner->yypush_buffer_state(newState);
    return context;
}

Ptr<TranslationContext> TranslationManager::popTranslationContext()
{
    Ptr<TranslationContext> context =
        tunit->translationContextManager.popContext();
    // scanner->yypop_buffer_state();
    return context;
}

Ptr<TranslationUnit> TranslationManager::getTranslationUnit() { return tunit; }

} // namespace splc