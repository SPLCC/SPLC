#ifndef __SPLC_AST_CONTEXT_HH__
#define __SPLC_AST_CONTEXT_HH__ 1

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

namespace splc {

enum ASTContextBufferType {
    File,
    MacroExpansion,
};

struct ASTContext {

    ASTContext() = delete;

    ASTContext(ASTContextBufferType type_, const std::string &name_,
               const Location &intrLocation_)
        : type{type_}, name{name_}, intrLocation{intrLocation_}
    {
    }

    ASTContext(ASTContextBufferType type_, const std::string &name_,
               const Location &intrLocation_, const std::string &content_)
        : type{type_}, name{name_},
          intrLocation(intrLocation_), content{content_}
    {
    }

    const ASTContextBufferType type;
    const std::string name;
    const Location intrLocation; // Interrupt Location
    const std::string content;
};

} // namespace splc

#endif // __SPLC_AST_CONTEXT_HH__