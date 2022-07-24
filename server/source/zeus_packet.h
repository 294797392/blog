/***********************************************************************************
 * @ file    : zeus_packet.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.24 0:03
 * @ brief   : 定义HTTP报文和相关的函数
 ************************************************************************************/

#ifndef __ZEUS_PACKET_H__
#define __ZEUS_PACKET_H__

#ifdef ZEUS_WIN32
#include <Windows.h>
#endif

#include "zeus_const.h"
#include "zeus_http.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct zeus_packet_s
    {
        ZEUS_HTTP_METHOD method;

        ZEUS_HTTP_VERSION version;

        char url[ZEUS_DEF_URL_SIZE];

        // 起始行
        char *start_line;

        // 首部
        char **headers;

        // 主体
        char *body;

    }zeus_packet;

    int zeus_receive_packet(SOCKET client, zeus_packet *pkt);

#ifdef __cplusplus
}
#endif


#endif