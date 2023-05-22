/***********************************************************************************
 * @ file    : protocol.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.15 17:00
 * @ brief   : HTTP协议常量定义
 ************************************************************************************/

#ifndef __CBLOG_PROTOCOL_H__
#define __CBLOG_PROTOCOL_H__

#include "cblog_string.h"

#define CBLOG_HTTP_HEADER_STRING_CONNECTION                   "connection"
#define CBLOG_HTTP_HEADER_STRING_CONTENT_LENGTH               "content-length"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
        STEAK_HTTP_METHOD_GET,
        STEAK_HTTP_METHOD_HEAD,
        STEAK_HTTP_METHOD_PUT,
        STEAK_HTTP_METHOD_POST,
        STEAK_HTTP_METHOD_TRACE,
        STEAK_HTTP_METHOD_OPTIONS,
        STEAK_HTTP_METHOD_DELETE,
    }cblog_http_method_enum;

    typedef enum
    {
        /// <summary>
        /// HTTP 最早期的模型和 HTTP/1.0 的默认模型，是短连接。每一个 HTTP 请求都由它自己独立的连接完成；
        /// 这意味着发起每一个 HTTP 请求之前都会有一次 TCP 握手，而且是连续不断的。
        /// </summary>
        CBLOG_HTTP_VERSION_1,

        /// <summary>
        /// HTTP/1.1 的请求默认使用一个持久连接
        /// 如果出现了Connection: close头，那么按照短连接处理
        /// </summary>
        CBLOG_HTTP_VERSION_1DOT1,
        CBLOG_HTTP_VERSION_2,
        CBLOG_HTTP_VERSION_3
    }cblog_http_version_enum;

    typedef enum
    {
        CBLOG_HTTP_STATUS_CODE_OK = 200
    }cblog_http_status_code_enum;

    typedef enum
    {
        CBLOG_HTTP_HEADER_CONTENT_LENGTH
    }cblog_http_header_enum;

    int cblog_http_method_string2enum(cblog_string *str);
    int cblog_http_version_string2enum(cblog_string *str);
    const char *cblog_http_status_code_string(cblog_http_status_code_enum status_code);
    const char *cblog_http_version_string(cblog_http_version_enum version);
    const char *cblog_http_header_string(cblog_http_header_enum header);

#ifdef __cplusplus
}
#endif

#endif