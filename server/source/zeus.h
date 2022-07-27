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

#include "zeus_parser.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct zeus_socket_s zeus_socket;
	typedef struct zeus_svc_s zeus_svc;
	typedef void(*zeus_socket_new_data_func)(zeus_svc *socketops, SOCKET fd, const char *data, size_t datasize, void *userdata);

	// 定义所支持的socket模型
	typedef enum
	{
		ZSOCKET_IO_MODE_SELECT,
		ZSOCKET_IO_MODE_EPOLL,
		ZSOCKET_IO_MODE_IOCP
	}ZEUS_SOCKET_MODE;

	// 封装socket的IO模型
	struct zeus_socket_s
	{
		ZEUS_SOCKET_MODE mode;
		void *(*initialize)(zeus_svc *svc);
		int(*start)(zeus_svc *svc);
		void(*stop)(zeus_svc *svc);
	};

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
		SOCKET fd;											// 服务器socket
		zeus_socket *socketops;                             // io模型
		void *priv;                                         // io模型的上下文信息
		zeus_socket_new_data_func on_client_new_data;		// io回调
		void *userdata;                                     // io回调用户自定义数据

		void *client_mgr;
	};

#ifdef __cplusplus
}
#endif

#endif

