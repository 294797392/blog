/***********************************************************************************
 * @ file    : header.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 19:13
 * @ brief   : 定义HTTP头对象
 ************************************************************************************/

#ifndef __STEAK_HEADER_H__
#define __STEAK_HEADER_H__

#include <stdint.h>

#include "steak_type.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// HTTP头数据
	/// </summary>
	typedef struct steak_http_header_s steak_http_header;

	struct steak_http_header_s
	{
		steak_string *key;
		steak_string *value;

		steak_http_header *next;
		steak_http_header *prev;
	};

#ifdef __cplusplus
}
#endif

#endif