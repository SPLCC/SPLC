#ifndef __SPLC_IO_SCANNER_HH__
#define __SPLC_IO_SCANNER_HH__ 1

// https://stackoverflow.com/questions/35606354/multiple-parsers-in-flex-bison-include-fails
#include "Core/splc.hh"

#include "Core/Utils/LocationWrapper.hh"
#include "IO/Parser.hh"

namespace splc::IO {

class Scanner : public SplcFlexLexer {
  public:
    // TODO: switch to translationManager
    Scanner(const std::string &filename_, std::istream *in)
        : filename{filename_}, SplcFlexLexer(in){};
    virtual ~Scanner(){};

    // get rid of override virtual function warning
    using FlexLexer::yylex;

    /**
     * The main procedure called by `splc::IO::Parser` to get tokens.
     * `YY_DECL` is defined to be this function in Lexer.ll.
     * Method body created by flex in lexer.cc.
     *
     * \param lval Pointer to the semantic value of this token.
     * \param location The location of this token.
     * */
    virtual int yylex(splc::IO::Parser::value_type *const lval,
                      splc::IO::Parser::location_type *location);

    /**
     * The main procedure for `yyFlexLexer` to switch to a different
     * input stream.
     *
     */
    // virtual int yywrap();

  private:
    std::string filename;

    /* yyval ptr */
    splc::IO::Parser::value_type *yylval = nullptr;
};

} // namespace splc

#endif /* __SPLC_IO_SCANNER_HH__ */
