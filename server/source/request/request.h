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

		char full_url[256];
		char resource_uri[128];

		char *content;
		int content_length;

		/// <summary>
		/// 保存接收到的数据
		/// </summary>
		char *buffer;
		int buffer_size;
		int write_offset;
	};

#ifdef __cplusplus
}
#endif

#endif