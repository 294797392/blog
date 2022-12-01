#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#include "FDEvents.h"
#include "FDMonitor.h"
#include "ServiceHost.h"

struct ServiceHost
{
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	SOCKET Socket;
#elif (defined(ENV_UNIX))
    int Socket;
#endif

    FDMonitor *FDMonitor;
    ServiceHostOptions *Options;
};

ServiceHost *ServiceHostOpen(ServiceHostOptions *options)
{
    ServiceHost *svchost = (ServiceHost*)calloc(1, sizeof(ServiceHost));
    svchost->Options = options;
    svchost->FDMonitor = options->FDMonitor;

	if((svchost->Socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
        YLOGE("create socket failed, %d", errno);
        return NULL;
    }

	// 设置Socket选项，当程序异常退出之后再次打开程序的时候端口还可以继续使用
	int on = 1;
	if(setsockopt(svchost->Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		YLOGE("setsockopt failed SO_REUSEADDR, %d", errno);
		return NULL;
	}

	// 设置要侦听的网络接口和端口
	struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(options->ListenPort);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

	// 绑定
	if(bind(svchost->Socket, (struct sockaddr *)&bdaddr, sizeof(struct sockaddr)) < 0)
	{
		YLOGE("bind failed, %d", errno);
		return NULL;
	}

    // 监听
    if(listen(svchost->Socket, 5) < 0)
    {
        YLOGE("listen failed, %d", errno);
        return NULL;
    }

    // 文件描述符加入监控列表
    FileDescriptor *fd = FDMonitorCreateFD(svchost->FDMonitor);
    fd->OnRead = AcceptClientEvent;
    fd->Socket = svchost->Socket;
    fd->Read = 1;
    FDMonitorAddFD(svchost->FDMonitor, fd);

    return svchost;
}

void ServiceHostClose(ServiceHost *svchost)
{

}

