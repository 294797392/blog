/*****************************************************************************************************************
 * @ file    : Ypool.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.06 16:43
 * @ brief   : 实现一个对象缓冲池，把对象开辟的内存空间的首地址存储起来，方便下次复用, 代码参考android Message类
 *				这个文件里不能加日志。。不然会造成循环调用
 *****************************************************************************************************************/

#ifndef __YPOOL_H__
#define __YPOOL_H__

#include "Yfirstinclude.h"

#include "Ybase.h"

typedef struct Yobject_s Yobject;
typedef struct Ypool_s Ypool;

#ifdef __cplusplus
extern "C" {
#endif

    /*
	 * 描述：
	 * 创建一个对象缓冲池
	 *
	 * 返回值：
	 * 创建的缓冲池对象
	 */
	YAPI Ypool *Y_create_pool();

	/*
	 * 描述：
	 * 从对象缓冲池里获取一个对象
	 * 如果没有多余的对象了，那么会重新创建一个
	 * 如果缓冲池里的对象数量大于最大数量，那么会直接创建一个新的对象
	 *
	 * 参数：
	 * @yp：缓冲池对象
	 *
	 * 返回值：
	 * 缓冲对象
	 */
	YAPI Yobject *Y_pool_obtain(Ypool *yp);

	/*
	 * 描述：
	 * 回收缓冲对象，以便于下次继续使用
	 *
	 * 参数：
	 * @yp：缓冲池对象
	 * @yo：要回收的对象
	 */
	YAPI void Y_pool_recycle(Ypool *yp, Yobject *yo);

	/*
	 * 描述：
	 * 获取Yobject里你开辟的内存空间
	 *
	 * 返回值：
	 * 内存地址
	 */
	YAPI void *Y_object_get_data(Yobject *yo);

	YAPI void Y_object_set_data(Yobject *yo, void *data);

#ifdef __cplusplus
}
#endif

#endif