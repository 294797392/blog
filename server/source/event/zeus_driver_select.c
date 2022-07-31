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
#include "zeus_event_driver.h"

typedef struct select_ctx_s
{
	zfd svc;
	fd_set rdfds;
	fd_set wrfds;
}select_ctx;

static select_ctx select_contetx =
{
	.svc = 0,
};

//static void handle_new_client(zeus_svc *svc)
//{
//	select_ctx *ctx = (select_ctx *)svc->priv;
//
//	while(1)
//	{
//		struct sockaddr_in client_addr;
//#ifdef ZEUS_WIN32
//		int sin_size = sizeof(struct sockaddr_in);
//#else
//		unsigned int sin_size = sizeof(struct sockaddr_in);
//#endif
//
//		SOCKET clientfd = accept(svc->fd, (struct sockaddr *)&client_addr, &sin_size);
//		if(clientfd < 0)
//		{
//			int error = zeus_os_error();
//			zloge(ZTEXT("accept客户端失败, %d"), error);
//			continue;
//		}
//
//		FD_SET(clientfd, &ctx->rdfds);
//	}
//}
//
//static void select_thread_proc(void *userdata)
//{
//	zeus_svc *svc = (zeus_svc *)userdata;
//	select_ctx *ctx = (select_ctx *)svc->priv;
//
//	struct timeval timeout;
//	memset(&timeout, 0, sizeof(struct timeval));
//	timeout.tv_usec = 50 * 1000;
//
//	while(1)
//	{
//		// 因为每次select，系统内部会修改fd_set的值，所以这里每次都要赋值创建一个新的fd_set
//		fd_set readfds = ctx->rdfds;
//
//		/*
//		 * nfds:监控的文件描述符集里最大文件描述符加1，因为此参数会告诉内核检测前多少个文件描述符的状态,Windows平台下这个值填0
//		 * readfds:监控有读数据到达文件描述符集合
//		 * writefds:监控写数据到达文件描述符集合
//		 * exceptfds:监控异常发生达文件描述符集合,如带外数据到达异常
//		 * timeout:定时阻塞监控时间,设置为NULL表示一直等待，直到有事件发生
//		 */
//		int ret = select(0, &readfds, NULL, NULL, NULL);
//		if(ret == 0)
//		{
//			/* timeout */
//			continue;
//		}
//		else if(ret < 0)
//		{
//			/* error */
//			int error = zeus_os_error();
//			switch(error)
//			{
//			case EINTR: // 等待时捕获了一个信号，可以重新发起调用
//				continue;
//			default:
//				zloge(ZTEXT("select failed, %d"), error);
//				break;
//			}
//		}
//		else
//		{
//			/* one or more fd has io event */
//			if(FD_ISSET(ctx->socket, &ctx->rdfds))
//			{
//				// 此时处理新的客户端连接请求
//				handle_new_client(svc);
//			}
//			else
//			{
//				// 此时处理客户端数据
//				for(int i = 0; i < FD_SETSIZE; i++)
//				{
//
//				}
//			}
//		}
//	}
//}

static int select_initialize(zeus_event_driver *drv)
{
	select_ctx *ctx = (select_ctx *)zeus_calloc(1, sizeof(select_ctx));

	FD_ZERO(&ctx->rdfds);
	FD_ZERO(&ctx->wrfds);

	return NULL;
}

static int select_add_event(zeus_event_driver *drv, zeus_event *evt)
{
}

static void select_remove_event(zeus_event_driver *drv, zeus_event *evt)
{
}

struct zeus_event_driver_operation_s zeus_event_driver_select =
{
	.name = "select",
	.author = "",
	.type = ZEUS_EVT_DRV_SELECT,
	.context = &select_contetx,
	.initialize = select_initialize,
	.add_event = select_add_event,
	.remove_event = select_remove_event,
};

