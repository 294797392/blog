/***********************************************************************************
 * @ file    : cblog_types.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.10 19:06
 * @ brief   : 定义cblog使用的主要的数据结构。独立模块单独定义到模块的头文件里
 ************************************************************************************/

#ifndef __CBLOG_TYPES_H__
#define __CBLOG_TYPES_H__

#include "parser.h"
#include "default.h"
#include "protocol.h"
#include "steak_socket.h"
#include "cblog_sockbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct cblog_string_s cblog_string;
	typedef struct cblog_http_header_s cblog_http_header;
	typedef struct svchost_s svchost;
	typedef struct svchost_options_s svchost_options;
	typedef enum cblog_conn_status_enum cblog_conn_status_enum;
	typedef struct cblog_connection_s cblog_connection;
	typedef struct cblog_response_s cblog_response;
	typedef struct cblog_request_s cblog_request;
	typedef struct cblog_session_s cblog_session;

	struct cblog_string_s
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
		cblog_socket sock;
		svchost_options options;
	};

	/// <summary>
	/// 保存HTTP标头信息
	/// </summary>
	struct cblog_http_header_s
	{
		cblog_string key;
		cblog_string value;
	};

	enum cblog_conn_status_enum
	{
		/// <summary>
		/// 已连接
		/// </summary>
		CBLOG_CONN_STATUS_CONNECTED,

		/// <summary>
		/// 主动关闭连接
		/// </summary>
		CBLOG_CONN_STATUS_CLOSE,

		/// <summary>
		/// 对方已关闭连接
		/// </summary>
		CBLOG_CONN_STATS_CLOSED_BY_PEER,

		/// <summary>
		/// 读/写出错
		/// </summary>
		CBLOG_CONN_STATUS_ERROR,

		/// <summary>
		/// 连接已超时
		/// </summary>
		CBLOG_CONN_STATUS_TIMEOUT
	};

	/// <summary>
	/// 记录与客户端的HTTP连接状态
	/// </summary>
	struct cblog_connection_s
	{
		/// <summary>
		/// 客户端的socket
		/// </summary>
		cblog_socket sock;

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
		cblog_request *request;

		/// <summary>
		/// 保存该连接最后一次响应的信息
		/// </summary>
		cblog_response *response;

		/// <summary>
		/// 接收缓冲区
		/// 保存当前request的原始HTTP报文
		/// </summary>
		cblog_sockbuf *recvbuf;

		/// <summary>
		/// 发送缓冲区
		/// 保存当前需要发送给客户端的原始HTTP报文
		/// </summary>
		cblog_sockbuf *sendbuf;

		/// <summary>
		/// 该连接的状态
		/// </summary>
		cblog_conn_status_enum status;
	};

	/// <summary>
	/// 存储当前HTTP请求的详细信息
	/// </summary>
	struct cblog_request_s
	{
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
		cblog_string method_string;
		cblog_string url;
		cblog_string version;
		cblog_string body;

		/// <summary>
		/// HTTP标头
		/// </summary>
		cblog_http_header headers[STEAK_DEFAULT_HEADER_COUNT];

		/// <summary>
		/// HTTP标头数量
		/// </summary>
		int nheader;
	};

	/// <summary>
	/// 记录HTTP响应的详细信息
	/// </summary>
	struct cblog_response_s
	{
		/// <summary>
		/// HTTP响应头部
		/// </summary>
		cblog_http_header *header;

		char *content;
		int content_length;
	};

	/// <summary>
	/// 存储当前会话的详细信息
	/// 一条HTTP网络连接就是一个会话
	/// </summary>
	struct cblog_session_s
	{
		/// <summary>
		/// 该会话的请求状态
		/// </summary>
		cblog_request request;

		/// <summary>
		/// 该会话的响应状态
		/// </summary>
		cblog_response response;

		/// <summary>
		/// 上一个会话
		/// </summary>
		cblog_session *prev;

		/// <summary>
		/// 下一个会话
		/// </summary>
		cblog_session *next;
	};

#ifdef __cplusplus
}
#endif

#endif