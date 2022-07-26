#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef ZEUS_WIN32
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#elif ZEUS_UNIX
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#endif

#include "zeus_errno.h"
#include "zeus_config.h"
#include "zeus_string.h"
#include "zeus_os.h"
#include "zeus_log.h"
#include "zeus.h"
#include "zeus_worker.h"

#ifdef ZEUS_WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

static int start_workers(zeus *z)
{
    zeus_worker **workers = (zeus_worker**)calloc(z->config->num_worker, sizeof(zeus_worker));

    for(int i = 0; i < z->config->num_worker)
    {
        zeus_worker *zw = (zeus_worker*)calloc(1, sizeof(zeus_worker));
        zw->index = i;
        zw->z = z;
        zw->state = ZW_STATE_IDLE;
        zw->event = CreateEvent(NULL, FALSE, FALSE, NULL);

        workers[i] = zw;

        #ifdef ZEUS_WIN32
        _beginthread(zeus_worker_main, 0, zw);
        #endif
    }

    z->workers = workers;
    z->num_worker = z->config->num_worker;

    return ZEUS_OK;
}

static zeus *new_zeus()
{
    zeus *z = (zeus*)calloc(1, sizeof(zeus));

    return z;
}

int main(int argc, char **argv)
{
    int code = ZEUS_OK;
    zeus *z = NULL;
    zeus_config *config = NULL;

    z = new_zeus();

    zeus_pase_config("", &config);

    zlogi(ZTEXT("zeus正在运行..."));

    while (1)
    {
        char line[1024] = {'\0'};
        fgets(line, sizeof(line), stdin);
    }

    return 0;
}
