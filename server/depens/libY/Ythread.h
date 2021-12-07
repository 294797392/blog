#ifndef __YTHREAD_H__
#define __YTHREAD_H__

#include "Ybase.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct Ythread_s Ythread;

    typedef void(*Ythread_entry)(void *userdata);

    YAPI Ythread *Y_create_thread(Ythread_entry entry, void *userdata);

    YAPI void Y_delete_thread(Ythread *thread);

#ifdef __cplusplus
}
#endif

#endif