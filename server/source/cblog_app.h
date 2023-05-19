/***********************************************************************************
 * @ file    : app.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 17:22
 * @ brief   : �������Ӧ�ó����ܵĽṹ���Լ�δ��������������ģ����չ�Ľṹ�嶨�塣
 *			 : ��Щ�����൱��ASP.NET��İ˴����
 ************************************************************************************/

#ifndef __STEAK_APP_H__
#define __STEAK_APP_H__

#include <stdint.h>

#include <libY.h>

#include "cblog_event_module.h"
#include "cblog_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// �洢����App����ϸ��Ϣ
	/// </summary>
	typedef struct steak_app_s steak_app;

	struct steak_app_s
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
	int steak_app_init(const char *config);

	/// <summary>
	/// ����app
	/// </summary>
	/// <returns></returns>
	int steak_app_start();

	/*
	 * ������
	 * ��ȡapp����ʵ��
	 *
	 * ����ֵ��
	 * steak_appʵ��
	 */
	steak_app *steak_app_get();

#ifdef __cplusplus
}
#endif

#endif