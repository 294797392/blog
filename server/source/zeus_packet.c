#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef ZEUS_WIN32
#include <Windows.h>
#endif

#include "zeus_errno.h"
#include "zeus_string.h"
#include "zeus_const.h"
#include "zeus_packet.h"

#define HTTP_METHOD_GET             "GET"
#define HTTP_METHOD_HEAD            "HEAD"
#define HTTP_METHOD_PUT             "PUT"
#define HTTP_METHOD_POST            "POST"
#define HTTP_METHOD_TRACE           "TRACE"
#define HTTP_METHOD_OPTIONS         "OPTIONS"
#define HTTP_METHOD_DELETE          "DELETE"

#define CMPMETHOD(buf, method)  strnicmp(buf, method, strlen(method)) == 0

#define RECV_BUF_SIZE           512

typedef enum
{
    ZPSR_STATE_STARTLINE,

    ZPSR_STATE_HEADER,

    ZPSR_STATE_BODY
}ZEUS_PARSER_STATE;

static ZEUS_HTTP_METHOD text2method(const char *method)
{
    if (CMPMETHOD(method, HTTP_METHOD_GET))
    {
        return ZHTTP_METHOD_GET;
    }
    else if (CMPMETHOD(method, HTTP_METHOD_HEAD))
    {
        return ZHTTP_METHOD_HEAD;
    }
    else if (CMPMETHOD(method, HTTP_METHOD_PUT))
    {
        return ZHTTP_METHOD_PUT;
    }
    else if (CMPMETHOD(method, HTTP_METHOD_POST))
    {
        return ZHTTP_METHOD_POST;
    }
    else if (CMPMETHOD(method, HTTP_METHOD_TRACE))
    {
        return ZHTTP_METHOD_TRACE;
    }
    else if (CMPMETHOD(method, HTTP_METHOD_OPTIONS))
    {
        return ZHTTP_METHOD_TRACE;
    }
    else if (CMPMETHOD(method, HTTP_METHOD_DELETE))
    {
        return ZHTTP_METHOD_DELETE;
    }

    return ZHTTP_METHOD_UNKOWN;
}

int zeus_receive_packet(SOCKET client, zeus_packet *pkt)
{
    ZEUS_PARSER_STATE psr_state = ZPSR_STATE_STARTLINE;
    char *content = (char*)calloc(RECV_BUF_SIZE, 1);
    int offset = 0;

    ZEUS_HTTP_METHOD method = ZHTTP_METHOD_UNKOWN;
    char url[ZEUS_DEF_URL_SIZE] = {'\0'};

    while (1)
    {
        int rc = recv(client, content + offset, RECV_BUF_SIZE, 0);
        if(rc < 0)
        {
            // 如果数据报文收失败了，那么就断定为客户端已断开连接
            return ZERR_CLIENT_DISCONNECTED;
        }

        switch (psr_state)
        {
            case ZPSR_STATE_STARTLINE:
            {
                int lf = zeus_charat(content + offset, '\n');
                if(lf == -1)
                {
                    offset = strlen(content);
                    break;
                }

                // 解析method
                int index = zeus_charat(content, ' ');
                if(index == -1)
                {
                    return ZERR_INVALID_REQUEST;
                }

                method = text2method(content);
                if(method == ZHTTP_METHOD_UNKOWN)
                {
                    return ZERR_INVALID_REQUEST;
                }

                int second_space = zeus_charat2(content + index + 1, ' ', url);
                if(second_space == -1)
                {
                    return ZERR_INVALID_REQUEST;
                }
                else if(second_space == -2)
                {
                    zprintf(ZTEXT("解析URL失败, 缓冲区不足"));
                    return ZERR_INVALID_REQUEST;
                }

                int 

                if(content + lf - 1 == '\r')
                {
                    // 结尾是\r\n
                }
                else
                {
                    // 结尾是\n
                }

                break;
            }
            
            default:
                break;
        }
    }
}