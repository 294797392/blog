/***********************************************************************************
 * @ file    : cblog_http_handler.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.15 11:00
 * @ brief   : ����HTTP����Ķ���
 ************************************************************************************/

#ifndef __CBLOG_HTTP_HANDLER_H__
#define __CBLOG_HTTP_HANDLER_H__

#include "cblog_types.h"
#include "cblog_response.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct cblog_http_context_s cblog_http_context;
	typedef struct cblog_http_handler_s cblog_http_handler;

	struct cblog_http_context_s
	{
		cblog_request *request;
		cblog_response *response;
	};

	struct cblog_http_handler_s
	{
		/// <summary>
		/// ��������
		/// 1. ���HTTP��Ӧ����
		/// </summary>
		void(*process_request)(cblog_http_context *ctx);
	};

#ifdef __cplusplus
}
#endif


#endif
