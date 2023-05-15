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
#include "protocol.h"

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
		/// Content-Length��ͷ��ֵ
		/// 0��content-lengthΪ0����ʾû��body
		/// </summary>
		int content_length;

		/// <summary>
		/// ���󷽷�
		/// </summary>
		http_method_enum method;
		char *url;
		char *version;
		char *content;
		steak_http_header *first_header;
		steak_http_header *last_header;
	};

#ifdef __cplusplus
}
#endif

#endif