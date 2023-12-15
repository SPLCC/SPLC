%skeleton "lalr1.cc"
%require  "3.8.2"
%define parse.trace // This is required for runtime traces. For example, symbol_name.

%define api.namespace {splc}
%define api.parser.class {Parser}

%code requires{
   namespace splc {
      class Driver;
      class Scanner;

      namespace context {
         class ContextManager;
      }
   }
}

%parse-param { context::ContextManager  &contextManager  }
%parse-param { Driver                   &driver  }
%parse-param { Scanner                  &scanner  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "core/splc.hh"
   #include "driver.hh"

   #undef yylex
   #define yylex scanner.yylex
}

%define api.symbol.prefix {} // The empty prefix is generally invalid, but there is namespace in C++.
%define api.value.type { std::string }
%define parse.assert

%locations
%define api.location.file "../include/location.hh"

/* Start of token definition section */
/* %token END 0 */
%token UPPER
%token LOWER
%token WORD
%token NEWLINE
%token CHAR


/* Start of production section */
%%

list_option : | List;

List
  : item
  | List item
  ;

item
  : UPPER   { driver.add_upper(); }
  | LOWER   { driver.add_lower(); }
  | WORD    { driver.add_word( $1 ); }
  | NEWLINE { driver.add_newline(); }
  | CHAR    { driver.add_char(); }
  ;

%%


void splc::Parser::error(const location_type &l, const std::string &err_message)
{
   SPLC_LOG_ERROR(l, err_message);
}

