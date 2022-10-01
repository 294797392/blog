#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(Y_WIN32)) || (defined(Y_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#elif (defined(Y_UNIX))
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

#include <libY.h>

#include "errors.h"
#include "default.h"
#include "blog.h"
#include "listening.h"
#include "event.h"

#if (defined(Y_WIN32)) || (defined(Y_MINGW))
#pragma comment(lib, "Ws2_32.lib")
#endif

cJSON *config = NULL;
event_poll *poll = NULL;

static int setupenv()
{
    Y_log_init("Ylog.json");

#if (defined(Y_WIN32)) || (defined(Y_MINGW))
    WORD version = MAKEWORD(1, 1);
    WSADATA wsaData;
    int rc = WSAStartup(version, &wsaData);
#endif

    return YERR_SUCCESS;
}

static cJSON *read_config()
{
    YBYTE *bytes = NULL;
    uint64_t size = 0;
    int code = Y_file_readbytes(YTEXT(DEF_CONFIG_PATH), &bytes, &size);
    if(code != YERR_SUCCESS)
    {
        YLOGE(YTEXT("read config failed, %d, %s"), code, YTEXT(DEF_CONFIG_PATH));
        return NULL;
    }

    cJSON *config = cJSON_Parse(bytes);
    if(config == NULL)
    {
        YLOGE(YTEXT("parse config failed, invalid format"));
        return NULL;
    }

    return config;
}

int main(int argc, char **argv)
{
    int rc = 0;

    // 做一些环境初始化操作
    // 比如socket初始化
    setupenv();

    // 读取配置文件
    if((config = read_config()) == NULL)
    {
        return 0;
    }

    // 创建监听并打开监听
    Ylist *listens = create_listens(config);
    if((rc = open_listens(listens)) != 0)
    {
        return 0;
    }

    // 初始化事件轮询器
    if(!(poll = create_event_poll(config)))
    {
        return 0;
    }

    while (1)
    {
        rc = poll_events(poll);
        if(rc != 0)
        {
            YLOGE(YTEXT("poll_events failed, %d"), rc);
        }
    }
    
    return 0;
}
