/***********************************************************************************
 * @ file    : cblog_response.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.22 10:29
 * @ brief   : 提供操作HTTP响应的函数
 ************************************************************************************/

#ifndef __CBLOG_RESPONSE_H__
#define __CBLOG_RESPONSE_H__

#include "cblog_string.h"
#include "cblog_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	void cblog_response_write_header_string(cblog_response *response, cblog_http_header_enum k, char *v, int vlen);
	void cblog_response_write_header_int(cblog_response *response, cblog_http_header_enum k, int v);

	cblog_pending_response *new_cblog_pending_response(cblog_response *response);
	void free_cblog_pending_response(cblog_pending_response *response);

#ifdef __cplusplus
}
#endif

#endif