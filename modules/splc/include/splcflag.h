#ifndef SPLCFLAG_H
#define SPLCFLAG_H

extern int splcf_enable_diag; /* if set to 1, enable diagnostic outputs */

extern int splcf_no_diagnostics_color; /* If 1, disable diagnostic colors */

extern int splcf_ast_dump; /* if set to 1, dump AST */

extern int splcf_enable_ast_punctuators; /* if set to 1, convert AST to the concrete by appending punctuators */

extern int splcf_enable_colored_ast; /* if set to 1, color the output AST */

#endif /* SPLCFLAG_H */