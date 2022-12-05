#ifndef __FDEVENT_POLL_H__
#define __FDEVENT_POLL_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#include "FDEventPollTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct FileDescriptor FileDescriptor;
    typedef struct FDEventPollActions FDEventPollActions;
    typedef struct FDEventPoll FDEventPoll;
    typedef struct FDEventPollOptions 
    {
        FDEventPollTypes Type;
    }FDEventPollOptions;

    struct FileDescriptor
    {
        #if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
        SOCKET Socket;
        #elif (defined(ENV_UNIX))
        int Socket;
        #endif
        
        int(*OnRead)(FDEventPoll *evpoll, FileDescriptor *fd);
        int(*OnWrite)(FDEventPoll *evpoll, FileDescriptor *fd);
        int(*OnExcept)(FDEventPoll *evpoll, FileDescriptor *fd);

        int Read;
        int Write;
    };

    struct FDEventPollActions
    {
        FDEventPollTypes Type;
        int(*Initialize)(FDEventPoll *evpoll);
        void(*Release)(FDEventPoll *evpoll);
        int(*AddFD)(FDEventPoll *evpoll, FileDescriptor *fd);
        int(*DelFD)(FDEventPoll *evpoll, FileDescriptor *fd);
        int(*PollFDs)(FDEventPoll *evpoll);
    };

    struct FDEventPoll
    {
        FDEventPollOptions *Options;
        FDEventPollActions *Actions;
        void *ActionsData;

        // 文件描述符缓冲池
        Ypool *FileDescriptorPool;

        // 监控的文件描述符列表
        Yqueue *FDQueue;

        // 已经有信号的文件描述符队列
        // 队列里的文件描述符要么是可读的，要么是可写的
        Yqueue *SignaledQueue;
    };

    FDEventPoll *FDEventPollCreate(FDEventPollOptions *options);
    int FDEventPollAddFD(FDEventPoll *evpoll, FileDescriptor *fd);
    int FDEventPollDelFD(FDEventPoll *evpoll, int fd);
    int FDEventPollPoll(FDEventPoll *evpoll);
    FileDescriptor *FDEventPollCreateFD(FDEventPoll *evpoll);
    void FDEventPollRelease(FDEventPoll *evpoll);

#ifdef __cplusplus
}
#endif

#endif