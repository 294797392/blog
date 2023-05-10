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

#include "header.h"
#include "response.h"
#include "request.h"
#include "session.h"

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
	int steak_app_init();

	/*
	 * ������
	 * ��ȡapp����ʵ��
	 *
	 * ����ֵ��
	 * steak_appʵ��
	 */
	steak_app *steak_app_get();

	steak_session *new_session(steak_socket sock);
	void free_session(steak_session *session);

	steak_request *new_request();
	void free_request(steak_request *request);

	steak_response *new_response();
	void free_response(steak_response *response);

#ifdef __cplusplus
}
#endif

#endif