#ifndef SPLCOPT_H
#define SPLCOPT_H
/* This file defines spl's option module, which helps splc parse include directories and other stuff. */

#include "splcdef.h"

/* Bad characters will not be used. */
#define SPLC_OPT_BADCH '?'

extern int splc_opterror; /* If set to 1, produce warnings upon error during parsing */
extern int splc_optind;   /* current index in the argument vector */
extern char splc_optopt;  /* the parsed option */
extern const char *splc_optfull; /* full argument */
extern const char *splc_optarg; /* the argument for the option */

/* The main function for splc to get options.
   When called, this function tries to parse an option from the argument vector.
   The argument for an option will be stored in the memory pointed by `splc_optarg`. The user
   is not expected to modify this, since it is entirely managed by `splcopt.c`.
   This function warns if `optarg` is empty.

   Arguments
   ---------
   - `nargc` the number of total arguments
   - `nargv` the argument vector
   - `ostr` the option string. The option string shall be in the following format:
     - Each individual character specifies an option to be parsed
     - If a character is followed by a colon ":", then this option expects exactly one argument
       after it.

   Return
   ------
   - `-1`, if no more arguments can be parsed.
   - `0`, if a direct argument to the program has been found.
   - `1`, if a known option is present.
   - `2`, if a full option is found
   - other `char` values, representing errors.

  */
int splc_getopt(const int nargc, const char *const nargv[], const char *ostr);

/* Parse all arguments for spl, and prepare for further processing. 
  */
void splc_process_args(const int nargc, const char *nargv[]);

void usage();

void print_prog_diag_info();

#endif /* SPLCOPT_H */