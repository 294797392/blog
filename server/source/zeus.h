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
#include "zeus_event_driver.h"
#include "zeus_parser.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct zeus_svc_s zeus_svc;

	// 封装客户端信息
	typedef struct zeus_client_s
	{
		SOCKET fd;                                          // 客户端的文件描述符
		zeus_parser *parser;                                // HTTP数据包解析器
	}zeus_client;

	// HTTP服务对象
	struct zeus_svc_s
	{
		int port;                                           // 监听的端口
		zfd fd;												// 服务器socket
		zeus_event_driver *evt_drv;							// 文件描述符监听器
		void *priv;                                         // io模型的上下文信息
		void *userdata;                                     // io回调用户自定义数据
	};

#ifdef __cplusplus
}
#endif

#endif

