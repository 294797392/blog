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
#include "zeus.h"
#include "zeus_worker.h"
#include "zeus_event.h"

#ifdef ZEUS_WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

static int start_workers(zeus *z)
{
    zeus_worker **workers = (zeus_worker**)calloc(z->config->num_worker, sizeof(zeus_worker));

    for(int i = 0; i < z->config->num_worker)
    {
        zeus_worker *zw = (zeus_worker*)calloc(1, sizeof(zeus_worker));
        zw->index = i;
        zw->z = z;
        zw->state = ZW_STATE_IDLE;
        zw->event = CreateEvent(NULL, FALSE, FALSE, NULL);

        workers[i] = zw;

        #ifdef ZEUS_WIN32
        _beginthread(zeus_worker_main, 0, zw);
        #endif
    }

    z->workers = workers;
    z->num_worker = z->config->num_worker;

    return ZEUS_OK;
}

static void zeus_handle_request(SOCKET client)
{

}

static void zeus_accept_request(void *userdata)
{
    zeus *z = (zeus*)userdata;

    while (1)
    {
        struct sockaddr_in client_addr;
        #ifdef ZEUS_WIN32
		int sin_size = sizeof(struct sockaddr_in);
		#else
		unsigned int sin_size = sizeof(struct sockaddr_in);
		#endif

		SOCKET clientfd = accept(z->fd, (struct sockaddr*)&client_addr, &sin_size);
		if(clientfd < 0)
		{
            int error = zeus_os_error();
            zloge(ZTEXT("accept客户端失败, %d"), error);
			continue;
        }

        zeus_handle_request(clientfd);
    }
}

static int start_service(zeus *z)
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
        zprintf(ZTEXT("socket初始化失败, %d"), error);
        return ZEUS_FAIL;
    }

    // 设置Socket选项，当程序异常退出之后再次打开程序的时候端口还可以继续使用
    int on = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {  
        error = zeus_os_error();
        zprintf(ZTEXT("setsockopt失败, SO_REUSEADDR, %d"), error);
		return ZEUS_FAIL;
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
        zprintf(ZTEXT("bind失败, %d"), error);
		return ZEUS_FAIL;
    }

    // 开始监听
	if(listen(s, 5) < 0)
	{
        error = zeus_os_error();
        zprintf(ZTEXT("listen失败, %d"), error);
		return ZEUS_FAIL;
	}

    zprintf(ZTEXT("zeus监听成功, 端口号：%d"), port);

    z->fd = s;

    // 启动线程接收客户端连接
    #ifdef ZEUS_WIN32
    _beginthread(zeus_accept_request, 0, z);
    #endif

    return ZEUS_OK;
}

static zeus *new_zeus()
{
    zeus *z = (zeus*)calloc(1, sizeof(zeus));

    return z;
}

int main(int argc, char **argv)
{
    int code = ZEUS_OK;
    zeus *z = NULL;
    zeus_config *config = NULL;

    z = new_zeus();

    zeus_pase_config("", &config);

    start_service(z);

    zlogi(ZTEXT("zeus正在运行..."));

    while (1)
    {
        char line[1024] = {'\0'};
        fgets(line, sizeof(line), stdin);
    }

    return 0;
}
