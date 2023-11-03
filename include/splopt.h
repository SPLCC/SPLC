#ifndef SPLOPT_H
#define SPLOPT_H
/* This file defines the argparse module, which helps splc parse include directories and other stuff. */

/* Bad characters will not be used. */
#define SPL_OPT_BADCH '?'

extern int spl_opterror; /* If set to 1, produce warnings upon error during parsing */
extern int spl_optind;   /* current index in the argument vector */
extern char spl_optopt;  /* the parsed option */
extern char *spl_optarg; /* the argument for the option */

/* The main procedure to process args by spl.
   When called, this function try to parse an option from the argument vector.
   The argument for an option will be stored in the memory pointed by `spl_optarg`. The user
   is not expected to modify this, since it is entirely managed by `splcopt.c`.

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
   - `1`, if a known argument is present.
   - other `char` values, representing errors.

  */
int spl_parse_opt(int nargc, char *nargv[], const char *ostr);

#endif