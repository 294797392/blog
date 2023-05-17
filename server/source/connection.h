/***********************************************************************************
 * @ file    : session.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.13 08:29
 * @ brief   : 定义HTTP链接对象
 ************************************************************************************/

#ifndef __STEAK_CONNECTION_H__
#define __STEAK_CONNECTION_H__

#include <libY.h>

#include "session.h"
#include "parser.h"
#include "steak_socket.h"
#include "svchost.h"

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
		/// HTTP报文解析器
		/// </summary>
		steak_parser parser;

		/// <summary>
		/// 当接收很快，发送很慢的时候，可能会出现有多个响应没发送出去的情况
		/// 该队列存储待发送的响应
		/// </summary>
		steak_session *first_session;

		/// <summary>
		/// session列表里的最后一个会话，也是正在处理的会话
		/// </summary>
		steak_session *last_session;

		/// <summary>
		/// 当前正在接收的HTTP请求
		/// </summary>
		steak_session *active_session;

		/// <summary>
		/// 该链接所属的service
		/// </summary>
		svchost *svc;

		/// <summary>
		/// 接收缓冲区
		/// </summary>
		char *recv_buf;
		/// <summary>
		/// 接收缓冲区长度
		/// </summary>
		int recv_buf_len;
		/// <summary>
		/// 已经使用了的缓冲区长度
		/// 也就是从客户端接收到的原始http报文长度
		/// </summary>
		int recv_buf_offset;

		/// <summary>
		/// 发送缓冲区
		/// </summary>
		char *send_msg;
		int send_msg_len;
		int send_msg_offset;
	};

#ifdef __cplusplus
}
#endif

#endif