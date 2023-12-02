#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "splcdef.h"

/* Expand given macro content. In design, this is called from yylex(). */
char *get_expanded_macro_content(const char *_content);

#endif /* PREPROCESSOR_H */