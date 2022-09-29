/***********************************************************************************
 * @ file    : Ydll.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.09.04 13:32
 * @ brief   : 封装dll相关的操作
 * @ remark  ：
 ************************************************************************************/

#ifndef __YDLL_H__
#define __YDLL_H__

#include "Y.h"
#include "Ylist.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct Ydll_s Ydll;

	/// <summary>
	/// 动态加载一个dll
	/// </summary>
	/// <param name="dll_path">要加载的dll的路径</param>
	/// <returns>dll实例，如果加载失败则返回空</returns>
	YAPI int Y_load_dll(YCHAR *dll_path, Ydll **outdll);

	/// <summary>
	/// 释放一个加载的dll
	/// </summary>
	/// <param name="dll"></param>
	/// <returns></returns>
	YAPI void Y_unload_dll(Ydll *dll);

	YAPI void *Y_load_symbol(Ydll *dll, const char *symbol);

#ifdef __cplusplus
}
#endif

#endif