#ifndef __SPLC_TRANSLATION_TRANSLATIONLOGGER_HH__
#define __SPLC_TRANSLATION_TRANSLATIONLOGGER_HH__ 1

#include "Translation/TranslationBase.hh"

#include "Translation/TranslationUnit.hh"

namespace splc {

class TranslationLogger : public splc::utils::logging::internal::Logger {
  public:
    using Level = splc::utils::logging::Level;

    TranslationLogger(const TranslationUnit &tunit_, const bool trace_,
                      const Location *locPtr_, const Level level_)
        : tunit{tunit_}, trace{trace_}, Logger{true, level_, locPtr_} {};

    TranslationLogger(TranslationLogger &other) = delete;
    TranslationLogger(TranslationLogger &&other)
        : tunit{other.tunit}, Logger{false, other.level, other.locPtr}
    {
        other.setEnabled(false);
    }

    ~TranslationLogger();

    virtual void printInitial();

  protected:
    void printContextStack(const TranslationContext *context,
                           size_t depth) const;

    bool trace;
    const TranslationUnit &tunit;
};

} // namespace splc

#define SPLC_TRLOG_EMPTY(tunit, locPtr_)                                       \
    TranslationLogger                                                          \
    {                                                                          \
        tunit, false, locPtr_, splc::utils::logging::Level::Empty              \
    }

#define SPLC_TRLOG_DEBUG(tunit, locPtr_)                                       \
    TranslationLogger                                                          \
    {                                                                          \
        tunit, false, locPtr_, splc::utils::logging::Level::Debug              \
    }

#define SPLC_TRLOG_INFO(tunit, locPtr_)                                        \
    TranslationLogger                                                          \
    {                                                                          \
        tunit, false, locPtr_, splc::utils::logging::Level::Info               \
    }

#define SPLC_TRLOG_NOTE(tunit, locPtr_)                                        \
    TranslationLogger                                                          \
    {                                                                          \
        tunit, false, locPtr_, splc::utils::logging::Level::Note               \
    }

#define SPLC_TRLOG_WARNING(tunit, locPtr_)                                     \
    TranslationLogger                                                          \
    {                                                                          \
        tunit, true, locPtr_, splc::utils::logging::Level::Warning             \
    }

#define SPLC_TRLOG_ERROR(tunit, locPtr_)                                       \
    TranslationLogger                                                          \
    {                                                                          \
        tunit, true, locPtr_, splc::utils::logging::Level::Error               \
    }

#define SPLC_TRLOG_SYNTAX_ERROR(tunit, locPtr_)                                \
    TranslationLogger                                                          \
    {                                                                          \
        tunit, true, locPtr_, splc::utils::logging::Level::SyntaxError         \
    }

#define SPLC_TRLOG_SEMANTIC_ERROR(tunit, locPtr_)                              \
    TranslationLogger                                                          \
    {                                                                          \
        tunit, true, locPtr_, splc::utils::logging::Level::SemanticError       \
    }

#define SPLC_TRLOG_FATAL_ERROR(tunit, locPtr_)                                 \
    TranslationLogger                                                          \
    {                                                                          \
        tunit, true, locPtr_, splc::utils::logging::Level::FatalError          \
    }

#endif // __SPLC_TRANSLATION_TRANSLATIONLOGGER_HH__