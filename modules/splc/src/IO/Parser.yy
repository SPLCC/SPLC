%skeleton "lalr1.cc"
%require  "3.8.2"
%define parse.trace // This is required for runtime traces. For example, symbol_name.

%define api.namespace {splc::IO}
%define api.parser.class {Parser}
// %define api.header.include { "IO/Parser.hh" }
%define parse.assert

%code requires{
    // Code section there will be placed directly inside `IO/Parser.hh`.
    #include "Core/Utils/LocationWrapper.hh"
    namespace splc {
    
    class TranslationManager;

    namespace IO {
    class Driver;
    class Scanner;
    } // namespace splc::IO

    } // namespace splc
}

%parse-param { TranslationManager  &translationManager }
%parse-param { Driver              &driver  }
%parse-param { Scanner             &scanner  }

%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    
    // include for all driver functions
    #include "Core/splc.hh"

    #include "IO/Driver.hh"

    #include "Translation/TranslationManager.hh"
    #include "Translation/TranslationLogHelper.hh"

    #undef yylex
    #define yylex scanner.yylex
}

%define api.symbol.prefix {} // The empty prefix is generally invalid, but there is namespace in C++.
%define api.value.type { std::string }
// %define api.location.file "../../include/Core/Utils/location.hh"
%define api.location.type { splc::Location }
%locations


// Start of token definition section
// %token END 0
%token UPPER
%token LOWER
%token WORD
%token NEWLINE
%token CHAR

// Start of production section
%%

list_option : | List { SPLC_TRLH_DEBUG(@1) << "Parsing finished."; }
  ;

List
  : item {}
  | List item {}
  ;

item
  : UPPER   { driver.add_upper(); }
  | LOWER   { driver.add_lower(); }
  | WORD    { driver.add_word( $1 ); }
  | NEWLINE { driver.add_newline(); }
  | CHAR    { driver.add_char(); }
  ;

%%


void splc::IO::Parser::error(const location_type &l, const std::string &err_message)
{
    SPLC_LOG_ERROR(&l) << err_message;
}

