%{

// Implementation of yyFlexScanner
#include "Core/splc.hh"
#include "IO/Scanner.hh"
#undef  YY_DECL
#define YY_DECL int splc::IO::Scanner::yylex(splc::IO::Parser::value_type *const lval, splc::IO::Parser::location_type *loc)

// Required std headers
#include <string>
#include <iostream>

// typedef to make the returns for the tokens shorter
using Token = splc::IO::Parser::token;

// define yyterminate as this instead of NULL
// #define yyterminate() return( Token::END )

// update location on matching
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

/* Option section */
%option c++
%option prefix="Splc"
%option never-interactive
%option yyclass="splc::IO::Scanner"
%option debug
%option nodefault
%option stack

/* Token section */
%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
            loc->begin.filename = &filename;
            loc->end.filename = &filename;
%}

[a-z]       {
                return( Token::LOWER );
            }

[A-Z]       {
                return( Token::UPPER );
            }

[a-zA-Z]+   {
                *yylval = yytext;
                return( Token::WORD );
            }

\n          {
                // Update line number
                loc->lines();
                return( Token::NEWLINE );
            }

.           {
                return ( Token::CHAR );
            }
%%

int yyFlexLexer::yywrap()
{
    // Inherit this method to let the code compile, but don't do anything.
    return 1;
}