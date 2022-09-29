/***********************************************************************************
 * @ file    : Yqueue.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.09.24
 * @ brief   : 实现一个先进先出的队列
 ************************************************************************************/
#ifndef __YQUEUE_H__
#define __YQUEUE_H__

#include "Y.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct Yqueue_s Yqueue;

    YAPI Yqueue *Y_create_queue();

    /*
     * 描述：
     * 释放一个集合
     * 如果集合里当前有元素并且你设置了Y_lits_free_func，那么这个函数会帮你调用freefunc并释放集合对象
     * 
     * 参数：
     * @yl：要操作的集合
     */
    YAPI void Y_delete_queue(Yqueue *yq);

    /*
     * 描述：
     * 往集合里插入一个元素
     * 如果集合的当前空间不够存储新的元素，那么会自动扩充空间，新扩充的空间的大小是当前大小的两倍
     * 
     * 参数：
     * @yl：要操作的集合
	 * @item：要插入的元素
     */
    YAPI void Y_queue_enqueue(Yqueue *yq, void *item);

    /*
     * 描述：
     * 清空集合里的元素
     * 如果你设置了Ylist_free_func，那么这个函数会帮你调用freefunc，然后把length设置成0
     * 
     * 参数：
     * @yl：要操作的集合
     */
    YAPI void *Y_queue_dequeue(Yqueue *yl);

    /*
     * 描述：
     * 获取集合里元素的数量
     * 
     * 参数：
     * @yl：要操作的集合
     */
    YAPI int Y_queue_size(Yqueue *yq);

    YAPI void Y_queue_clear(Yqueue *yq);

#ifdef __cplusplus
}
#endif

#endif