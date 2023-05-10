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
		/// 存放浏览器请求的数据
		/// </summary>
		steak_request *request;

		/// <summary>
		/// 存放要响应的数据
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