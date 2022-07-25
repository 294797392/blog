/***********************************************************************************
 * @ file    : zeus_packet.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.24 0:03
 * @ brief   : 解析HTTP数据包，使用状态机模式来解析HTTP数据包
 ************************************************************************************/

#ifndef __ZEUS_PACKET_H__
#define __ZEUS_PACKET_H__

#ifdef ZEUS_WIN32
#include <Windows.h>
#endif

#include "zeus_const.h"
#include "zeus_http_procotol.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum 
    {
        ZEUS_PSRSTAT_GROUND,

        ZEUS_PSRSTAT_METHOD,
        ZEUS_PSRSTAT_URL,
        ZEUS_PSRSTAT_URL_PARAM,
        ZEUS_PSRSTAT_VER,
        ZEUS_PSRSTAT_VER_PARAM,

        ZEUS_PSRSTAT_HDR_KEY,
        ZEUS_PSRSTAT_HDR_KEY_PARAM,
        ZEUS_PSRSTAT_HDR_VAL,
        ZEUS_PSRSTAT_HDR_VAL_PARAM,
        ZEUS_PSRSTAT_HDR_END,

        ZEUS_PSRSTAT_BODY
    }ZEUS_PASER_STATE;

    typedef struct zeus_header_s zeus_header;
    struct zeus_header_s
    {
        char *key;
        int key_size;
        char *value;
        int value_size;
        zeus_header *next;  // 下一个header
    };

    // 定义一个格式化的HTTP数据包
    typedef struct zeus_packet_s
    {
        ZEUS_HTTP_METHOD method;                    // 请求方法
        char *method_text;                          // Method文本
        size_t method_size;

        ZEUS_HTTP_VERSION version;                  // http版本号
        char *version_text;
        size_t version_size;

        char *url;                                  // 请求的资源的URL，相对URL
        size_t url_size;                            // URL的长度

        zeus_header *first_hdr;                     // header链表
        zeus_header *last_hdr;
        int num_hdr;                                // HTTP头的数量
        int content_length;                         // 从header解析出来的content-length

        char *body;
        int body_size;
    }zeus_packet;

    /* 解析HTTP数据包的时候的上下文信息 */
    typedef struct zeus_parser_s
    {
        zeus_packet *packet;                        // HTTP数据包
        char *raw;                              // 原始HTTP报文
        int datasize;
        int bufsize;
        int psr_offset;                             // 当前解析到的字符的索引位置
        ZEUS_PASER_STATE state;                     // 解析器状态
    }zeus_parser;


    int zeus_psrse_packet(zeus_parser *psr);

#ifdef __cplusplus
}
#endif

#endif