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

static zeus *new_zeus()
{
    zeus *z = (zeus*)calloc(1, sizeof(zeus));

    return z;
}

int main(int argc, char **argv)
{
    int code = ZERR_OK;
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
