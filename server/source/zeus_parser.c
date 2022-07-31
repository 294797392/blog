#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "zeus_errno.h"
#include "zeus_string.h"
#include "zeus_const.h"
#include "zeus_os.h"
#include "zeus_parser.h"

#define HTTP_METHOD_GET             "GET"
#define HTTP_METHOD_HEAD            "HEAD"
#define HTTP_METHOD_PUT             "PUT"
#define HTTP_METHOD_POST            "POST"
#define HTTP_METHOD_TRACE           "TRACE"
#define HTTP_METHOD_OPTIONS         "OPTIONS"
#define HTTP_METHOD_DELETE          "DELETE"

#define CMPMETHOD(buf, method)  strnicmp(buf, method, strlen(method)) == 0

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

static int is_space(char c)
{
    return c == ' ' ? 1 : 0;
}

static int is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c >= 'Z');
}

static int is_number(char c)
{
    return c >= '0' && c <= 9;
}

static int is_method_param(char c)
{
    if(is_letter(c))
    {
        return 1;
    }

    return 0;
}

static int is_url_param(char c)
{
    // TODO：要处理URL中含有中文的情况
    if(is_letter(c) || is_number(c) || c == '/' || c == '\\')
    {
        return 1;
    }

    return 0;
}

static int is_hdr_param(char c)
{
    if(is_letter(c))
    {
        return 1;
    }

    return 0;
}

static int is_version_param(char c)
{
    // TODO：要处理URL中含有中文的情况
    if(c != ' ')
    {
        return 1;
    }

    return 0;
}

static void event_ground(const char *current, zeus_parser *psr)
{
    char c = current[0];
    zeus_packet *packet = psr->packet;

    if(is_method_param(c))
    {
        packet->method_text = current;
        packet->method_size++;
        psr->state = ZEUS_PSRSTAT_METHOD;
    }
    else
    {
        // ignore..
        zprintf(ZTEXT("ground state, ignore:%c"), c);
    }
}

static void event_method(const char *current, zeus_parser *psr)
{
    char c = current[0];
    zeus_packet *packet = psr->packet;

    if(is_space(c))
    {
        // Method结束了
        psr->state = ZEUS_PSRSTAT_URL;
    }
    else
    {
        packet->method_size++;
    }
}

static void event_url(const char *current, zeus_parser *psr)
{
    char c = current[0];
    zeus_packet *packet = psr->packet;

    if(is_space(c))
    {
        return;
    }
    else if(is_url_param(c))
    {
        packet->url = current;
        packet->url_size++;
        psr->state = ZEUS_PSRSTAT_URL_PARAM;
    }
    else
    {
        // ignore..
        zprintf(ZTEXT("url state, ignore:%c"), c);
    }
}

static void event_url_param(const char *current, zeus_parser *psr)
{
    char c = current[0];
    zeus_packet *packet = psr->packet;

    if(is_space(c))
    {
        psr->state = ZEUS_PSRSTAT_VER;
    }
    else if(is_url_param(c))
    {
        packet->url_size++;
    }
    else
    {
        // ignore..
        zprintf(ZTEXT("url param state, ignore:%c"), c);
    }
}

static void event_version(const char *current, zeus_parser *psr)
{
    char c = current[0];
    zeus_packet *packet = psr->packet;

    if(is_space(c))
    {
        return;
    }
    else if(is_version_param(c))
    {
        packet->version_text = current;
        packet->version_size++;
        psr->state = ZEUS_PSRSTAT_HDR_VAL_PARAM;
    }
    else
    {
        // ignore..
        zprintf(ZTEXT("version state, ignore:%c"), c);
    }
}

static void event_ver_param(const char *current, zeus_parser *psr)
{
    char c = current[0];
    zeus_packet *packet = psr->packet;

    if(c == '\n')
    {
        psr->state = ZEUS_PSRSTAT_HDR_KEY;
    }
    else if(is_version_param(c))
    {
        packet->version_size++;
    }
    else
    {
        // ignore..
        zprintf(ZTEXT("version param state, ignore:%c"), c);
    }
}

