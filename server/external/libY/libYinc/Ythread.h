/***********************************************************************************
 * @ file    : Ythread.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.09 19:28
 * @ brief   : 封装不同平台下的线程函数
 ************************************************************************************/

#ifndef __YTHREAD_H__
#define __YTHREAD_H__

#include "Y.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct Ythread_s Ythread;

    typedef void(*Ythread_entry)(void *userdata);

    /*
     * 描述：
     * 创建一个Ythread对象并开始运行
     *
     * 参数：
     * @entry：线程函数
     * @userdata：传递给线程的自定义数据
     * 
     * 返回值：
     * Ythread对象
     */
    YAPI Ythread *Y_create_thread(Ythread_entry entry, void *userdata);

    /*
     * 描述：
     * 删除一个Ythread对象
     * 该函数会先等待线程运行完毕，然后再释放掉Ythread
     *
     * 参数：
     * @yt：要释放的thread对象
     */
    YAPI void Y_delete_thread(Ythread *yt);

#ifdef __cplusplus
}
#endif

#endif