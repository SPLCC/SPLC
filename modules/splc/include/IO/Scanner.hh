#ifndef __SPLC_IO_SCANNER_HH__
#define __SPLC_IO_SCANNER_HH__ 1

#include "Core/splc.hh"

#include "IO/IOBase.hh"
#include "IO/Parser.hh"

#include "Translation/TranslationManager.hh"

namespace splc::IO {

class Scanner : public SplcFlexLexer {
  public:
    Scanner(TranslationManager &translationManager_, std::istream *in = nullptr)
        : translationManager{translationManager_}, SplcFlexLexer{in} {};
    virtual ~Scanner() = default;

    // get rid of override virtual function warning
    using FlexLexer::yylex;

    /// The main procedure called by `splc::IO::Parser` to get tokens.
    /// `YY_DECL` is defined to be this function in Lexer.ll.
    /// Method body created by flex in lexer.cc.
    ///
    /// \param lval Pointer to the semantic value of this token.
    /// \param location The location of this token.
    virtual int yylex(splc::IO::Parser::value_type *const yylval,
                      splc::IO::Parser::location_type *yyloc);

    /// \brief The main procedure for `yyFlexLexer` to switch to a different
    /// input stream.
    virtual int yywrap();

    void setInitialContext(Ptr<TranslationContext> initialContext);
    
    /// \brief Push file context and switch to the included file.
    void pushFileContext(const Location *intrLoc_, std::string_view fileName_);

    /// \brief Push macro context and switch to macro substitution.
    void pushMacroVarContext(const Location *intrLoc_, std::string_view macroVarName_);

    bool isMacroVarContextPresent(std::string_view macroVarName_) const;

    void registerMacroVarContext(const Location *regLocation, std::string_view macroVarName_, std::string_view content_);

    void unregisterMacroVarContext(const Location *unRegLoc, std::string_view macroVarName_);

  protected:
    void pushInternalBuffer(Ptr<TranslationContext> context);

    TranslationManager &translationManager;

    /// Theoretically, this stores the same pointer as the input in
    /// `yylex()`.

  public:
    friend class Driver;
    friend class Parser;
};

} // namespace splc::IO

#endif // __SPLC_IO_SCANNER_HH__
