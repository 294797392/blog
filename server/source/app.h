/***********************************************************************************
 * @ file    : app.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 17:22
 * @ brief   : 定义服务应用程序框架的结构体以及未来给第三方进行模块扩展的结构体定义。
 *			 : 这些对象相当于ASP.NET里的八大对象
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
	/// 存储整个App的详细信息
	/// </summary>
	typedef struct steak_app_s steak_app;

	struct steak_app_s
	{
		/// <summary>
		/// app启动时间
		/// </summary>
		int startup_time;

		/// <summary>
		/// app运行时间
		/// </summary>
		int running_time;
	};

	/*
	 * 描述：
	 * 初始化app对象
	 * 整个应用程序只有一个app对象
	 * app对象用来存储整个服务应用的上下文信息
	 *
	 * 返回值：
	 * STEAK_ERR
	 */
	int steak_app_init();

	/*
	 * 描述：
	 * 获取app对象实例
	 *
	 * 返回值：
	 * steak_app实例
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