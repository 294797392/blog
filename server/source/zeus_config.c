#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "zeus_errno.h"
#include "zeus_os.h"
#include "zeus_const.h"
#include "zeus_config.h"

zeus_config *zeus_pase_config(wchar_t *path)
{
    zeus_config *config = (zeus_config*)zeus_calloc(1, sizeof(zeus_config));
    config->listen_port = ZEUS_DEF_PORT;

    return config;
}

