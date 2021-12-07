#include <libY/Yfirstinclude.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef Y_API_WIN32
#include <WinSock2.h>
#elif Y_API_UNIX
#endif

// libY
#include <libY/Ylog.h>
#include <libY/Ythread.h>
#include <libY/Ynet.h>

// mm
#include "mm_errno.h"
#include "mm_config.h"

static Ythread *svc_thread;
static Ysocket ysock;

static void svc_thread_entry(void *userdata)
{
    while(1)
    {
        // accept()
    }
}

int main(int argc, char **argv)
{
    Ylog_global_init();

    mmcfg_init();

    ysock = Y_create_tcp_svc(NULL, mmcfg_get_port());
    svc_thread = Y_create_thread(svc_thread_entry, NULL);

    YLOGI(YTEXT("mmsvc is running..."));

    while (1)
    {
        char line[1024] = {'\0'};
        fgets(line, sizeof(line), stdin);
    }

    return 0;
}
