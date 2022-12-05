#ifndef __FDMONITOR_TYPES_H__
#define __FDMONITOR_TYPES_H__

typedef enum
{
    FDMON_TYPE_SELECT,
    FDMON_TYPE_POLL,
    FDMON_TYPE_EPOLL,
    FDMON_TYPE_IOCP,
    FDMON_TYPE_KPOLL
}FDEventPollTypes;

#endif