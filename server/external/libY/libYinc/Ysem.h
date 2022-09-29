/***********************************************************************************
 * @ file    : Ysem.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.02.26 10:45
 * @ brief   : 封裝不同平台下的信号量对象
 ************************************************************************************/
#ifndef __YSEM_H__
#define __YSEM_H__

#include "Y.h"

#if (defined(Y_WIN32)) || (defined(Y_MINGW))
#include <Windows.h>
#elif (defined(Y_UNIX)) || (defined(Y_MSYS))
#include <pthread.h>
#include <semaphore.h>
#endif

#if (defined(Y_WIN32)) || (defined(Y_MINGW))
typedef HANDLE Ysem;
#define Y_create_sem(Y_sem, max_size)       Y_sem = CreateSemaphoreW(NULL, 0, max_size, NULL)
#define Y_delete_sem(Y_sem)                 ReleaseSemaphore(Y_sem, 0, NULL); CloseHandle(Y_sem)
#define Y_sem_wait(Y_sem)                   WaitForSingleObject(Y_sem, INFINITE)
#define Y_sem_post(Y_sem)                   ReleaseSemaphore(Y_sem, 1, NULL)
#elif (defined(Y_UNIX)) || (defined(Y_MSYS))
typedef sem_t Ysem;
#define Y_create_sem(Y_sem, max_size)       sem_init(&Y_sem, 0, 0)
#define Y_delete_sem(Y_sem)                 sem_destroy(&Y_sem)
#define Y_sem_wait(Y_sem)                   sem_wait(&Y_sem)
#define Y_sem_post(Y_sem)                   sem_post(&Y_sem)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif