#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#if (defined(Y_WIN32))
#include <Windows.h>
#elif (defined(Y_UNIX))
#include <unistd.h>
#endif

#define DEFAULT_PORT		1018

typedef struct service_module_s
{
    int fd;
}service_module;

int init_service_module(Ymodule *module, void **context)
{
#ifdef Y_UNIX
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
		YLOGE(YTEXT("create socket failed, %d"), error);
		return NULL;
	}

	// 设置Socket选项，当程序异常退出之后再次打开程序的时候端口还可以继续使用
	int on = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		error = zeus_os_error();
		YLOGE(YTEXT("setsockopt SO_REUSEADDR failed, %d"), error);
		return NULL;
	}

	// 设置要侦听的网络接口和端口
	int port = Y_module_config_get_int(module, "port", DEFAULT_PORT);
	struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(port);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

	// 绑定
	if(bind(s, (struct sockaddr *)&bdaddr, sizeof(struct sockaddr)) < 0)
	{
		error = zeus_os_error();
		YLOGE(YTEXT("socket bind failed, %d"), error);
		return NULL;
	}

	service_module *svc = (service_module*)Ycalloc(1, sizeof(service_module));
	svc->fd = s;

	*context = svc;

	return YERR_SUCCESS;
}

void release_service_module(Ymodule *module)
{

}


