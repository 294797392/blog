/***********************************************************************************
 * @ file    : Yevent.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.03.05 13:17
 * @ brief   : 封裝类似于C#里AutoResetEvent的功能
 * @ remark  ：linux条件变量参考：https://blog.csdn.net/zzran/article/details/8830213
 ************************************************************************************/
#ifndef __YEVENT_H__
#define __YEVENT_H__

#include "Y.h"

#ifdef __cplusplus
extern "C" {
#endif

    // 事件对象
    typedef struct Yevent_s Yevent;

    YAPI Yevent *Y_create_event();

    /*
     * 描述：
     * 删除一个事件
     * 
     * 参数：
     * @ye：要删除的事件
     */
    YAPI void Y_delete_event(Yevent *ye);

    /*
     * 描述：
     * 等待一个事件触发
     * 
     * 参数：
     * @ye：要等待的事件
     */
    YAPI void Y_event_wait(Yevent *ye);

    /*
     * 描述：
     * 触发一个事件
     * 
     * 参数：
     * @ye：要触发的事件
     */
    YAPI void Y_event_set(Yevent *ye);

#ifdef __cplusplus
}
#endif

#endif