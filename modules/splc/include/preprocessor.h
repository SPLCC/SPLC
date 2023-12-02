#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "splcdef.h"

/* Substitute all macro mount points inside the given AST.
   When subsituting macro functions, the following requirement holds:
   - Once the target macro function has been substituted, it is not possible to substitute the outer part again. */
void invoke_macro_subtitution(splc_trans_unit tunit);

#endif /* PREPROCESSOR_H */