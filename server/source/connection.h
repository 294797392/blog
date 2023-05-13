/***********************************************************************************
 * @ file    : session.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.13 08:29
 * @ brief   : 定义HTTP链接对象
 ************************************************************************************/

#ifndef __STEAK_CONNECTION_H__
#define __STEAK_CONNECTION_H__

#include "session.h"
#include "parser.h"
#include "steak_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct steak_connection_s steak_connection;
	struct steak_connection_s
	{
		/// <summary>
		/// 客户端的socket
		/// </summary>
		steak_socket sock;

		/// <summary>
		/// 该链接当前所有的会话列表
		/// 一个会话就是请求 - 响应的过程
		/// </summary>
		Ylist *session_list;

		/// <summary>
		/// HTTP报文解析器
		/// </summary>
		steak_parser parser;
	};

#ifdef __cplusplus
}
#endif

#endif