/*****************************************************************************************************************
 * @ file    : zeus_poll.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.31 10:41
 * @ brief   : 实现一个线程安全的对象缓冲池，把对象开辟的内存空间的首地址存储起来，方便下次复用, 代码参考android Message类
 *****************************************************************************************************************/

#ifndef __ZEUS_POLL_H__
#define __ZEUS_POLL_H__

typedef struct Yobject_s Yobject;
typedef struct zeus_pool_s zeus_pool;

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
	zeus_pool *new_zeus_pool();

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
	zeus_pool *zeus_pool_obtain(Ypool *yp);

	/*
	 * 描述：
	 * 回收缓冲对象，以便于下次继续使用
	 * 当你从缓冲池里obtain的对象用完了的时候请调用这个函数
	 * 这个函数会把你用完的对象重新放到缓冲池里，以便于下次复用
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