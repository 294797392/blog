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

#include "cblog_event_module.h"
#include "cblog_types.h"

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
	int steak_app_init(const char *config);

	/// <summary>
	/// 运行app
	/// </summary>
	/// <returns></returns>
	int steak_app_start();

	/*
	 * 描述：
	 * 获取app对象实例
	 *
	 * 返回值：
	 * steak_app实例
	 */
	steak_app *steak_app_get();

#ifdef __cplusplus
}
#endif

#endif