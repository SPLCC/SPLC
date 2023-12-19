#ifndef __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__
#define __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__ 1

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "Translation/TranslationBase.hh"
#include "Translation/TranslationUnit.hh"

namespace splc {

/// \brief class `TranslationManager` is designed to decouple the tight cohesion
/// between the driver class and the scanner class. It provides basic facilities
/// such as context switching, error reporting, and stuff.
class TranslationManager {
  public:
    TranslationManager() = default;

    // TODO
    void startTranslationRecord();

    // TODO
    void endTranslationRecord();

    void reset();

    // TODO
    void getCurrentASTContext();

    // TODO
    void pushASTContext();

    // TODO
    void popASTContext();

    Ptr<TranslationContext> getCurrentTranslationContext();

    const std::string &getCurrentTranslationContextName();

    bool translationContextStackEmpty() const
    {
        return tunit->translationContextManager.contextStackEmpty();
    }

    size_t translationContextStackSize() const
    {
        return tunit->translationContextManager.contextStackSize();
    }

    /// \brief Push a new translation context into the stack.
    /// \param intrLoc The location where context switch occurred. Note that
    /// this location
    ///                accepts `nullptr` for representing non-existing
    ///                locations. Internally, a copy is maintained, so that the
    ///                original location will not be affected.
    Ptr<TranslationContext> pushTranslationContext(const Location *intrLoc_,
                                                   std::string_view fileName_);

    /// \brief Push a new translation context into the stack.
    /// \param intrLoc The location where context switch occurred. Note that
    /// this location
    ///                accepts `nullptr` for representing non-existing
    ///                locations. Internally, a copy is maintained, so that the
    ///                original location will not be affected.
    Ptr<TranslationContext> pushTranslationContext(const Location *intrLoc_,
                                                   std::string_view macroName_,
                                                   std::string_view content_);

    Ptr<TranslationContext> popTranslationContext();

    Ptr<TranslationUnit> getTranslationUnit();

  protected:
    Ptr<TranslationUnit> tunit;
};

} // namespace splc

#define SPLC_TRANS_LOG_DEBUG(translationManager_, locPtr_)

#endif // __SPLC_TRANSLATION_TRANSLATIONMANAGER_HH__