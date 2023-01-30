#ifndef __EVENT_POLL_H__
#define __EVENT_POLL_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#include "EventPollTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct Event Event;
    typedef struct EventPollActions EventPollActions;
    typedef struct EventPoll EventPoll;
    typedef struct EventPollOptions EventPollOptions;

    struct EventPollOptions
    {
        EventPollTypes Type;
    };

    struct Event
    {
        #if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
        SOCKET Socket;
        #elif (defined(ENV_UNIX))
        int Socket;
        #endif

        int(*OnRead)(EventPoll *evpoll, Event *evt);
        int(*OnWrite)(EventPoll *evpoll, Event *evt);
        int(*OnExcept)(EventPoll *evpoll, Event *evt);

        int Read;
        int Write;
    };

    struct EventPollActions
    {
        EventPollTypes Type;
        int(*Initialize)(EventPoll *evpoll);
        void(*Release)(EventPoll *evpoll);
        int(*AddFD)(EventPoll *evpoll, Event *evt);
        int(*DelFD)(EventPoll *evpoll, Event *evt);
        int(*PollFDs)(EventPoll *evpoll);
    };

    struct EventPoll
    {
        EventPollOptions *Options;
        EventPollActions *Actions;
        void *ActionsData;

        // 文件描述符缓冲池
        Ypool *FileDescriptorPool;

        // 监控的文件描述符列表
        Yqueue *FDQueue;

        // 已经有信号的文件描述符队列
        // 队列里的文件描述符要么是可读的，要么是可写的
        Yqueue *SignaledQueue;
    };

    EventPoll *EventPollCreate(EventPollOptions *options);
    int EventPollAddEvent(EventPoll *evpoll, Event *evt);
    int EventPollDelEvent(EventPoll *evpoll, int fd);
    int EventPollPoll(EventPoll *evpoll);
    Event *EventPollCreateEvent(EventPoll *evpoll);
    void EventPollRelease(EventPoll *evpoll);

#ifdef __cplusplus
}
#endif

#endif