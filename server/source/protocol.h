/***********************************************************************************
 * @ file    : protocol.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.15 17:00
 * @ brief   : HTTP协议常量定义
 ************************************************************************************/

#ifndef __STEAK_PROTOCOL_H__
#define __STEAK_PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
        STEAK_HTTP_VERSION_0,
        STEAK_HTTP_VERSION_1,
        STEAK_HTTP_VERSION_1_1,
        STEAK_HTTP_VERSION_2,
    }http_version_enum;

    typedef enum
    {
        STEAK_HTTP_METHOD_GET,
        STEAK_HTTP_METHOD_HEAD,
        STEAK_HTTP_METHOD_PUT,
        STEAK_HTTP_METHOD_POST,
        STEAK_HTTP_METHOD_TRACE,
        STEAK_HTTP_METHOD_OPTIONS,
        STEAK_HTTP_METHOD_DELETE,
    }http_method_enum;

#ifdef __cplusplus
}
#endif

#endif