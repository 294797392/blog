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
		/// HTTP���ĵĽ���״̬
		/// </summary>
		steak_parser_state state;

		/// <summary>
		/// ��ǰ������segement�����raw_msg��ƫ����
		/// </summary>
		int seg_offset;

		/// <summary>
		/// ��ǰ������segement�ĳ���
		/// </summary>
		int seg_len;

		int seg2_offset;
		int seg2_len;

		/// <summary>
		/// ����Content-Length��ͷ��ֵ��������ȡbody
		/// </summary>
		int content_length;

		/// <summary>
		/// ��������HTTP���ĵ�ʱ��Ļص�����
		/// </summary>
		steak_parser_event_callback on_event;

		/// <summary>
		/// �ý��������û�����
		/// </summary>
		void *userdata;
	};

	/// <summary>
	/// ����HTTP����
	/// </summary>
	/// <param name="parser">����������</param>
	/// <param name="http_msg">�����Ĵӿͻ����յ���HTTP������</param>
	/// <param name="offset">http_msg��ƫ�������Ӹô���ʼ��������</param>
	/// <param name="size">Ҫ�����ı��ĳ���</param>
	/// <returns>
	/// 0�����Ľ�������
	/// -1�����Ľ���û����������Ҫ�����������������
	/// > 0�����Ľ�����������������û���꣬�����������������һ����������ݡ�����ֵ��ʾ�����˵��ֽ���
	/// </returns>
	int steak_parser_parse(steak_parser *parser, char *http_msg, int offset, int size);

#ifdef __cplusplus
}
#endif

#endif

