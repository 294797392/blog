/***********************************************************************************
 * @ file    : cblog_parser.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 21:15
 * @ brief   : HTTP���Ľ�����
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
		/// HTTP���ĵĽ���״̬
		/// </summary>
		cblog_parser_state_enum state;

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
		cblog_parser_event_callback on_event;

		/// <summary>
		/// �ý��������û�����
		/// </summary>
		void *userdata;
	};

	/// <summary>
	/// ����HTTP����
	/// ע���ⲿģ����ҪΪparser��content_length�ֶθ�ֵ����Ȼparser��֪������Ҫ������������
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
	int cblog_parser_parse(cblog_parser *parser, char *http_msg, int offset, int size);

#ifdef __cplusplus
}
#endif

#endif

