#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mm_errno.h"
#include "mm_default.h"
#include "mm_config.h"

int mmcfg_init()
{
    return MME_OK;
}

int mmcfg_get_port()
{
    return MMDEF_PORT;
}

int mmcfg_get_thread_num()
{
    return MMDEF_THREAD_NUM;
}

