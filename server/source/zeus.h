/***********************************************************************************
 * @ file    : zeus.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.28 00:54
 * @ brief   : 定义公共类型
 ************************************************************************************/

#ifndef __ZEUS_H__
#define __ZEUS_H__

#ifdef ZEUS_WIN32
#include <WinSock2.h>
#include <Windows.h>
#endif

#include "zeus_os.h"
#include "zeus_config.h"
#include "zeus_event_driver.h"
#include "zeus_parser.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct zeus_s zeus;
	typedef struct zeus_svc_s zeus_svc;
	typedef struct zeus_client_s zeus_client;

	// 封装客户端信息
	struct zeus_client_s
	{
		SOCKET fd;                                          // 客户端的文件描述符
		zeus_parser *parser;                                // HTTP数据包解析器
	};

	// HTTP服务对象
	struct zeus_svc_s
	{
		int port;                                           // 监听的端口
		zfd fd;												// 服务器socket
	};

	struct zeus_s
	{
		zeus_config *config;

		zeus_svc *svc;

		zeus_event_driver *event_driver;							// 文件描述符监听器
	};

#ifdef __cplusplus
}
#endif

#endif

