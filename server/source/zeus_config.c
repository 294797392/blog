#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "zeus_errno.h"
#include "zeus_config.h"

int zeus_pase_config(wchar_t *path, zeus_config **config)
{
    zeus_config *cfg = (zeus_config*)calloc(1, sizeof(zeus_config));


    *config = cfg;

    return ZEUS_OK;
}

