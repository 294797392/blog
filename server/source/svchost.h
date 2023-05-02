#ifndef __SERVICE_HOST_H__
#define __SERVICE_HOST_H__

#include <libY.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct svchost_s svchost;
    typedef struct svchost_options_s svchost_options;

    struct svchost_options_s
    {
        char *bindaddr;
        int port;
        char *root;
    };

    struct svchost_s
    {
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
        SOCKET sock;
#elif (defined(ENV_UNIX))
        int sock;
#endif

        svchost_options *options;
    };

    svchost *new_svchost(svchost_options *options);
    void free_svchost(svchost *svc);

#ifdef __cplusplus
}
#endif


#endif