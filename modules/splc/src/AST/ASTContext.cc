#include "AST/ASTContext.hh"

namespace splc {

ASTContext::ASTContext(ASTContextBufferType type_, const std::string &name_,
                 const Location &intrLocation_)
    : type{type_}, name{name_}, intrLocation{intrLocation_}
{
}

} // namespace splc