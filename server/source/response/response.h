/***********************************************************************************
 * @ file    : response.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 19:13
 * @ brief   : 定义response对象
 ************************************************************************************/

#ifndef __STEAK_RESPONSE_H__
#define __STEAK_RESPONSE_H__

#include <stdint.h>

#include "header.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// 存储当前HTTP响应的详细信息
	/// </summary>
	typedef struct steak_response_s steak_response;

	struct steak_response_s
	{
		/// <summary>
		/// HTTP响应头部
		/// </summary>
		steak_http_header *header;

		char *content;
		int content_length;
	};

#ifdef __cplusplus
}
#endif

#endif