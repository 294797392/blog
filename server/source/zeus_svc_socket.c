/***********************************************************************************
 * @ file    : zeus_svc_socket.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.26 21:07
 * @ brief   : 使用select模型实现HTTP服务
 * https://cloud.tencent.com/developer/article/1096023
 ************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef ZEUS_WIN32
#include <Windows.h>
#include <WinSock2.h>
#endif

#include "zeus_errno.h"
#include "zeus_log.h"
#include "zeus_os.h"
#include "zeus_svc.h"

struct zeus_svc_s
{
    SOCKET skfd;
    zeus_svc_new_data_func svc_new_data_callback;
    void *userdata;
    fd_set rdfds;
};

static void zeus_accept_thread_proc(void *userdata)
{
    zeus *z = (zeus*)userdata;
    zeus_svc *svc = z->svc;

    while (1)
    {
        struct sockaddr_in client_addr;
        #ifdef ZEUS_WIN32
		int sin_size = sizeof(struct sockaddr_in);
		#else
		unsigned int sin_size = sizeof(struct sockaddr_in);
		#endif

		SOCKET clientfd = accept(svc->skfd, (struct sockaddr*)&client_addr, &sin_size);
		if(clientfd < 0)
		{
            int error = zeus_os_error();
            zloge(ZTEXT("accept客户端失败, %d"), error);
			continue;
        }
    }
}

static void zeus_receive_thread_proc(void *userdata)
{
    zeus *z = (zeus*)userdata;
    zeus_svc *svc = z->svc;

    while (1)
    {
        // 因为每次select，系统内部会修改fd_set的值，所以这里每次都要赋值创建一个新的fd_set
        fd_set readfds = svc->rdfds;

        int ret = select(0, &readfds, NULL, NULL, NULL);
        if (ret == 0)
		{
			/* timeout */
			continue;
		}
		else if (ret < 0)
		{
			/* error */
			switch (errno)
			{
			case EINTR: // 等待时捕获了一个信号，可以重新发起调用
				continue;
			default:
                zloge(ZTEXT("select failed, %d"), zeus_os_error());
				break;
			}
		}
		else
		{
			/* one or more fd has io event */
			if (FD_ISSET(svc->skfd, &svc->rdfds))
			{
			}
		}
    }
}

int new_zeus_svc(zeus *z)
{
#ifdef ZEUS_UNIX
    	/*
	 * 如果客户端socket已经关闭，服务端继续向socket写数据，内核会发出SIGPIPE信号。
	 * 默认的操作是终止进程，但该信号可以被捕获并处理。
	 * 设置SIG_IGN选项忽略该信号
	 */
	signal(SIGPIPE, SIG_IGN);
#endif

    int error = 0;
    int port = z->config->listen_port;
    SOCKET s = -1;

    // 新建一个socket
    if((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        error = zeus_os_error();
        zloge(ZTEXT("socket初始化失败, %d"), error);
        return ZERR_FAIL;
    }

    // 设置Socket选项，当程序异常退出之后再次打开程序的时候端口还可以继续使用
    int on = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {  
        error = zeus_os_error();
        zloge(ZTEXT("setsockopt失败, SO_REUSEADDR, %d"), error);
		return ZERR_FAIL;
    }

    // 设置要侦听的网络接口和端口
    struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(port);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

    // 绑定
    if(bind(s, (struct sockaddr*)&bdaddr, sizeof(struct sockaddr)) < 0)
    {
        error = zeus_os_error();
        zloge(ZTEXT("bind失败, %d"), error);
		return ZERR_FAIL;
    }

    // 开始监听
	if(listen(s, 5) < 0)
	{
        error = zeus_os_error();
        zloge(ZTEXT("listen失败, %d"), error);
		return ZERR_FAIL;
	}

    zlogi(ZTEXT("zeus监听成功, 端口号：%d"), port);

    // 初始化select
    fd_set rdfds;
    FD_ZERO(&rdfds);
	FD_SET(s, &rdfds);

    // 初始化svc
    zeus_svc *svc = (zeus_svc*)zeus_calloc(1, sizeof(zeus_svc));
    svc->skfd = s;
    svc->rdfds = rdfds;
    z->svc = svc;

    // 启动线程接收客户端连接和接收数据
    #ifdef ZEUS_WIN32
    _beginthread(zeus_accept_thread_proc, 0, z);
    _beginthread(zeus_receive_thread_proc, 0, z);
    #endif

    return ZERR_OK;
}

int delete_zeus_svc(zeus *z)
{}

int zeus_svc_start(zeus *z)
{}

void zeus_svc_new_data_callback(zeus *z, zeus_svc_new_data_func callback, void *userdata)
{
    z->svc->svc_new_data_callback = callback;
    z->svc->userdata = userdata;
}

