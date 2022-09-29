/***********************************************************************************
 * @ file    : Ylits.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.06
 * @ brief   : 实现一个动态数组
 ************************************************************************************/
#ifndef __YLIST_H__
#define __YLIST_H__

#include "Ybase.h"

typedef struct Ylist_s Ylist;

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (*Ylist_free_func)(void *item);

	typedef void (*Ylist_foreach_func)(Ylist *yl, void *item, void *userdata);

    YAPI Ylist *Y_create_list();

    YAPI Ylist *Y_create_list2(Ylist_free_func freefunc);

    /*
     * 描述：
     * 释放一个集合
     * 如果集合里当前有元素并且你设置了Y_lits_free_func，那么这个函数会帮你调用freefunc并释放集合对象
     * 
     * 参数：
     * @yl：要操作的集合
     */
    YAPI void Y_delete_list(Ylist *yl);

    /*
     * 描述：
     * 往集合里插入一个元素
     * 如果集合的当前空间不够存储新的元素，那么会自动扩充空间，新扩充的空间的大小是当前大小的两倍
     * 
     * 参数：
     * @yl：要操作的集合
	 * @item：要插入的元素
     */
    YAPI void Y_list_foreach(Ylist *yl, Ylist_foreach_func ff, void *userdata);

    /*
     * 描述：
     * 往集合里插入一个元素
     * 如果集合的当前空间不够存储新的元素，那么会自动扩充空间，新扩充的空间的大小是当前大小的两倍
     * 
     * 参数：
     * @yl：要操作的集合
	 * @item：要插入的元素
     */
    YAPI void Y_list_add(Ylist *yl, void *item);

    /*
     * 描述：
     * 清空集合里的元素
     * 如果你设置了Ylist_free_func，那么这个函数会帮你调用freefunc，然后把length设置成0
     * 
     * 参数：
     * @yl：要操作的集合
     */
    YAPI void Y_list_clear(Ylist *yl);

    /*
     * 描述：
     * 获取集合里元素的数量
     * 
     * 参数：
     * @yl：要操作的集合
     */
    YAPI int Y_list_count(Ylist *yl);

    /*
     * 描述：
     * 判断一个元素是否存在于集合里
     * 
     * 参数：
     * @yl：要判断的集合
     * @item：要判断的元素
     * 
     * 返回值：
     * 存在返回元素的索引，不存在返回-1
     */
    YAPI int Y_list_contains(Ylist *yl, void *item);

    /*
     * 描述：
     * 向集合里插入一个元素
     * 
     * 参数：
     * @yl：要插入的集合
     * @index：要插入的索引位置
     * @item：要插入的元素
     * 
     * 返回值：
     * 存在返回元素的索引，不存在返回-1
     */
    YAPI void Y_list_insert(Ylist *yl, int index, void *item);

    /*
     * 描述：
     * 从集合里移除一个元素
     * 
     * 参数：
     * @yl：要移除元素的集合
     * @item：要移除的元素
     * @free：如果free == 1，并且你指定了freefunc，那么该函数会帮你free掉item
     */
    YAPI void Y_list_remove(Ylist *yl, void *item, int free);

    /*
     * 描述：
     * 根据索引从集合里移除一个元素
     * 
     * 参数：
     * @yl：要移除元素的集合
     * @at：要移除的元素的索引
     * @free：如果free == 1，并且你指定了freefunc，那么该函数会帮你free掉item
     */
    YAPI void Y_list_removeat(Ylist *yl, int at, int free);

#ifdef __cplusplus
}
#endif

#endif