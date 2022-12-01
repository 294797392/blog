#ifndef __HTTPCONSTANTS_H__
#define __HTTPCONSTANTS_H__

    typedef enum
    {
        HTTP_VERSION_0,
        HTTP_VERSION_1,
        HTTP_VERSION_1_1,
        HTTP_VERSION_2,
    }HttpVersions;

    typedef enum
    {
        HTTP_METHOD_UNKOWN,
        HTTP_METHOD_GET,
        HTTP_METHOD_HEAD,
        HTTP_METHOD_PUT,
        HTTP_METHOD_POST,
        HTTP_METHOD_TRACE,
        HTTP_METHOD_OPTIONS,
        HTTP_METHOD_DELETE,
    }HttpMethods;

    #define HTTP_HEAD_CONTENT_LENGTH                "content-length"

#endif