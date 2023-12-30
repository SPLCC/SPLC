#ifndef __SPLC_IO_IOBASE_HH__
#define __SPLC_IO_IOBASE_HH__ 1

#ifndef yyFlexLexer // avoid cancelling existing definitions
#define yyFlexLexer SplcFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

namespace splc {

// class SplcFlexLexer;
class Parser;
class Driver;

} // namespace splc

#endif // __SPLC_IO_IOBASE_HH__
