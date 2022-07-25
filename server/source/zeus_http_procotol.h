/***********************************************************************************
 * @ file    : zeus_http_procotol.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.24 10:07
 * @ brief   : 定义HTTP协议常量
 ************************************************************************************/

#ifndef __ZEUS_HTTP_H__
#define __ZEUS_HTTP_H__

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
        ZHTTP_VERSION_0,
        ZHTTP_VERSION_1,
        ZHTTP_VERSION_1_1,
        ZHTTP_VERSION_2,
    }ZEUS_HTTP_VERSION;

    typedef enum
    {
        ZHTTP_METHOD_UNKOWN,
        ZHTTP_METHOD_GET,
        ZHTTP_METHOD_HEAD,
        ZHTTP_METHOD_PUT,
        ZHTTP_METHOD_POST,
        ZHTTP_METHOD_TRACE,
        ZHTTP_METHOD_OPTIONS,
        ZHTTP_METHOD_DELETE,
    }ZEUS_HTTP_METHOD;


    #define ZEUS_HDRKEY_CONTENT_LENGTH                "content-length"

#ifdef __cplusplus
}
#endif

#endif