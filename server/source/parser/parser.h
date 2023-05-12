/***********************************************************************************
 * @ file    : parser.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 21:15
 * @ brief   : HTTP���Ľ�����
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
		/// ���󷽷�
		/// </summary>
		char *method;
		char *url;
		char *version;
		char *body;
		steak_http_header *first_header;
		steak_http_header *last_header;

		/// <summary>
		/// Content-Length��ͷ��ֵ
		/// -1��û��content-length��ͷ
		/// 0��content-lengthΪ0����ʾû��body
		/// </summary>
		int content_length;

		/// <summary>
		/// ���δӿͻ��˶�ȡ�������ݳ���
		/// </summary>
		int readsize;

		/// <summary>
		/// ��ǰ������segement�����raw_msg��ƫ����
		/// </summary>
		int seg_offset;
		/// <summary>
		/// ��ǰ������segement�ĳ���
		/// </summary>
		int seg_len;

		/// <summary>
		/// �ӿͻ��˽��յ���ԭʼhttp����
		/// </summary>
		char *raw_msg;
		/// <summary>
		/// �ӿͻ��˽��յ���ԭʼhttp���ĳ���
		/// </summary>
		int raw_msg_len;
		/// <summary>
		/// ��ǰ�������˵Ļ�������ƫ����
		/// </summary>
		int raw_msg_offset;

		/// <summary>
		/// HTTP�����Ƿ�������
		/// </summary>
		int completed;
	};

	int steak_parser_parse(steak_parser *parser);

#ifdef __cplusplus
}
#endif

#endif
