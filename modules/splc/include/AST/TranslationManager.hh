#ifndef __SPLC_AST_TRANSLATIONMANAGER_HH__
#define __SPLC_AST_TRANSLATIONMANAGER_HH__ 1

#include <vector>
#include <string>
#include <string_view>

#include "Core/splc.hh"

#include "AST/TranslationUnit.hh"

namespace splc {

class TranslationManager {
  public:
    TranslationManager() = default;

    void startTranslationRecord();

    void endTranslationRecord();

    // TODO
    void pushEnvironment();

    // TODO
    void popEnvironment();

    // TODO
    void pushContext();

    // TODO
    void popContext();

    // TODO
    Ptr<TranslationUnit> getTranslationUnit();

  private:
    Ptr<TranslationUnit> tunit;
};

} // namespace splc::module

#endif // __SPLC_AST_TRANSLATIONMANAGER_HH__