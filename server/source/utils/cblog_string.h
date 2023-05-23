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

#define cblog_string_charat(_str, _offset) _str->buffer->pdata[_str->offset + _offset]
#define cblog_string_string(_str) _str->buffer->pdata + _str->offset

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct cblog_buffer_s cblog_buffer;
	typedef struct cblog_string_s cblog_string;
	struct cblog_string_s
	{
		/// <summary>
		/// 存储字符串的缓冲区
		/// </summary>
		cblog_buffer *buffer;

		/// <summary>
		/// 该字符串相对于缓冲区的偏移量
		/// </summary>
		int offset;

		/// <summary>
		/// 字符串长度
		/// </summary>
		int length;
	};

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

	int cblog_string_to_int32(cblog_string *str);

#ifdef __cplusplus
}
#endif

#endif