#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef ZEUS_WIN32
#include <WinSock2.h>
#include <Windows.h>
#endif

#include "zeus_log.h"
#include "zeus_os.h"
#include "zeus_errno.h"
#include "zeus_svc.h"
#include "zeus_config.h"
#include "zeus_event_mgr.h"

// HTTP服务对象
struct zeus_svc_s
{
	int port;											// 监听的端口
	zfd fd;												// 服务器socket
};

zeus_svc *new_zeus_svc(zeus_config *config)
{
	zeus_svc *svc = (zeus_svc *)zeus_calloc(1, sizeof(zeus_svc));
	svc->port = config->listen_port;

#ifdef ZEUS_UNIX
	/*
	 * 如果客户端socket已经关闭，服务端继续向socket写数据，内核会发出SIGPIPE信号。
	 * 默认的操作是终止进程，但该信号可以被捕获并处理。
	 * 设置SIG_IGN选项忽略该信号
	 */
	signal(SIGPIPE, SIG_IGN);
#endif

	int error = 0;
	SOCKET s = -1;

	// 新建一个socket
	if((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		error = zeus_os_error();
		zloge(ZTEXT("socket初始化失败, %d"), error);
		return NULL;
	}

	// 设置Socket选项，当程序异常退出之后再次打开程序的时候端口还可以继续使用
	int on = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		error = zeus_os_error();
		zloge(ZTEXT("setsockopt失败, SO_REUSEADDR, %d"), error);
		return NULL;
	}

	// 设置要侦听的网络接口和端口
	struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(svc->port);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

	// 绑定
	if(bind(s, (struct sockaddr *)&bdaddr, sizeof(struct sockaddr)) < 0)
	{
		error = zeus_os_error();
		zloge(ZTEXT("bind失败, %d"), error);
		return NULL;
	}

	svc->fd = s;

	return svc;
}

void free_zeus_svc(zeus_svc *svc)
{
	free(svc);
}

void zeus_svc_start(zeus_svc *svc)
{
	// 开始监听
	if(listen(svc->fd, 5) < 0)
	{
		int	error = zeus_os_error();
		zloge(ZTEXT("listen失败, %d"), error);
	}
}

void zeus_svc_stop(zeus_svc *svc)
{
}

