#include "IO/Scanner.hh"
#include "Core/System.hh"
#include "Core/Utils/Location.hh"
#include "Core/Utils/Logging.hh"
#include "Translation/TranslationContext.hh"
#include <numeric>

// *This file constitute only part of the definition of class
// `splc::IO::Scanner`. *The remaining implementation comes from `IO/lexer.ll".

namespace splc::IO {

int Scanner::yywrap()
{
    if (!transMgr.transCtxtStackEmpty()) {
        Ptr<TranslationContext> context = transMgr.popTransContext();
    }

    return transMgr.transCtxtStackEmpty();
}

bool Scanner::setInitialContext(Ptr<TranslationContext> initialContext)
{
    if (initialContext) {
        pushInternalBuffer(initialContext);
        return true;
    }
    return false;
}

bool Scanner::pushFileContext(const Location *intrLoc_,
                              std::string_view fileName_)
{
    try {
        Ptr<TranslationContext> context =
            transMgr.pushTransFileContext(intrLoc_, fileName_);

        pushInternalBuffer(context);
        return true;
    }
    catch (SemanticError e) {
        SPLC_LOG_ERROR(intrLoc_, true) << e.what();
    }
    return false;
}

bool Scanner::pushMacroVarContext(const Location *intrLoc_,
                                  std::string_view macroVarName_)
{
    try {
        Ptr<TranslationContext> context =
            transMgr.pushTransMacroVarContext(intrLoc_, macroVarName_);
        pushInternalBuffer(context);
        return true;
    }
    catch (SemanticError e) {
        SPLC_LOG_ERROR(intrLoc_, true) << e.what();
    }
    return false;
}

bool Scanner::isContextPresent(TranslationContextBufferType type_,
                               std::string_view contextName_) const noexcept
{
    return transMgr.isContextExistInStack(type_, contextName_);
}

bool Scanner::isMacroVarContextPresent(
    std::string_view macroVarName_) const noexcept
{
    return transMgr.isTransMacroVarPresent(macroVarName_);
}

bool Scanner::registerMacroVarContext(const Location *regLocation,
                                      std::string_view macroVarName_,
                                      std::string_view content_)
{
    try {
        return transMgr.registerTransMacroVarContext(regLocation, macroVarName_,
                                                     content_) != nullptr;
    }
    catch (SemanticError e) {
        SPLC_LOG_ERROR(regLocation, true) << e.what();
        if (e.loc)
            SPLC_LOG_NOTE(&e.loc, false) << "previously defined here";
        return false;
    }
}

bool Scanner::unregisterMacroVarContext(const Location *unRegLoc,
                                        std::string_view macroVarName_)
{
    try {
        return transMgr.unregisterTransMacroVarContext(
                   unRegLoc, macroVarName_) != nullptr;
    }
    catch (SemanticError e) {
        SPLC_LOG_ERROR(unRegLoc, true) << e.what();
        return false;
    }
}

void Scanner::pushInternalBuffer(Ptr<TranslationContext> context)
{
    yy_buffer_state *state =
        yy_create_buffer(context->inputStream.get(), SPLC_BUF_SIZE);
    yypush_buffer_state(state);
}

void Scanner::stepLoc(splc::utils::Location &loc, std::string_view yytext)
{
    using CounterType = splc::utils::Location::CounterType;

    CounterType lines = 0;
    CounterType columns = 0;

    for (auto c : yytext) {
        if (c == '\n') {
            columns = 0;
            ++lines;
        }
        else if (c != '\r')
            ++columns;
    }

    loc.step();
    loc.lines(lines);
    loc.columns(columns);
}

std::string Scanner::concatTmpStrVec()
{
    size_t count = accumulate(
        strVec.begin(), strVec.end(), 0,
        [](size_t sz, const std::string &s) { return sz + s.length(); });
    std::string result;
    result.reserve(count);
    std::for_each(strVec.begin(), strVec.end(),
                  [&](const std::string &s) { result.append(s); });
    strVec.clear();
    return result;
}

Location Scanner::concatTmpLocVec()
{
    Location loc =
        std::accumulate(++locVec.begin(), locVec.end(), *locVec.begin());
    locVec.clear();
    return loc;
}

} // namespace splc::IO