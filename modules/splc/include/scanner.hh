#ifndef __SPLC_SCANNER_HH__
#define __SPLC_SCANNER_HH__ 1

// https://stackoverflow.com/questions/35606354/multiple-parsers-in-flex-bison-include-fails
#include "core/splc.hh"

#include "location.hh"
#include "parser.hh"

namespace splc {

class Scanner : public SplcFlexLexer {
  public:
    // TODO: change to context manager
    Scanner(const std::string &filename_, std::istream *in) : filename{filename_}, SplcFlexLexer(in){};
    virtual ~Scanner(){};

    // get rid of override virtual function warning
    using FlexLexer::yylex;

    /** \brief The main procedure called by `splc::Parser` to get tokens.
     *  `YY_DECL` is defined to be this function in lexer.ll.
     *  Method body created by flex in lexer.cc.
     *
     *  \param lval Pointer to the semantic value of this token.
     *  \param location The location of this token. */
    virtual int yylex(splc::Parser::value_type *const lval,
                      splc::Parser::location_type *location);

    /** \brief The main procedure for `yyFlexLexer` to switch to a different input stream. 
     *  
    */
    // virtual int yywrap();

  private:
    std::string filename;

    /* yyval ptr */
    splc::Parser::value_type *yylval = nullptr;
};

} // namespace splc

#endif /* __SPLC_SCANNER_HH__ */
