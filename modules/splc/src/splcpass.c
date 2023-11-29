#include "splcdef.h"
#include "splcpass.h"

void splc_run_single_passes(splc_single_pass* passes, splc_trans_unit tunit)
{
    while (*passes != NULL)
    {
        (*passes)(tunit);
        ++passes;
    }
}