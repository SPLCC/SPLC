#include "IO/Scanner.hh"
#include "Translation/TranslationContext.hh"

// *This file constitute only part of the definition of class
// `splc::IO::Scanner`. *The remaining implementation comes from `IO/lexer.ll".

namespace splc::IO {

int Scanner::yywrap()
{
    if (!translationManager.transContextStackEmpty()) {
        Ptr<TranslationContext> context = translationManager.popTransContext();
    }

    return translationManager.transContextStackEmpty();
}

void Scanner::setInitialContext(Ptr<TranslationContext> initialContext)
{
    if (initialContext)
        pushInternalBuffer(initialContext);
}

/// \brief Push file context and switch to the included file.
void Scanner::pushFileContext(const Location *intrLoc_,
                              std::string_view fileName_)
{
    Ptr<TranslationContext> context =
        translationManager.pushTransFileContext(intrLoc_, fileName_);

    if (context)
        pushInternalBuffer(context);
}

/// \brief Push macro context and switch to macro substitution.
void Scanner::pushMacroVarContext(const Location *intrLoc_,
                                  std::string_view macroVarName_)
{

    Ptr<TranslationContext> context =
        translationManager.pushTransMacroVarContext(intrLoc_, macroVarName_);

    if (context)
        pushInternalBuffer(context);
}

bool Scanner::isMacroVarContextPresent(std::string_view macroVarName_) const
{
    return translationManager.isTransMacroVarPresent(macroVarName_);
}

void Scanner::registerMacroVarContext(const Location *regLocation,
                                      std::string_view macroVarName_,
                                      std::string_view content_)
{
    translationManager.registerTransMacroVarContext(regLocation, macroVarName_,
                                                    content_);
}

void Scanner::unregisterMacroVarContext(const Location *unRegLoc,
                                        std::string_view macroVarName_)
{
    translationManager.unregisterTransMacroVarContext(unRegLoc, macroVarName_);
}

void Scanner::pushInternalBuffer(Ptr<TranslationContext> context)
{
    yy_buffer_state *state =
        yy_create_buffer(context->inputStream.get(), SPLC_BUF_SIZE);
    yypush_buffer_state(state);
}

} // namespace splc::IO