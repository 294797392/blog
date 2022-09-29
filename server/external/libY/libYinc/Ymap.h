/***************************************************************************************************************************
 * @ file    : Ymap.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.09 09:46
 * @ brief   : 实现一个哈希表
 * @ ref     ：https://zhuanlan.zhihu.com/p/95156642
 * @ remark  ：开启Y_MAP_CHAIN宏，指定使用链式扩展法解决hash冲突，使用除留余数计算hash值
 * 散列表：把数据分散着排列，分开排列
 **************************************************************************************************************************/

#ifndef __YMAP_H__
#define __YMAP_H__

#include "Y.h"

#ifdef __cplusplus
extern "C" {
#endif

    // Ymap对象
    typedef struct Ymap_s Ymap;

    // 计算hash值的函数
    typedef int(*Ymap_hash_func)(void *key);

    // 比较key的函数
    // 如果key相等，那么返回1
    // 如果key不相等，那么返回0
    typedef int(*Ymap_keycmp_func)(void *key1, void *key2);

    // 用来释放value的函数
    typedef void(*Ymap_free_func)(void *value);

    // 遍历哈希表的函数
    typedef void(*Ymap_foreach_func)(Ymap *ym, void *key, void *value, void *userdata);


    // 预定义的哈希函数
    // 使用java里的string类型的hash函数
    YAPI int Y_map_hash_string_java(void *key);

    // 预定义的字符串键比较函数
    YAPI int Y_map_keycmp_string(void *key1, void *key2);

    // 预定义的指针类型的键比较函数
    YAPI int Y_map_keycmp_pointer(void *key1, void *key2);


	/*
	 * 描述：
	 * 创建一个哈希表对象
	 *
	 * 返回值：
	 * 新创建的哈希表对象
	 */
    YAPI Ymap *Y_create_map(Ymap_hash_func hash, Ymap_keycmp_func keycmp, Ymap_free_func free);

	/*
	 * 描述：
	 * 删除hash表对象
	 *
     * 参数：
     * @ym：要删除的哈希表对象
	 */
    YAPI void Y_delete_map(Ymap *ym);

    /*
     * 描述：
     * 查找对应key的value
     *
     * 参数：
     * @ym：要操作的哈希表对象
     * @key：要查找的键
     *
     * 返回值：
     * key对应的value，注意value可能是NULL
     */
    YAPI void *Y_map_get(Ymap *ym, void *key);

	/*
	 * 描述：
	 * 往hash表里存储一个键值对
	 * 
     * 参数：
     * @ym：要操作的哈希表对象
     * @key：要存储的键
     * @value：要存储的值
     * 
     * 返回值：
     * 如果该key已经存在，那么返回该key所对应的value
     * 如果该key不存在，那么返回value
	 */
    YAPI void *Y_map_set(Ymap *ym, void *key, void *value);

	/*
	 * 描述：
	 * 根据key移除一个键值对
	 *
     * 参数：
     * @ym：要删除的哈希表对象
     * @key：要删除的键值对的键
     * 
     * 注意，如果你设置了Ymap_free_func，那么该函数会帮你释放掉value
	 */
    YAPI void Y_map_remove(Ymap *ym, void *key);

	/*
	 * 描述：
	 * 清空hash表
	 *
     * 参数：
     * @ym：要清空的表
     * 
     * 注意，如果你设置了Ymap_free_func，那么该函数会帮你释放掉所有的value
	 */
    YAPI void Y_map_clear(Ymap *ym);

	/*
	 * 描述：
	 * 获取hash表里的元素的个数
	 *
     * 参数：
     * @ym：要获取元素个数的hash表
     * 
     * 返回值：
     * hash表里元素的个数
	 */
    YAPI int Y_map_count(Ymap *ym);

    /*
     * 描述：
     * 对hash表进行遍历操作
     *
     * 参数：
     * @ym：要遍历的表
     * @foreach：遍历函数
     * @userdata：遍历过程中用户自定义数据
     *
     * 注意，哈希表的遍历有可能是无序的
     */
    YAPI void Y_map_foreach(Ymap *ym, Ymap_foreach_func foreach, void *userdata);

#ifdef __cplusplus
}
#endif

#endif