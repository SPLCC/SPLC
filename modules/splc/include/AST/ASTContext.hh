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

class ASTContext {
  public:
    ASTContext() = delete;
    ASTContext(ASTContextBufferType type_, const std::string &name_,
            const Location &intrLocation);

    const auto &getType() const { return type; }
    std::string_view getName() const { return {name}; }
    const auto &getIntrLocation() const { return intrLocation; }
    std::string_view getContent() const { return {content}; }

  private:
    const ASTContextBufferType type;
    const std::string name;
    const Location intrLocation; // Interrupt Location
    const std::string content;
};

} // namespace splc

#endif /* __SPLC_AST_CONTEXT_HH__ */