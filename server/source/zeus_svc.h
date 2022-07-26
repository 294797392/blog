/***********************************************************************************
 * @ file    : zeus_svc.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.26 21:00
 * @ brief   : HTTP服务实现
 ************************************************************************************/

#ifndef __ZEUS_SVC_H__
#define __ZEUS_SVC_H__

#include "zeus.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct zeus_svc_s zeus_svc;

	typedef void(*zeus_svc_new_data_func)(zeus *z, SOCKET fd, const char *data, size_t datasize, void *userdata);

    int new_zeus_svc(zeus *z);
    int delete_zeus_svc(zeus *z);
    int zeus_svc_start(zeus *z);
    void zeus_svc_new_data_callback(zeus *z, zeus_svc_new_data_func callback, void *userdata);

#ifdef __cplusplus
}
#endif

#endif

