#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#elif (defined(ENV_UNIX))
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

#include "FDEventPoll.h"
#include "ServiceHost.h"

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "libY.lib")
#endif

static void Initialize()
{
    Y_log_init(NULL);

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
    WORD version = MAKEWORD(1, 1);
    WSADATA wsaData;
    int rc = WSAStartup(version, &wsaData);
#endif
}

int main(int argc, char **argv)
{
    Initialize();

    FDEventPollOptions evpollOptions = 
    {
        .Type = FDMON_TYPE_SELECT
    };
    FDEventPoll *fdMonitor = FDEventPollCreate(&evpollOptions);

    ServiceHostOptions svchostOptions = 
    {
        .BindAddress = "0.0.0.0",
        .ListenPort = 1018,
        .RootDir = "~/",
        .FDEventPoll = fdMonitor
    };
    ServiceHost *svchost = ServiceHostOpen(&svchostOptions);    

    YLOGI("start webserver success");

    while (1)
    {
        FDEventPollPoll(fdMonitor);
    }
    return 0;
}
