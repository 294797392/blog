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

#include "steak_type.h"
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
		STEAK_PARSER_BODY
	};

	struct steak_parser_s
	{
		steak_parser_state state;

		/// <summary>
		/// 请求方法
		/// </summary>
		steak_string *method;
		steak_string *url;
		steak_string *version;
		steak_http_header *header;

		/// <summary>
		/// 当前解析到了的缓冲区的偏移量
		/// </summary>
		int offset;

		char *buffer;
		int buffer_size;
	};

	int steak_parser_parse(steak_parser *parser);

#ifdef __cplusplus
}
#endif

#endif
