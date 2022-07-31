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
#include "zeus_event_driver.h"

#ifdef ZEUS_WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

int main(int argc, char **argv)
{
#ifdef ZEUS_WIN32
    WORD version = MAKEWORD(1, 1);
    WSADATA wsaData;
    int rc = WSAStartup(version, &wsaData);
#endif // ZEUS_WIN32

    int code = ZERR_OK;
    zeus_config *config = NULL;
    zeus_event_driver *event_driver = NULL;
    zeus_svc *svc = NULL;
    zeus *z = NULL;

    // 实例化变量
    z = (zeus *)zeus_calloc(1, sizeof(zeus));
    
    // 解析配置文件
    config = zeus_pase_config(L"");

    // 初始化事件监控模块，对文件描述符进行监控
    event_driver = new_event_driver(config);

    // 初始化服务
    svc = new_zeus_svc(config);


    //zeus_event_driver_add_event(event_driver, 


    z->config = config;
    z->svc = svc;
    z->event_driver = event_driver;

    zlogi(ZTEXT("zeus正在运行..."));

    while (1)
    {
        char line[1024] = {'\0'};
        fgets(line, sizeof(line), stdin);
    }

    return 0;
}
