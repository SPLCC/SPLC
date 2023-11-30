#include "splcpass.h"
#include "splcdef.h"
#include "utils.h"

void splc_run_single_passes(splc_single_pass *passes, splc_trans_unit tunit)
{
    while (*passes != NULL)
    {
        (*passes)(tunit);
        if (err_count)
        {
            SPLC_FWARN_NOLOC("%s at line %d: failed to execute pass. The remaining passes are omitted.", __FILE__, __LINE__);
            break;
        }
        ++passes;
    }
}