/***********************************************************************************
 * @ file    : parser.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 21:15
 * @ brief   : HTTP报文解析器
 * https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods
 ************************************************************************************/

#ifndef __STEAK_PARSER__H__
#define __STEAK_PARSER__H__

#include <stdint.h>

#include <libY.h>

#include "steak_string.h"
#include "header.h"
#include "request.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum steak_parser_state_e steak_parser_state;
	typedef enum steak_parser_event_enum steak_parser_event_enum;
	typedef struct steak_parser_s steak_parser;

	typedef void(*steak_parser_event_callback)(steak_parser *parser, steak_parser_event_enum evt, char *data1, int data1len, char *data2, int data2len);

	enum steak_parser_state_e
	{
		STEAK_PARSER_INITIAL,
		STEAK_PARSER_METHOD,
		STEAK_PARSER_METHOD_END,
		STEAK_PARSER_URL,
		STEAK_PARSER_URL_END,
		STEAK_PARSER_VERSION,
		STEAK_PARSER_VERSION_END,
		STEAK_PARSER_HEADER_KEY,
		STEAK_PARSER_HEADER_VALUE,
		STEAK_PARSER_HEADER_VALUE_END,
		STEAK_PARSER_BODY,
	};

	enum steak_parser_event_enum
	{
		STEAK_PARSER_EVENT_METHOD,
		STEAK_PARSER_EVENT_URI,
		STEAK_PARSER_EVENT_VERSION,
		STEAK_PARSER_EVENT_HEADER,
		STEAK_PARSER_EVENT_BODY
	};

	struct steak_parser_s
	{
		/// <summary>
		/// HTTP报文的解析状态
		/// </summary>
		steak_parser_state state;

		/// <summary>
		/// 当前解析的segement相对于raw_msg的偏移量
		/// </summary>
		int seg_offset;

		/// <summary>
		/// 当前解析的segement的长度
		/// </summary>
		int seg_len;

		int seg2_offset;
		int seg2_len;

		/// <summary>
		/// 保存Content-Length标头的值，用来读取body
		/// </summary>
		int content_length;

		/// <summary>
		/// 当解析到HTTP报文的时候的回调函数
		/// </summary>
		steak_parser_event_callback on_event;
	};

	/// <summary>
	/// 解析HTTP报文
	/// </summary>
	/// <param name="parser">解析器对象</param>
	/// <param name="http_msg">完整的从客户端收到的HTTP请求报文</param>
	/// <param name="offset">http_msg的偏移量，从该处开始解析报文</param>
	/// <param name="size">要解析的报文长度</param>
	/// <returns>
	/// 使用了的字节数
	/// 有三种可能：
	/// 1. 返回值小于size：说明本次报文解析完毕。返回值表示本次解析解析使用的字节数。当返回值小于size的时候说明数据里有新的HTTP报文出现。会出现这种情况吗?
	/// 2. 返回值等于size：说明没解析完，需要外部模块继续接收并解析HTTP报文
	/// 3. 返回值等于0：说明本次HTTP报文解析完毕，并且本次解析使用了所有的字节数
	/// </returns>
	int steak_parser_parse(steak_parser *parser, char *http_msg, int offset, int size);

#ifdef __cplusplus
}
#endif

#endif

