/***********************************************************************************
 * @ file    : response.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 19:13
 * @ brief   : ����response����
 ************************************************************************************/

#ifndef __STEAK_RESPONSE_H__
#define __STEAK_RESPONSE_H__

#include <stdint.h>

#include "header.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// �洢��ǰHTTP��Ӧ����ϸ��Ϣ
	/// </summary>
	typedef struct steak_response_s steak_response;

	struct steak_response_s
	{
		/// <summary>
		/// HTTP��Ӧͷ��
		/// </summary>
		steak_http_header *header;

		char *content;
		int content_length;
	};

#ifdef __cplusplus
}
#endif

#endif