#include <cstddef>
#ifndef __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__
#define __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__ 1

#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "Translation/TranslationBase.hh"
#include "Translation/TranslationUnit.hh"

namespace splc {

/// \brief class `TranslationManager` is designed to decouple the tight cohesion between 
/// the driver class and the scanner class. It provides basic facilities such as
/// context switching, error reporting, and stuff.
class TranslationManager {
  public:
    TranslationManager() = default;

    // TODO
    void startTranslationRecord();

    // TODO
    void endTranslationRecord();

    // TODO
    void pushASTContext();

    // TODO
    void popASTContext();

    // TODO
    void getCurrentASTContext();

    bool translationContextStackEmpty() const
    {
        return tunit->translationContextManager.contextStackEmpty();
    }

    size_t translationContextStackSize() const
    {
        return tunit->translationContextManager.contextStackSize();
    }

    Ptr<TranslationContext>
    pushTranslationContext(const Location &intrLoc_,
                           const std::string &fileName_);

    Ptr<TranslationContext>
    pushTranslationContext(const Location &intrLoc_,
                           const std::string &macroName_,
                           const std::string &content_);

    Ptr<TranslationContext> popTranslationContext();

    Ptr<TranslationUnit> getTranslationUnit();

    Ptr<TranslationContext> getCurrentTranslationContext();

  protected:
    Ptr<TranslationUnit> tunit;
};

} // namespace splc

#endif // __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__