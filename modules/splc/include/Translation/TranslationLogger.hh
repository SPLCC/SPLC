#ifndef __SPLC_TRANSLATION_TRANSLATIONLOGGER_HH__
#define __SPLC_TRANSLATION_TRANSLATIONLOGGER_HH__ 1

#include "Translation/TranslationBase.hh"

#include "Translation/TranslationUnit.hh"

namespace splc {

class TranslationLogger : public splc::utils::logging::internal::Logger {
  public:
    using Level = splc::utils::logging::Level;

    TranslationLogger(const Ptr<const TranslationUnit> tunit_,
                      const bool trace_, const Location *locPtr_,
                      const Level level_);

    ~TranslationLogger();

  private:
    Ptr<const TranslationUnit> tunit;
};

} // namespace splc

#endif // __SPLC_TRANSLATION_TRANSLATIONLOGGER_HH__