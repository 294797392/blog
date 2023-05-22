/***********************************************************************************
 * @ file    : cblog_types.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.10 19:06
 * @ brief   : 定义cblog使用的主要的数据结构。独立模块单独定义到模块的头文件里
 ************************************************************************************/

#ifndef __CBLOG_TYPES_H__
#define __CBLOG_TYPES_H__

#include "cblog_protocol.h"
#include "cblog_default.h"
#include "cblog_socket.h"
#include "cblog_parser.h"
#include "cblog_string.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct svchost_s svchost;
	typedef struct svchost_options_s svchost_options;
	typedef enum cblog_conn_status_enum cblog_conn_status_enum;
	typedef struct cblog_connection_s cblog_connection;
	typedef struct cblog_connection_options_s cblog_connection_options;
	typedef struct cblog_http_header_s cblog_http_header;
	typedef struct cblog_http_headers_s cblog_http_headers;
	typedef struct cblog_request_s cblog_request;
	typedef struct cblog_pending_response_s cblog_pending_response;
	typedef struct cblog_pending_response_chain_s cblog_pending_response_chain;
	typedef struct cblog_response_s cblog_response;
	typedef struct cblog_session_s cblog_session;

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
		cblog_http_header *next;
		cblog_http_header *prev;

		cblog_string key;
		cblog_string value;
	};

	struct cblog_http_headers_s
	{
		cblog_http_header *first;
		cblog_http_header *last;
		int count;
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
		cblog_http_method_enum method;
		cblog_string method_string;

		/// <summary>
		/// HTTP版本号
		/// </summary>
		cblog_http_version_enum version;
		cblog_string version_string;
		cblog_string url;
		cblog_string body;

		/// <summary>
		/// HTTP标头链表
		/// </summary>
		cblog_http_headers *header_chain;
	};

	/// <summary>
	/// 待响应数据
	/// 需要发送给客户端的数据
	/// </summary>
	struct cblog_pending_response_s
	{
		cblog_pending_response *prev;
		cblog_pending_response *next;

		char *buffer;
		int buflen;
	};

	/// <summary>
	/// 待响应列表
	/// 表示要发送给客户端的响应列表
	/// 考虑到send比recv慢，那么此时可能存储了多个请求而没有响应
	/// 为了解决这个问题，把所有请求对应的响应存储到一个列表里慢慢发送
	/// </summary>
	struct cblog_pending_response_chain_s
	{
		cblog_pending_response *first;
		cblog_pending_response *last;
		int count;
	};

	/// <summary>
	/// 记录HTTP响应的详细信息
	/// </summary>
	struct cblog_response_s
	{
		/// <summary>
		/// 存储所有HTTP标头的缓冲区
		/// </summary>
		cblog_buffer *header_buffer;

		/// <summary>
		/// HTTP标头链表
		/// </summary>
		cblog_http_headers *header_chain;

		/// <summary>
		/// HTTP版本号
		/// </summary>
		cblog_http_version_enum version;

		/// <summary>
		/// HTTP状态码
		/// </summary>
		cblog_http_status_code_enum status_code;

		/// <summary>
		/// body缓冲区
		/// </summary>
		cblog_buffer *body_buffer;

		/// <summary>
		/// 存储原始HTTP响应报文的缓冲区
		/// </summary>
		cblog_buffer *buffer;
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

	/// <summary>
	/// HTTP连接的选项参数
	/// </summary>
	struct cblog_connection_options_s
	{
		/// <summary>
		/// 是否保存原始的HTTP请求报文
		/// </summary>
		int save_request;

		/// <summary>
		/// 保存HTTP请求报文的文件路径
		/// </summary>
		char save_request_path[256];
	};

	/// <summary>
	/// 记录与客户端的HTTP连接状态
	/// 记录一条HTTP连接的状态
	/// </summary>
	struct cblog_connection_s
	{
		/// <summary>
		/// 客户端的socket
		/// </summary>
		cblog_socket sock;

		/// <summary>
		/// 该链接的选项
		/// </summary>
		cblog_connection_options options;

		/// <summary>
		/// HTTP报文解析器
		/// </summary>
		cblog_parser parser;

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
		/// 表示要发送给客户端的响应列表
		/// 考虑到send比recv慢的情况，那么此时可能存储了多个请求而没有响应
		/// 为了解决这个问题，把所有请求对应的响应存储到一个列表里慢慢发送
		/// </summary>
		cblog_pending_response_chain *pending_response_list;

		/// <summary>
		/// 接收缓冲区
		/// 保存当前request的原始HTTP报文
		/// </summary>
		cblog_buffer *recvbuf;

		/// <summary>
		/// 该连接的状态
		/// </summary>
		cblog_conn_status_enum status;

		/// <summary>
		/// Connection标头是否需要keep-alive
		/// </summary>
		int keep_alive;
	};

#ifdef __cplusplus
}
#endif

#endif