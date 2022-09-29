/***********************************************************************************
 * @ file    : Ylock.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.16 22:29
 * @ brief   : 封裝不同平台下的锁操作
 ************************************************************************************/
#ifndef __YLOCK_H__
#define __YLOCK_H__

#include "Y.h"

#if (defined(Y_WIN32)) || (defined(Y_MINGW))
#include <Windows.h>
#elif (defined(Y_UNIX)) || (defined(Y_MSYS))
#include <pthread.h>
#include <semaphore.h>
#endif

#if (defined(Y_WIN32)) || (defined(Y_MINGW))
typedef CRITICAL_SECTION Ylock;
#define Y_create_lock(Y_lock)             InitializeCriticalSection(&Y_lock)
#define Y_delete_lock(Y_lock)             DeleteCriticalSection(&Y_lock)
#define Y_lock_lock(Y_lock)               EnterCriticalSection(&Y_lock)
#define Y_lock_unlock(Y_lock)             LeaveCriticalSection(&Y_lock)
#elif (defined(Y_UNIX)) || (defined(Y_MSYS))
typedef pthread_mutex_t Ylock;
#define Y_create_lock(Y_lock)             pthread_mutex_init(&Y_lock, NULL)
#define Y_delete_lock(Y_lock)             pthread_mutex_destroy(&Y_lock)
#define Y_lock_lock(Y_lock)               pthread_mutex_lock(&Y_lock)
#define Y_lock_unlock(Y_lock)             pthread_mutex_unlock(&Y_lock)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // typedef struct Ylock_s Ylock;

    // YAPI Ylock *Y_create_lock();

    // /*
    //  * 描述：
    //  * 删除一把锁
    //  * 
    //  * 参数：
    //  * @yl：要删除的锁
    //  */
    // YAPI void Y_delete_lock(Ylock *yl);

    // /*
    //  * 描述：
    //  * 执行锁操作
    //  * 
    //  * 参数：
    //  * @yl：要锁住的锁
    //  */
    // YAPI void Y_lock_lock(Ylock *yl);

    // /*
    //  * 描述：
    //  * 执行解锁操作
    //  * 
    //  * 参数：
    //  * @yl：要解锁的锁
    //  */
    // YAPI void Y_lock_unlock(Ylock *yl);

#ifdef __cplusplus
}
#endif

#endif