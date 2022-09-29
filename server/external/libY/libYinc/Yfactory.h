/***********************************************************************************
 * @ file    : Yfactory.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.08.31 19:33
 * @ brief   : 模块工厂
 * @ remark  ：
 ************************************************************************************/

#ifndef __YFACTORY_H__
#define __YFACTORY_H__

#include <cJSON.h>

#include "Y.h"
#include "Ylist.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct Yfactory_s Yfactory;
	typedef struct Yfactory_options_s Yfactory_options;
	typedef struct Ymodule_s Ymodule;
	typedef struct Ymodule_manifest_s Ymodule_manifest;
	typedef enum Ymodule_flags_e Ymodule_flags;
	typedef enum Ymodule_status_e Ymodule_status;

	typedef int(*Ymodule_initialize_entry)(Ymodule *module, void **context);
	typedef void(*Ymodule_release_entry)(Ymodule *module);

	typedef void(*Yfactory_initialized)(Yfactory *factory);
	typedef void(*Yfactory_module_status_changed)(Yfactory *factory, Ymodule *m, Ymodule_status status);

	// 模块的选项
	enum Ymodule_flags_e {
		YMOD_FLAGS_NONE,
		YMOD_FLAGS_DISABLED
	};

	// 模块状态
	enum Ymodule_status_e {
		YMOD_STAT_UN_INITIALIZED,
		YMOD_STAT_INITIALIZED,
		YMOD_STAT_INIT_FAILED,
		YMOD_STAT_INITIALIZING,
		YMOD_STAT_INIT_EXCEPTION
	};

	struct Ymodule_manifest_s {
		YCHAR id[256];
		YCHAR name[256];
		YCHAR desc[256];
		YCHAR author[256];
		YCHAR lib_path[YMAX_PATH];
		int flags;
		char *init_entry;					// 入口点函数名
		char *release_entry;				// 释放函数名
		cJSON *input;
	};

	struct Yfactory_options_s
	{
		// 初始化完成的回调
		Yfactory_initialized on_initialized;
		// 模块状态发生改变的回调
		Yfactory_module_status_changed on_module_status_changed;

		// 当某个模块初始化失败的时候，自动重试初始化的间隔时间
		// 单位毫秒
		int initial_interval;
	};

	/*
	 * 描述：
	 * 使用options创建一个工厂
	 *
	 * 参数：
	 * @opts：模块工厂的选项
	 *
	 * 返回值：
	 * Yfactory实例
	 */
	YAPI Yfactory *Y_create_factory(Yfactory_options *opts);

	/*
	 * 描述：
	 * 使用一组模块异步初始化模块工厂
	 *
	 * 参数：
	 * @modules：要加载的模块列表, Ymodule_manifest列表
	 *
	 * 返回值：
	 * Yfactory实例
	 */
	YAPI int Y_setup_factory_async(Yfactory *factory, Ylist *manifests);

	YAPI int Y_setup_factory_async2(Yfactory *factory, const YCHAR *config_file);

	YAPI void Y_delete_factory(Yfactory *factory);


	// module公开接口
	YAPI void *Y_module_context(Ymodule *module);
	YAPI int Y_module_config_get_int(Ymodule *module, const char *key, int defval);
	YAPI void Y_module_config_get_string(Ymodule *module, const char *key, char *buf, size_t bufsize, const char *defval);
	YAPI double Y_module_config_get_double(Ymodule *module, const char *key, double defval);

#ifdef __cplusplus
}
#endif

#endif
