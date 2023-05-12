/***********************************************************************************
 * @ file    : request.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 19:15
 * @ brief   : 定义request头对象
 ************************************************************************************/

#ifndef __STEAK_REQUEST_H__
#define __STEAK_REQUEST_H__

#include <stdint.h>

#include "header.h"
#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// 存储当前HTTP请求的详细信息
	/// </summary>
	typedef struct steak_request_s steak_request;

	struct steak_request_s
	{
		/// <summary>
		/// 请求时间
		/// </summary>
		uint64_t timestamp;

		/// <summary>
		/// HTTP请求头部
		/// </summary>
		steak_http_header *header;

		/// <summary>
		/// HTTP请求解析器
		/// </summary>
		steak_parser *parser;

		char full_url[256];
		char resource_uri[128];

		int content_length;

		/// <summary>
		/// HTTP报文接收缓冲区
		/// </summary>
		char *raw_msg;

		/// <summary>
		/// HTTP报文接收缓冲区的大小
		/// </summary>
		int raw_msg_size;

		/// <summary>
		/// HTTP报文接收缓冲区的偏移量
		/// 也就是报文的总长度
		/// </summary>
		int raw_msg_offset;
	};

#ifdef __cplusplus
}
#endif

#endif