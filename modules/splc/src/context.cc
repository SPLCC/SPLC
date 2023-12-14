#include "context.hh"

namespace splc::context {

Context::Context(BufferType type_, const std::string &name_,
                 const Location &intrLocation_)
    : type{type_}, name{name_}, intrLocation{intrLocation_}
{
}

ContextManager::ContextManager() : warningCount{0}, errorCount{0} {}

} // namespace splc::context