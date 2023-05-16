/***********************************************************************************
 * @ file    : svchost.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.16 16:19
 * @ brief   : 定义HTTP服务实例对象
 ************************************************************************************/

#ifndef __SERVICE_HOST_H__
#define __SERVICE_HOST_H__

#include <libY.h>

#include "steak_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct svchost_s svchost;
    typedef struct svchost_options_s svchost_options;

    struct svchost_options_s
    {
        char bindaddr[64];
        int port;
        char root[256];
        char index_page[256];
    };

    struct svchost_s
    {
        steak_socket sock;
        svchost_options options;
    };

#ifdef __cplusplus
}
#endif


#endif