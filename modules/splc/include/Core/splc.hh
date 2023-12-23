#ifndef __SPLC_CORE_SPLC_HH__
#define __SPLC_CORE_SPLC_HH__ 1

// Global definitions

#define __SPLC_VERSION__ "0.0.1"
#define __SPLC_MAJOR_VERSION__ 0
#define __SPLC_MINOR_VERSION__ 0
#define __SPLC_SUBMINOR_VERSION__ 1

// Include submodules

// Generated files
#ifndef yyFlexLexer // avoid cancelling existing definitions
#undef yyFlexLexer
#define yyFlexLexer SplcFlexLexer
#include <FlexLexer.h>
#endif

// Include core utilities

#include "Base.hh"
#include "Options.hh"
#include "System.hh"
#include "Utils.hh"

namespace splc {

} // namespace splc

#endif // __SPLC_CORE_SPLC_HH__
