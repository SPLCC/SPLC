#ifndef SPLCPASS_H
#define SPLCPASS_H

#include "splcdef.h"

/* Single-target pass function type. */
typedef void (*splc_single_pass)(splc_trans_unit tunit);

/* Multi-target pass function type. The input array must end with `NULL`. */
typedef void (*splc_multi_pass)(splc_trans_unit *tunits);

/* Perform a single pass on the target pass array. 
   Passes array must end with `NULL`. */
void splc_run_single_passes(splc_single_pass* passes, splc_trans_unit tunit);

#endif /* SPLCPASS_H */