/***********************************************************************************
 * @ file    : cblog_app.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 17:22
 * @ brief   : 定义服务应用程序框架的结构体以及未来给第三方进行模块扩展的结构体定义。
 *			 : 这些对象相当于ASP.NET里的八大对象
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
		/// 当收到了一个新的请求的时候触发该事件
		/// </summary>
		CBLOG_APP_EVENT_BEGIN_REUQEST = 0,

		/// <summary>
		/// 当HttpHandler处理完请求，准备发送到客户端之前触发
		/// </summary>
		CBLOG_APP_EVENT_END_REQUEST
	}cblog_app_event_enum;

	typedef int(*cblog_app_event_handler)(cblog_app *app);

	/// <summary>
	/// 存储整个App的详细信息
	/// </summary>
	typedef struct cblog_app_s cblog_app;

	struct cblog_app_s
	{
		/// <summary>
		/// app启动时间
		/// </summary>
		int startup_time;

		/// <summary>
		/// app运行时间
		/// </summary>
		int running_time;

		/// <summary>
		/// 服务主机列表
		/// 一个svchost实例维护着一个HTTP服务器的状态
		/// </summary>
		svchost **svchosts;
		int nsvchost;

		/// <summary>
		/// 事件轮询模块
		/// </summary>
		event_module *evm;

		cblog_app_event_handler **event_handlers;
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
	int cblog_app_init(const char *config);

	/// <summary>
	/// 运行app
	/// </summary>
	/// <returns></returns>
	int cblog_app_start();

	/*
	 * 描述：
	 * 获取app对象实例
	 *
	 * 返回值：
	 * steak_app实例
	 */
	cblog_app *cblog_app_get();

	/// <summary>
	/// 注册app事件
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