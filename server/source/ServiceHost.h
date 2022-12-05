#ifndef __SERVICE_HOST_H__
#define __SERVICE_HOST_H__

#include <libY.h>

#include "FDEventPoll.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct ServiceHost ServiceHost;
    typedef struct ServiceHostOptions
    {
        char *BindAddress;
        int ListenPort;
        char *RootDir;
        FDEventPoll *FDEventPoll;
    }ServiceHostOptions;

    ServiceHost *ServiceHostOpen(ServiceHostOptions *options);
    void ServiceHostClose(ServiceHost *svchost);

#ifdef __cplusplus
}
#endif


#endif