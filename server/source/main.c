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

#include "errors.h"
#include "event.h"
#include "svchost.h"
#include "app.h"
#include "default.h"

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "libY.lib")
#endif

extern int accept_client_event(event_module *evpoll, steak_event *evt);

/// <summary>
/// 当客户端和服务器之间有一段时间没有通信了就表示该链接超时了
/// 关闭超时的连接
/// </summary>
static void cleanup_timeout_events(event_module *evm, steak_event **events, int nevent)
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
	Y_pool_init(65535, 8192);
	
	if((rc = steak_socket_init()) != STEAK_ERR_OK)
	{
		return 0;
	}

	if((rc = steak_app_init(STEAK_DEFAULT_CONFIG_FILE)) != STEAK_ERR_OK)
	{
		return 0;
	}

	if((rc = init_modules()) != STEAK_ERR_OK)
	{
		return 0;
	}

	YLOGI("start svchost success");

	int nevent = 0;
	steak_event **events = NULL;

	while(1)
	{
		// 轮询事件列表
		//events = (steak_event **)Y_list_to_array(evm->event_list, &nevent);
		//rc = event_poll(evm, events, nevent);
		//if(rc != STEAK_ERR_OK)
		//{
		//	YLOGE("poll event failed, %d", rc);
		//	continue;
		//}

		//// 处理有信号的事件
		//events = (steak_event **)Y_list_to_array(evm->process_event_list, &nevent);
		//if(nevent > 0)
		//{
		//	// 开始处理事件
		//	rc = event_process(evm, events, nevent);
		//	Y_list_clear(evm->process_event_list);
		//}

		//// 处理超时了的事件
		//events = (steak_event **)Y_list_to_array(evm->event_list, &nevent);
		//cleanup_timeout_events(evm, events, nevent);
	}

	return 0;
}
