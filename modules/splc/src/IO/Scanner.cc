#include "IO/Scanner.hh"

// *This file constitute only part of the definition of class
// `splc::IO::Scanner`. *The remaining implementation comes from `IO/lexer.ll".

namespace splc::IO {

int splc::IO::Scanner::yywrap()
{
    if (translationManager.translationContextStackSize()) {
        Ptr<TranslationContext> context =
            translationManager.popTranslationContext();
    }
    return translationManager.translationContextStackEmpty();
}

} // namespace splc::IO