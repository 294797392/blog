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

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum steak_parser_state_e steak_parser_state;
	typedef struct steak_parser_s steak_parser;

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
		STEAK_PARSER_BODY
	};

	struct steak_parser_s
	{
		steak_parser_state state;

		/// <summary>
		/// 请求方法
		/// </summary>
		char *method;
		char *url;
		char *version;
		char *body;
		steak_http_header *first_header;
		steak_http_header *last_header;

		/// <summary>
		/// Content-Length标头的值
		/// -1：没有content-length标头
		/// 0：content-length为0，表示没有body
		/// </summary>
		int content_length;

		/// <summary>
		/// 本次从客户端读取到的数据长度
		/// </summary>
		int readsize;

		/// <summary>
		/// 当前解析的segement相对于raw_msg的偏移量
		/// </summary>
		int seg_offset;
		/// <summary>
		/// 当前解析的segement的长度
		/// </summary>
		int seg_len;

		/// <summary>
		/// 从客户端接收到的原始http报文
		/// </summary>
		char *raw_msg;
		/// <summary>
		/// 从客户端接收到的原始http报文长度
		/// </summary>
		int raw_msg_len;
		/// <summary>
		/// 当前解析到了的缓冲区的偏移量
		/// </summary>
		int raw_msg_offset;

		/// <summary>
		/// HTTP报文是否解析完毕
		/// </summary>
		int completed;
	};

	int steak_parser_parse(steak_parser *parser);

#ifdef __cplusplus
}
#endif

#endif
