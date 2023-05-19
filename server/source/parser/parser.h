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

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum steak_parser_state_e steak_parser_state;
	typedef enum steak_parser_event_enum steak_parser_event_enum;
	typedef struct steak_parser_s steak_parser;

	typedef void(*steak_parser_event_callback)(steak_parser *parser, steak_parser_event_enum evt);

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
		STEAK_PARSER_EVENT_BODY,
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

		/// <summary>
		/// 该解析器的用户数据
		/// </summary>
		void *userdata;
	};

	/// <summary>
	/// 解析HTTP报文
	/// </summary>
	/// <param name="parser">解析器对象</param>
	/// <param name="http_msg">完整的从客户端收到的HTTP请求报文</param>
	/// <param name="offset">http_msg的偏移量，从该处开始解析报文</param>
	/// <param name="size">要解析的报文长度</param>
	/// <returns>
	/// 0：报文解析结束
	/// -1：报文解析没结束，还需要更多的数据用来解析
	/// > 0：报文解析结束，但是数据没用完，解析的数据里包含下一次请求的数据。返回值表示解析了的字节数
	/// </returns>
	int steak_parser_parse(steak_parser *parser, char *http_msg, int offset, int size);

#ifdef __cplusplus
}
#endif

#endif

