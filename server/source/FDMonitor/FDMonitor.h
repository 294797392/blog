#ifndef __FDMONITOR_H__
#define __FDMONITOR_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#include "FDMonitorTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct FileDescriptor FileDescriptor;
    typedef struct FDMonitorActions FDMonitorActions;
    typedef struct FDMonitor FDMonitor;
    typedef struct FDMonitorOptions 
    {
        FDMonitorTypes Type;
    }FDMonitorOptions;

    struct FileDescriptor
    {
        #if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
        SOCKET Socket;
        #elif (defined(ENV_UNIX))
        int Socket;
        #endif

        int(*OnRead)(FDMonitor *monitor, FileDescriptor *fd);
        int(*OnWrite)(FDMonitor *monitor, FileDescriptor *fd);
        int(*OnExcept)(FDMonitor *monitor, FileDescriptor *fd);

        int Read;
        int Write;
    };

    struct FDMonitorActions
    {
        FDMonitorTypes Type;
        void*(*Initialize)(FDMonitor *monitor);
        void(*Release)(FDMonitor *monitor);
        int(*AddFD)(FDMonitor *monitor, FileDescriptor *fd);
        int(*DelFD)(FDMonitor *monitor, FileDescriptor *fd);
        int(*PollFD)(FDMonitor *monitor);
    };

    struct FDMonitor
    {
        FDMonitorOptions *Options;
        FDMonitorActions *Actions;
        void *ActionsData;

        // 文件描述符缓冲池
        Ypool *FileDescriptorPool;

        // 监控的文件描述符列表
        Yqueue *FDQueue;

        // 已经有信号的文件描述符队列
        // 队列里的文件描述符要么是可读的，要么是可写的
        Yqueue *SignaledQueue;
    };

    FDMonitor *FDMonitorCreate(FDMonitorOptions *options);
    int FDMonitorAddFD(FDMonitor *monitor, FileDescriptor *fd);
    int FDMonitorDelFD(FDMonitor *monitor, int fd);
    int FDMonitorPoll(FDMonitor *monitor);
    FileDescriptor *FDMonitorCreateFD(FDMonitor *monitor);
    void FDMonitorRelease(FDMonitor *monitor);

#ifdef __cplusplus
}
#endif

#endif