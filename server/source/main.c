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

#include "cblog_app.h"
#include "cblog_default.h"
#include "cblog_types.h"
#include "cblog_errors.h"
#include "cblog_event_module.h"
#include "cblog_unit_test.h"

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "libY.lib")
#endif

extern int accept_client_event(event_module *evpoll, cblog_event *evt);

/// <summary>
/// 当客户端和服务器之间有一段时间没有通信了就表示该链接超时了
/// 关闭超时的连接
/// </summary>
static void cleanup_timeout_events(event_module *evm, cblog_event **events, int nevent)
{

}

/// <summary>
/// 初始化所有的模块
/// </summary>
/// <returns></returns>
static int init_modules()
{
	return STEAK_ERR_OK;
}



int main(int argc, char **argv)
{
	int rc = YERR_SUCCESS;

	Y_log_init(NULL);

	if((rc = cblog_socket_init()) != STEAK_ERR_OK)
	{
		return 0;
	}

	if((rc = init_modules()) != STEAK_ERR_OK)
	{
		return 0;
	}

	if((rc = steak_app_init(STEAK_DEFAULT_CONFIG_FILE)) != STEAK_ERR_OK)
	{
		return 0;
	}

	if((rc = steak_app_start()) != STEAK_ERR_OK)
	{
		return 0;
	}

	YLOGI("start svchost success");

	steak_app *app = steak_app_get();
	event_module *evm = app->evm;

	while(1)
	{
		// 轮询事件列表
		rc = event_run_cycle(evm);
		if(rc != STEAK_ERR_OK)
		{
			YLOGE("poll event failed, %d", rc);
			continue;
		}

		// 处理超时了的事件
	}

	return 0;
}
