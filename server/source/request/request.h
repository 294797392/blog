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
#include "protocol.h"

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
		/// Content-Length标头的值
		/// 0：content-length为0，表示没有body
		/// </summary>
		int content_length;

		/// <summary>
		/// 请求方法
		/// </summary>
		http_method_enum method;
		char *url;
		char *version;
		char *content;
		steak_http_header *first_header;
		steak_http_header *last_header;
	};

#ifdef __cplusplus
}
#endif

#endif