/***********************************************************************************
 * @ file    : cblog_parser.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 21:15
 * @ brief   : HTTP报文解析器
 * https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods
 ************************************************************************************/

#ifndef __CBLOG_PARSER_H__
#define __CBLOG_PARSER_H__

#include <stdint.h>

#include <libY.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum cblog_parser_state_enum cblog_parser_state_enum;
	typedef enum cblog_parser_event_enum cblog_parser_event_enum;
	typedef struct cblog_parser_s cblog_parser;

	typedef void(*cblog_parser_event_callback)(cblog_parser *parser, cblog_parser_event_enum evt);

	enum cblog_parser_state_enum
	{
		CBLOG_PARSER_STATE_INITIAL,
		CBLOG_PARSER_STATE_METHOD,
		CBLOG_PARSER_STATE_METHOD_END,
		CBLOG_PARSER_STATE_URL,
		CBLOG_PARSER_STATE_URL_END,
		CBLOG_PARSER_STATE_VERSION,
		CBLOG_PARSER_STATE_VERSION_END,
		CBLOG_PARSER_STATE_HEADER_KEY,
		CBLOG_PARSER_STATE_HEADER_VALUE,
		CBLOG_PARSER_STATE_HEADER_VALUE_END,
		CBLOG_PARSER_STATE_BODY,
	};

	enum cblog_parser_event_enum
	{
		CBLOG_PARSER_EVENT_METHOD,
		CBLOG_PARSER_EVENT_URI,
		CBLOG_PARSER_EVENT_VERSION,
		CBLOG_PARSER_EVENT_HEADER,
		CBLOG_PARSER_EVENT_BODY,
	};

	struct cblog_parser_s
	{
		/// <summary>
		/// HTTP报文的解析状态
		/// </summary>
		cblog_parser_state_enum state;

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
		cblog_parser_event_callback on_event;

		/// <summary>
		/// 该解析器的用户数据
		/// </summary>
		void *userdata;
	};

	/// <summary>
	/// 解析HTTP报文
	/// 注意外部模块需要为parser的content_length字段赋值，不然parser不知道到底要解析多少数据
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
	int cblog_parser_parse(cblog_parser *parser, char *http_msg, int offset, int size);

#ifdef __cplusplus
}
#endif

#endif

