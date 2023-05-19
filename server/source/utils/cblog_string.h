/***********************************************************************************
 * @ file    : cblog_string.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.11 16:45
 * @ brief   : 字符串工具函数
 ************************************************************************************/

#ifndef __CBLOG_STRING_H__
#define __CBLOG_STRING_H__

#include <stdint.h>

#include <libY.h>

#include "cblog_types.h"
#include "cblog_socket.h"

#define cblog_string_charat(_str, _offset) _str->buffer->ptr[_str->offset + _offset]
#define cblog_string_string(_str) _str->buffer->ptr + _str->offset

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// 
	/// </summary>
	/// <param name="str1"></param>
	/// <param name="str2"></param>
	/// <returns>str1和str2一样的话返回0，不一样返回1</returns>
	int cblog_string_casecmp(cblog_string *str1, const char *str2);
	int cblog_string_casecmp2(char *str1, const char *str2, int ncount);

	void cblog_string_print(cblog_string *str);
	void cblog_string_print2(char *prepend, cblog_string *str);
	void cblog_string_print_header(cblog_http_header *header);

	int cblog_string_to_int32(cblog_string *str);

#ifdef __cplusplus
}
#endif

#endif