#include "IO/Scanner.hh"
#include "Translation/TranslationContext.hh"

// *This file constitute only part of the definition of class
// `splc::IO::Scanner`. *The remaining implementation comes from `IO/lexer.ll".

namespace splc::IO {

int Scanner::yywrap()
{
    if (!transMgr.transContextStackEmpty()) {
        Ptr<TranslationContext> context = transMgr.popTransContext();
    }

    return transMgr.transContextStackEmpty();
}

void Scanner::setInitialContext(Ptr<TranslationContext> initialContext)
{
    if (initialContext)
        pushInternalBuffer(initialContext);
}

void Scanner::pushFileContext(const Location *intrLoc_,
                              std::string_view fileName_)
{
    Ptr<TranslationContext> context =
        transMgr.pushTransFileContext(intrLoc_, fileName_);

    if (context)
        pushInternalBuffer(context);
}

void Scanner::pushMacroVarContext(const Location *intrLoc_,
                                  std::string_view macroVarName_)
{

    Ptr<TranslationContext> context =
        transMgr.pushTransMacroVarContext(intrLoc_, macroVarName_);

    if (context)
        pushInternalBuffer(context);
}

bool Scanner::isMacroVarContextPresent(std::string_view macroVarName_) const
{
    return transMgr.isTransMacroVarPresent(macroVarName_);
}

void Scanner::registerMacroVarContext(const Location *regLocation,
                                      std::string_view macroVarName_,
                                      std::string_view content_)
{
    transMgr.registerTransMacroVarContext(regLocation, macroVarName_,
                                                    content_);
}

void Scanner::unregisterMacroVarContext(const Location *unRegLoc,
                                        std::string_view macroVarName_)
{
    transMgr.unregisterTransMacroVarContext(unRegLoc, macroVarName_);
}

void Scanner::pushInternalBuffer(Ptr<TranslationContext> context)
{
    yy_buffer_state *state =
        yy_create_buffer(context->inputStream.get(), SPLC_BUF_SIZE);
    yypush_buffer_state(state);
}

} // namespace splc::IO