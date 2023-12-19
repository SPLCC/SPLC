#include "Translation/TranslationLogger.hh"

namespace splc {

TranslationLogger::TranslationLogger(const Ptr<const TranslationUnit> tunit_,
                                     const bool trace_, const Location *locPtr_,
                                     const Level level_)
    : Logger{true, level_, locPtr_}
{
    // TODO: allow debug trace
}

TranslationLogger::~TranslationLogger()
{
    // TODO
}

} // namespace splc