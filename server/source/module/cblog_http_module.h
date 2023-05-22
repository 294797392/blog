/***********************************************************************************
 * @ file    : cblog_http_module.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.22 16:57
 * @ brief   : HTTP模块可以处理一个HTTP请求中触发的多个事件，可以同时存在多个HTTPModule对
 *			   请求事件进行处理，处理完之后把请求交给HttpHandler
 ************************************************************************************/

#ifndef __CBLOG_HTTP_MODULE_H__
#define __CBLOG_HTTP_MODULE_H__

#include <libY.h>

#ifdef __cplusplus
extern "C" {
#endif
	
	typedef struct cblog_http_module_s cblog_http_module;

	typedef int(*cblog_http_module_init_func)(cblog_http_module *mod);

	typedef void(*cblog_http_module_release_func)(cblog_http_module *mod);

	/// <summary>
	/// 当收到了一个新的请求的时候触发该事件
	/// </summary>
	typedef int(*cblog_http_module_begin_request_func)(cblog_http_module *mod);

	/// <summary>
	/// 当HttpHandler处理完请求，准备发送到客户端之前触发
	/// </summary>
	typedef int(*cblog_http_module_end_request_func)(cblog_http_module *mod);

	struct cblog_http_module_s
	{
		void *ptr;
	};


#ifdef __cplusplus
}
#endif


#endif
