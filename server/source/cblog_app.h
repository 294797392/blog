/***********************************************************************************
 * @ file    : cblog_app.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 17:22
 * @ brief   : �������Ӧ�ó����ܵĽṹ���Լ�δ��������������ģ����չ�Ľṹ�嶨�塣
 *			 : ��Щ�����൱��ASP.NET��İ˴����
 ************************************************************************************/

#ifndef __CBLOG_APP_H__
#define __CBLOG_APP_H__

#include <stdint.h>

#include <libY.h>

#include "cblog_event_module.h"
#include "cblog_types.h"
#include "cblog_http_event.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct cblog_app_s cblog_app;
	typedef struct cblog_http_module_chain_s cblog_http_module_chain;
	typedef struct cblog_http_module_s cblog_http_module;

	struct cblog_http_module_chain_s
	{
		cblog_http_module *first;
		cblog_http_module *last;
		int count;
	};

	/// <summary>
	/// �洢����App����ϸ��Ϣ
	/// </summary>
	struct cblog_app_s
	{
		/// <summary>
		/// app����ʱ��
		/// </summary>
		int startup_time;

		/// <summary>
		/// app����ʱ��
		/// </summary>
		int running_time;

		/// <summary>
		/// ���������б�
		/// һ��svchostʵ��ά����һ��HTTP��������״̬
		/// </summary>
		svchost **svchosts;
		int nsvchost;

		/// <summary>
		/// �¼���ѯģ��
		/// </summary>
		event_module *evm;

		/// <summary>
		/// ����HTTP��������ṹ��
		/// </summary>
		cblog_http_handler *http_handler;

		/// <summary>
		/// app�¼��б�
		/// </summary>
		cblog_http_event_list *http_evlist;

		/// <summary>
		/// httpģ���б�
		/// </summary>
		cblog_http_module_chain *http_modules;
	};

	/*
	 * ������
	 * ��ʼ��app����
	 * ����Ӧ�ó���ֻ��һ��app����
	 * app���������洢��������Ӧ�õ���������Ϣ
	 *
	 * ����ֵ��
	 * STEAK_ERR
	 */
	int cblog_app_init(const char *config);

	/// <summary>
	/// ����app
	/// </summary>
	/// <returns></returns>
	int cblog_app_start();

	/*
	 * ������
	 * ��ȡapp����ʵ��
	 *
	 * ����ֵ��
	 * steak_appʵ��
	 */
	cblog_app *cblog_app_get();

#ifdef __cplusplus
}
#endif

#endif