#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// libY
#include <libY/Ylog.h>

#include "mm_errno.h"
#include "mm_config.h"

int main(int argc, char **argv)
{
    Ylog_global_init();

    mmcfg_init();

    YLOGI(YTEXT("mmsvc is running..."));

    while (1)
    {
        char line[1024] = {'\0'};
        fgets(line, sizeof(line), stdin);
    }

    return 0;
}
