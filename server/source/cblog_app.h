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

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum cblog_app_event_enum
	{
		/// <summary>
		/// ���յ���һ���µ������ʱ�򴥷����¼�
		/// </summary>
		CBLOG_APP_EVENT_BEGIN_REUQEST = 0,

		/// <summary>
		/// ��HttpHandler����������׼�����͵��ͻ���֮ǰ����
		/// </summary>
		CBLOG_APP_EVENT_END_REQUEST
	}cblog_app_event_enum;

	typedef int(*cblog_app_event_handler)(cblog_app *app);

	/// <summary>
	/// �洢����App����ϸ��Ϣ
	/// </summary>
	typedef struct cblog_app_s cblog_app;

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

		cblog_app_event_handler **event_handlers;
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

	/// <summary>
	/// ע��app�¼�
	/// </summary>
	/// <param name="app"></param>
	/// <param name="evt"></param>
	/// <param name="handler"></param>
	/// <returns></returns>
	void cblog_app_register_event(cblog_app *app, cblog_app_event_enum evt, cblog_app_event_handler handler);
	void cblog_app_unregister_event(cblog_app *app, cblog_app_event_enum evt, cblog_app_event_handler handler);

#ifdef __cplusplus
}
#endif

#endif