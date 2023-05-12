/***********************************************************************************
 * @ file    : session.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 19:11
 * @ brief   : 定义session对象
 ************************************************************************************/

#ifndef __STEAK_SESSION_H__
#define __STEAK_SESSION_H__

#include <stdint.h>

#include "steak_socket.h"
#include "request.h"
#include "response.h"
#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// 存储当前会话的详细信息
	/// 一条HTTP网络连接就是一个会话
	/// </summary>
	typedef struct steak_session_s steak_session;

	struct steak_session_s
	{
		/// <summary>
		/// HTTP报文解析器
		/// </summary>
		steak_parser *parser;

		/// <summary>
		/// 当前正在处理的请求
		/// </summary>
		steak_request *request;

		/// <summary>
		/// 当前正在处理的响应
		/// </summary>
		steak_response *response;

		/// <summary>
		/// 客户端的socket
		/// </summary>
		steak_socket sock;
	};

#ifdef __cplusplus
}
#endif

#endif