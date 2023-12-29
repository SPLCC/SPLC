#ifndef __SPLC_IO_SCANNER_HH__
#define __SPLC_IO_SCANNER_HH__ 1

#include "Core/splc.hh"
#include "IO/Parser.hh"
#include "Translation/TranslationContext.hh"
#include "Translation/TranslationManager.hh"

namespace splc::IO {

class Scanner : public SplcFlexLexer {
  public:
    Scanner(TranslationManager &transMgr_, std::istream *in = nullptr)
        : transMgr{transMgr_}, tyCtx{transMgr_.getTyContext()}, SplcFlexLexer{in} {};
    virtual ~Scanner() = default;

    // get rid of override virtual function warning
    using FlexLexer::yylex;

    ///
    /// \brief The main procedure called by `splc::IO::Parser` to get tokens.
    ///
    /// `YY_DECL` is defined to be this function in Lexer.ll.
    /// Method body created by flex in lexer.cc.
    ///
    /// \param lval Pointer to the semantic value of this token.
    /// \param location The location of this token.
    virtual int yylex(splc::IO::Parser::value_type *const yylval,
                      splc::IO::Parser::location_type *yyloc);

    ///
    /// \brief The main procedure for `yyFlexLexer` to switch to a different
    ///        input stream.
    ///
    virtual int yywrap();

    bool setInitialContext(Ptr<TranslationContext> initialContext);

    /// \brief Push file context and switch to the included file.
    bool pushFileContext(const Location *intrLoc_, std::string_view fileName_);

    /// \brief Push macro context and switch to macro substitution.
    bool pushMacroVarContext(const Location *intrLoc_,
                             std::string_view macroVarName_);

    bool isContextPresent(TranslationContextBufferType type_,
                          std::string_view contextName_) const noexcept;

    bool
    isMacroVarContextPresent(std::string_view macroVarName_) const noexcept;

    bool registerMacroVarContext(const Location *regLocation,
                                 std::string_view macroVarName_,
                                 std::string_view content_);

    bool unregisterMacroVarContext(const Location *unRegLoc,
                                   std::string_view macroVarName_);

    /// Generally, do not use this because of performance penalty
    void stepLoc(splc::utils::Location &loc, std::string_view yytext);

  protected:
    void pushInternalBuffer(Ptr<TranslationContext> context);

    std::string concatTmpStrVec();
    Location concatTmpLocVec();

    TranslationManager &transMgr;
    Ptr<TypeContext> tyCtx;

    splc::IO::Parser::value_type *glval; ///< yylval
    splc::utils::Location *gloc;         ///< yyloc

    std::vector<std::string> strVec;
    std::vector<Location> locVec;

    bool internalFlag;
    std::string tmpStr;
    Location tmpLoc;

    /// Theoretically, this stores the same pointer as the input in
    /// `yylex()`.

  public:
    friend class Driver;
    friend class Parser;
};

} // namespace splc::IO

#endif // __SPLC_IO_SCANNER_HH__
