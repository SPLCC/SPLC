#ifndef __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__
#define __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__ 1

#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "Translation/TranslationBase.hh"
#include "Translation/TranslationUnit.hh"

namespace splc {

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
    void pushTranslationContext();

    // TODO
    void popTranslationContext();

    // TODO
    Ptr<TranslationUnit> getTranslationUnit();

  private:
    Ptr<TranslationUnit> tunit;
};

} // namespace splc

#endif // __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__