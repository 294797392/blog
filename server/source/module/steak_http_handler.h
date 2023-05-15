/***********************************************************************************
 * @ file    : steak_module.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.15 11:00
 * @ brief   : 当HTTP请求响应完了之后，用户自定义的处理HTTP请求的对象
 ************************************************************************************/

#ifndef __STEAK_MODULE_H__
#define __STEAK_MODULE_H__

#include "request.h"
#include "response.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// 存储模块状态
	/// </summary>
	typedef struct steak_module_s steak_module;

	/// <summary>
	/// 模块的输入参数
	/// </summary>
	typedef struct steak_module_input_s steak_module_input;

	struct steak_module_s
	{
		char *name;
		char *desc;
		char *author;

		/// <summary>
		/// 模块类型
		/// </summary>
		int type;

		/// <summary>
		/// 在整个应用启动的时候调用
		/// 只调用一次
		/// </summary>
		int(*initialize)();

		/// <summary>
		/// 在整个应用结束的时候调用
		/// 只调用一次
		/// </summary>
		void(*release)();

		/// <summary>
		/// 每收到一个HTTP请求就调用一次
		/// </summary>
		int(*process)(steak_module_input *input);
		void *context;
	};

	struct steak_module_input_s
	{
		steak_request *request;
		steak_response *response;
	};

#ifdef __cplusplus
}
#endif


#endif
