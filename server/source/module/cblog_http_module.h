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

#ifdef __cplusplus
extern "C" {
#endif
	
	typedef struct cblog_http_module_s cblog_http_module;

	typedef int(*cblog_http_module_init_func)(cblog_http_module *mod);

	typedef void(*cblog_http_module_release_func)(cblog_http_module *mod);

	/// <summary>
	/// ���յ���һ���µ������ʱ�򴥷����¼�
	/// </summary>
	typedef int(*cblog_http_module_begin_request_func)(cblog_http_module *mod);

	/// <summary>
	/// ��HttpHandler����������׼�����͵��ͻ���֮ǰ����
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