static void event_hdr_key(const char *current, zeus_parser *psr)
{
    char c = current[0];
    zeus_packet *packet = psr->packet;
    zeus_header *hdr = NULL;

    if(packet->num_hdr == 0)
    {
        // 第一次遇到header
        hdr = (zeus_header*)zeus_calloc(1, sizeof(zeus_header));
        packet->first_hdr = hdr;
        packet->last_hdr = hdr;
    }
    else
    {
        hdr = packet->last_hdr;
    }

    if(is_hdr_param(c))
    {
        if(hdr->key == NULL)
        {
            hdr->key = current;
        }

        // accumulate header key
        hdr->key_size++;
    }
    else if(c == ':')
    {
        psr->state = ZEUS_PSRSTAT_HDR_VAL;
    }
    else
    {
        // ignore..
        zprintf(ZTEXT("hdr key state, ignore:%c"), c);
    }
}

static void event_hdr_val(const char *current, zeus_parser *psr)
{
    char c = current[0];
    zeus_packet *packet = psr->packet;
    zeus_header *hdr = packet->last_hdr;

    if(is_hdr_param(c))
    {
        if(hdr->value == NULL)
        {
            hdr->value = current;
        }

        // accumulate header key
        hdr->value_size++;
    }
    else if(c == '\n')
    {
        // 此时说明一个头部结束了
        if(strnicmp(hdr->key, ZEUS_HDRKEY_CONTENT_LENGTH, hdr->key_size) == 0)
        {
            // TODO；解析content-length
        }

        packet->num_hdr++;
        psr->state = ZEUS_PSRSTAT_HDR_END;
    }
    else
    {
        // ignore..
        zprintf(ZTEXT("hdr key state, ignore:%c"), c);
    }
}

static void event_hdr_end(const char *current, zeus_parser *psr)
{
    char c = current[0];
    zeus_packet *packet = psr->packet;

    if(c == '\n')
    {
        // 头部结束后，紧接着马上又遇到了一个\n，那就说明所有标头都收完了
        psr->state = ZEUS_PSRSTAT_BODY;
    }
    else if(is_hdr_param(c))
    {
        // 标头下又是一个标头
        psr->state = ZEUS_PSRSTAT_HDR_KEY;
        zeus_header *hdr = (zeus_header*)zeus_calloc(1, sizeof(zeus_header));
        hdr->key = current;
        hdr->key_size++;
        packet->last_hdr->next = hdr;
        packet->last_hdr = hdr;
    }
    else
    {
        // ignore..
        zprintf(ZTEXT("hdr end state, ignore:%c"), c);
    }
}

static int event_body(const char *current, zeus_parser *psr)
{
    zeus_packet *packet = psr->packet;
    if(packet->body == NULL)
    {
        packet->body = current;
    }

    packet->body_size++;

    if(packet->body_size == psr->packet->content_length)
    {
        return ZERR_OK;
    }

    return ZERR_NEED_MORE;
}

int zeus_psrse_packet(zeus_parser *psr)
{
    zeus_packet *packet = psr->packet;
    int start = psr->psr_offset;
    int end = psr->datasize;

    for(int i = start; i < end; i++)
    {
        char *current = psr->raw + psr->psr_offset;

        switch (psr->state)
        {
            case ZEUS_PSRSTAT_GROUND:
                {
                    event_ground(current, psr);
                    break;
                }

            case ZEUS_PSRSTAT_METHOD:
                {
                    event_method(current, psr);
                    break;
                }

            case ZEUS_PSRSTAT_URL:
                {
                    event_url(current, psr);
                    break;
                }

            case ZEUS_PSRSTAT_URL_PARAM:
                {
                    event_url_param(current, psr);
                    break;
                }

            case ZEUS_PSRSTAT_VER:
                {
                    event_version(current, psr);
                    break;
                }

            case ZEUS_PSRSTAT_VER_PARAM:
                {
                    event_ver_param(current, psr);
                    break;
                }

            case ZEUS_PSRSTAT_HDR_KEY:
                {
                    event_hdr_key(current, psr);
                    break;
                }

            case ZEUS_PSRSTAT_HDR_VAL:
                {
                    event_hdr_val(current, psr);
                    break;
                }

            case ZEUS_PSRSTAT_HDR_END:
                {
                    event_hdr_end(current, psr);
                    break;
                }

            case ZEUS_PSRSTAT_BODY:
                {
                    int code = event_body(current, psr);
                    if(code == ZERR_OK)
                    {
                        zprintf(ZTEXT("http packet解析完毕..."));
                        return code;
                    }
                    break;
                }

            default:
                break;
        }

        psr->psr_offset++;
    }

    return ZERR_NEED_MORE;
}