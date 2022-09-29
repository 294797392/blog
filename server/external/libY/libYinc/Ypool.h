/*****************************************************************************************************************
 * @ file    : Ypool.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.06 16:43
 * @ brief   : 实现一个线程安全的对象缓冲池，把对象开辟的内存空间的首地址存储起来，方便下次复用, 代码参考android Message类
 *				这个文件里不能加日志。。不然会造成循环调用
 *****************************************************************************************************************/

#ifndef __YPOOL_H__
#define __YPOOL_H__

#include "Y.h"

typedef struct Yobject_s Yobject;
typedef struct Ypool_s Ypool;

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * 描述：
	 * 创建一个对象缓冲池
	 *
	 * 参数：
	 * @max_block_size：缓冲池里可以申请的最大的缓冲区大小
	 * @max_blocks：缓冲池里的内存块的最大个数
	 *
	 * 返回值：
	 * 创建的缓冲池对象
	 */
	YAPI Ypool *Y_create_pool(int max_block_size, int max_blocks);

	/*
	 * 描述：
	 * 从对象缓冲池里获取一个对象
	 * 如果没有多余的对象了，那么会重新创建一个
	 * 如果缓冲池里的对象数量大于最大数量，那么会直接创建一个新的对象
	 *
	 * 参数：
	 * @yp：缓冲池对象
	 * @blocksize：要申请的内存块的大小
	 *
	 * 返回值：
	 * 缓冲对象
	 */
	YAPI Yobject *Y_pool_obtain(Ypool *yp, int blocksize);

	/*
	 * 描述：
	 * 回收缓冲对象，以便于下次继续使用
	 * 当你从缓冲池里obtain的对象用完了的时候请调用这个函数
	 * 这个函数会把你用完的对象重新放到缓冲池里，以便于下次复用
	 *
	 * 参数：
	 * @yo：要回收的对象
	 */
	YAPI void Y_pool_recycle(Yobject *yo);

	/*
	 * 描述：
	 * 获取Yobject里你开辟的内存空间
	 *
	 * 返回值：
	 * 内存地址
	 */
	YAPI void *Y_object_get_data(Yobject *yo);

#ifdef __cplusplus
}
#endif

#endif