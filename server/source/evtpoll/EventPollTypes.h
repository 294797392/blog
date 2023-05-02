#ifndef __FDMONITOR_TYPES_H__
#define __FDMONITOR_TYPES_H__

typedef enum
{
    EVENT_POLL_TYPE_SELECT,
    EVENT_POLL_TYPE_POLL,
    EVENT_POLL_TYPE_EPOLL,
    EVENT_POLL_TYPE_IOCP,
    EVENT_POLL_TYPE_KPOLL
}EventPollTypes;

#endif