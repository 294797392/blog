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
#include "zeus_parser.h"
#include "zeus.h"
#include "zeus_svc.h"

#ifdef ZEUS_WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

typedef struct zeus_s
{
    zeus_config *config;
    zeus_svc *socket;
}zeus;

static void handle_client_new_data(zeus_svc *socketops, SOCKET fd, const char *data, size_t datasize, void *userdata)
{
}

int main(int argc, char **argv)
{
#ifdef ZEUS_WIN32
    WORD version = MAKEWORD(1, 1);
    WSADATA wsaData;
    int rc = WSAStartup(version, &wsaData);
#endif // ZEUS_WIN32

    int code = ZERR_OK;
    zeus_config *config = NULL;
    zeus_svc *svc_socket = NULL;
    zeus *z = NULL;

    // 实例化变量
    z = (zeus *)zeus_calloc(1, sizeof(zeus));
    config = zeus_pase_config(L"");
    svc_socket = new_zeus_svc(config->listen_port, ZSOCKET_IO_MODE_SELECT);
    zeus_svc_register_new_data_callback(svc_socket, handle_client_new_data, z);

    z->config = config;
    z->socket = svc_socket;

    zlogi(ZTEXT("zeus正在运行..."));

    while (1)
    {
        char line[1024] = {'\0'};
        fgets(line, sizeof(line), stdin);
    }

    return 0;
}
