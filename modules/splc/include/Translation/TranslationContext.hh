#ifndef __SPLC_AST_ASTCONTEXT_HH__
#define __SPLC_AST_ASTCONTEXT_HH__ 1

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

namespace splc {

enum TranslationContextBufferType {
    File,
    MacroExpansion,
};

struct TranslationContext {

    TranslationContext() = delete;

    TranslationContext(TranslationContextBufferType type_,
                       const std::string &name_, const Location &intrLocation_)
        : type{type_}, name{name_}, intrLocation{intrLocation_}
    {
    }

    TranslationContext(TranslationContextBufferType type_,
                       const std::string &name_, const Location &intrLocation_,
                       const std::string &content_)
        : type{type_}, name{name_},
          intrLocation(intrLocation_), content{content_}
    {
    }

    const TranslationContextBufferType type;
    const std::string name;
    const Location intrLocation; // Interrupt Location
    const std::string content;
};

} // namespace splc

#endif // __SPLC_AST_ASTCONTEXT_HH__