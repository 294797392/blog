/***********************************************************************************
 * @ file    : cblog_http_module.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.22 16:57
 * @ brief   : HTTPģ����Դ���һ��HTTP�����д����Ķ���¼�������ͬʱ���ڶ��HTTPModule��
 *			   �����¼����д���������֮������󽻸�HttpHandler
 ************************************************************************************/

#ifndef __CBLOG_HTTP_MODULE_H__
#define __CBLOG_HTTP_MODULE_H__

#include <libY.h>

#include "cblog_app.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct cblog_http_module_s cblog_http_module;
	struct cblog_http_module_s
	{
		cblog_http_module *prev;
		cblog_http_module *next;

		char *name;
		char *desc;
		char *author;
		void *(*initialize)(cblog_app *app);
		void(*release)(void *ctx);
		void *context;
	};

#ifdef __cplusplus
}
#endif


#endif
