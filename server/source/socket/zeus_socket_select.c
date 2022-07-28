#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef ZEUS_WIN32
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#endif

#include "zeus_log.h"
#include "zeus_os.h"
#include "zeus_errno.h"
#include "zeus.h"

typedef struct select_ctx_s
{
	SOCKET socket;
	fd_set rdfds;
}select_ctx;

static void handle_new_client(zeus_svc *svc)
{
	select_ctx *ctx = (select_ctx *)svc->priv;

	while(1)
	{
		struct sockaddr_in client_addr;
#ifdef ZEUS_WIN32
		int sin_size = sizeof(struct sockaddr_in);
#else
		unsigned int sin_size = sizeof(struct sockaddr_in);
#endif

		SOCKET clientfd = accept(svc->fd, (struct sockaddr *)&client_addr, &sin_size);
		if(clientfd < 0)
		{
			int error = zeus_os_error();
			zloge(ZTEXT("accept客户端失败, %d"), error);
			continue;
		}

		FD_SET(clientfd, &ctx->rdfds);
	}
}

static void select_thread_proc(void *userdata)
{
	zeus_svc *svc = (zeus_svc *)userdata;
	select_ctx *ctx = (select_ctx *)svc->priv;

	struct timeval timeout;
	memset(&timeout, 0, sizeof(struct timeval));
	timeout.tv_usec = 50 * 1000;

	while(1)
	{
		// 因为每次select，系统内部会修改fd_set的值，所以这里每次都要赋值创建一个新的fd_set
		fd_set readfds = ctx->rdfds;

		/*
		 * nfds:监控的文件描述符集里最大文件描述符加1，因为此参数会告诉内核检测前多少个文件描述符的状态,Windows平台下这个值填0
		 * readfds:监控有读数据到达文件描述符集合
		 * writefds:监控写数据到达文件描述符集合
		 * exceptfds:监控异常发生达文件描述符集合,如带外数据到达异常
		 * timeout:定时阻塞监控时间,设置为NULL表示一直等待，直到有事件发生
		 */
		int ret = select(0, &readfds, NULL, NULL, NULL);
		if(ret == 0)
		{
			/* timeout */
			continue;
		}
		else if(ret < 0)
		{
			/* error */
			int error = zeus_os_error();
			switch(error)
			{
			case EINTR: // 等待时捕获了一个信号，可以重新发起调用
				continue;
			default:
				zloge(ZTEXT("select failed, %d"), error);
				break;
			}
		}
		else
		{
			/* one or more fd has io event */
			if(FD_ISSET(ctx->socket, &ctx->rdfds))
			{
				// 此时处理新的客户端连接请求
				handle_new_client(svc);
			}
			else
			{
				// 此时处理客户端数据
				for(int i = 0; i < FD_SETSIZE; i++)
				{

				}
			}
		}
	}
}


static void *select_initialize(zeus_svc *svc)
{
	select_ctx *sctx = (select_ctx *)zeus_calloc(1, sizeof(select_ctx));

#ifdef ZEUS_UNIX
	/*
 * 如果客户端socket已经关闭，服务端继续向socket写数据，内核会发出SIGPIPE信号。
 * 默认的操作是终止进程，但该信号可以被捕获并处理。
 * 设置SIG_IGN选项忽略该信号
 */
	signal(SIGPIPE, SIG_IGN);
#endif

	int error = 0;
	int port = svc->port;
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
	bdaddr.sin_port = htons(port);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

	// 绑定
	if(bind(s, (struct sockaddr *)&bdaddr, sizeof(struct sockaddr)) < 0)
	{
		error = zeus_os_error();
		zloge(ZTEXT("bind失败, %d"), error);
		return NULL;
	}

	// 开始监听
	if(listen(s, 5) < 0)
	{
		error = zeus_os_error();
		zloge(ZTEXT("listen失败, %d"), error);
		return NULL;
	}

	// 初始化select
	FD_ZERO(&sctx->rdfds);
	FD_SET(s, &sctx->rdfds);
	sctx->socket = s;
	svc->fd = s;

	return sctx;
}

static int select_start(zeus_svc *svc)
{
	// 启动线程接收客户端连接和接收数据
#ifdef ZEUS_WIN32
	_beginthread(select_thread_proc, 0, svc);
#endif

	return ZERR_OK;
}

static void select_stop(zeus_svc *svc) {
}

struct zeus_socket_s socket_io_select =
{
	.mode = ZSOCKET_IO_MODE_SELECT,
	.initialize = select_initialize,
	.start = select_start,
	.stop = select_stop
};

