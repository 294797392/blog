/***********************************************************************************
 * @ file    : request.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 19:15
 * @ brief   : ����requestͷ����
 ************************************************************************************/

#ifndef __STEAK_REQUEST_H__
#define __STEAK_REQUEST_H__

#include <stdint.h>

#include "header.h"
#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// �洢��ǰHTTP�������ϸ��Ϣ
	/// </summary>
	typedef struct steak_request_s steak_request;

	struct steak_request_s
	{
		/// <summary>
		/// ����ʱ��
		/// </summary>
		uint64_t timestamp;

		/// <summary>
		/// HTTP����ͷ��
		/// </summary>
		steak_http_header *header;

		/// <summary>
		/// HTTP���������
		/// </summary>
		steak_parser *parser;

		char full_url[256];
		char resource_uri[128];

		int content_length;

		/// <summary>
		/// HTTP���Ľ��ջ�����
		/// </summary>
		char *raw_msg;

		/// <summary>
		/// HTTP���Ľ��ջ������Ĵ�С
		/// </summary>
		int raw_msg_size;

		/// <summary>
		/// HTTP���Ľ��ջ�������ƫ����
		/// Ҳ���Ǳ��ĵ��ܳ���
		/// </summary>
		int raw_msg_offset;
	};

#ifdef __cplusplus
}
#endif

#endif