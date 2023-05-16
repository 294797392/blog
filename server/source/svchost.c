#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#include "default.h"
#include "event.h"
#include "svchost.h"

svchost *new_svchost()
{
	svchost *svc = (svchost *)calloc(1, sizeof(svchost));
	strncpy(svc->options.bindaddr, "0.0.0.0", strlen("0.0.0.0"));
	strncpy(svc->options.root, STEAK_DEFAULT_ROOT, strlen(STEAK_DEFAULT_ROOT));
	svc->options.port = STEAK_DEFAULT_LISTEN_PORT;

	if((svc->sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		YLOGE("create socket failed, %d", errno);
		return NULL;
	}

	// 设置Socket选项，当程序异常退出之后再次打开程序的时候端口还可以继续使用
	int on = 1;
	if(setsockopt(svc->sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		YLOGE("setsockopt failed SO_REUSEADDR, %d", errno);
		return NULL;
	}

	// 设置要侦听的网络接口和端口
	struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(svc->options.port);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

	// 绑定
	if(bind(svc->sock, (struct sockaddr *)&bdaddr, sizeof(struct sockaddr)) < 0)
	{
		YLOGE("bind failed, %d", errno);
		return NULL;
	}

	// 监听
	if(listen(svc->sock, 5) < 0)
	{
		YLOGE("listen failed, %d", errno);
		return NULL;
	}

	return svc;
}

void free_svchost(svchost *svc)
{

}
