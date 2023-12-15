#ifndef __SPLC_CORE_OPTIONS_HH__
#define __SPLC_CORE_OPTIONS_HH__ 1

namespace splc::options {

/* if set to 1, enable diagnostic outputs */
extern int verbose;

/* If 1, disable diagnostic colors */
extern int noDiagnosticColor;

/* if set to 1, dump AST */
extern int ASTDump;

/* if set to 1, convert AST to the concrete by appending punctuators */
extern int enableASTPunctuators;

/* if set to 1, color the output AST */
extern int noAstColor;

} // namespace splc::options

#endif /* __SPLC_CORE_OPTIONS_HH__ */