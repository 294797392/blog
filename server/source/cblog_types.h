/***********************************************************************************
 * @ file    : cblog_types.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.10 19:06
 * @ brief   : 定义cblog使用的主要的数据结构。独立模块单独定义到模块的头文件里
 ************************************************************************************/

#ifndef __CBLOG_TYPES_H__
#define __CBLOG_TYPES_H__

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include "parser.h"
#include "default.h"
#include "protocol.h"

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
typedef SOCKET steak_socket;
#elif (defined(ENV_UNIX))
typedef int steak_socket;
#endif

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct steak_string_s steak_string;
	typedef struct svchost_s svchost;
	typedef struct svchost_options_s svchost_options;
	typedef struct steak_connection_s steak_connection;
	typedef struct steak_response_s steak_response;
	typedef struct steak_session_s steak_session;
	typedef struct steak_request_s steak_request;
	typedef struct steak_http_header_s steak_http_header;

	struct steak_string_s
	{
		int offset;
		int length;
	};

	struct svchost_options_s
	{
		char bindaddr[64];
		int port;
		char root[256];
		char index_page[256];
	};

	struct svchost_s
	{
		steak_socket sock;
		svchost_options options;
	};

	/// <summary>
	/// 保存HTTP标头信息
	/// </summary>
	struct steak_http_header_s
	{
		steak_string key;
		steak_string value;
	};

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
		/// 该链接所属的service
		/// </summary>
		svchost *svc;

		/// <summary>
		/// 保存该连接最后一次请求的信息
		/// </summary>
		steak_request *request;

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

	/// <summary>
	/// 存储当前HTTP请求的详细信息
	/// </summary>
	struct steak_request_s
	{
		/// <summary>
		/// 原始HTTP报文
		/// </summary>
		char *raw_msg;

		/// <summary>
		/// 请求时间
		/// </summary>
		uint64_t timestamp;

		/// <summary>
		/// Content-Length标头的值
		/// 0：content-length为0，表示没有body
		/// </summary>
		int content_length;

		/// <summary>
		/// 请求方法
		/// </summary>
		http_method_enum method;
		steak_string url;
		steak_string version;
		steak_string body;

		/// <summary>
		/// HTTP标头
		/// </summary>
		steak_http_header headers[STEAK_DEFAULT_HEADER_COUNT];

		/// <summary>
		/// HTTP标头数量
		/// </summary>
		int nheader;
	};

	/// <summary>
	/// 记录HTTP响应的详细信息
	/// </summary>
	struct steak_response_s
	{
		/// <summary>
		/// HTTP响应头部
		/// </summary>
		steak_http_header *header;

		char *content;
		int content_length;
	};

	/// <summary>
	/// 存储当前会话的详细信息
	/// 一条HTTP网络连接就是一个会话
	/// </summary>
	struct steak_session_s
	{
		/// <summary>
		/// 该会话的请求状态
		/// </summary>
		steak_request request;

		/// <summary>
		/// 该会话的响应状态
		/// </summary>
		steak_response response;

		/// <summary>
		/// 上一个会话
		/// </summary>
		steak_session *prev;

		/// <summary>
		/// 下一个会话
		/// </summary>
		steak_session *next;
	};

#ifdef __cplusplus
}
#endif

#endif