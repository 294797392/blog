#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#endif

#include "errors.h"
#include "eventpoll.h"

typedef struct eventpoll_select_s
{
    FD_SET master_read_fd_set;
    FD_SET master_write_fd_set;
    FD_SET work_read_fd_set;
    FD_SET work_write_fd_set;
    FD_SET work_except_fd_set;
    int max_read;
    int max_write;
    int nevents;
}eventpoll_select;

int eventpoll_select_initialize(eventpoll *evtpoll)
{
    eventpoll_select *evtpoll_select = (eventpoll_select *)calloc(1, sizeof(eventpoll_select));
    evtpoll->private_data = evtpoll_select;

    FD_ZERO(&evtpoll_select->master_read_fd_set);
    FD_ZERO(&evtpoll_select->master_write_fd_set);

    return ERR_SUCCESS;
}

void eventpoll_select_release(eventpoll *evtpoll)
{

}

int eventpoll_select_add_event(eventpoll *evtpoll, eventpoll_event *evt)
{
    eventpoll_select *evtpoll_select = (eventpoll_select *)evtpoll->private_data;

    if(evt->readable)
    {
        FD_SET(evt->sock, &evtpoll_select->master_read_fd_set);
        evtpoll_select->max_read++;
    }

    if(evt->writeable)
    {
        FD_SET(evt->sock, &evtpoll_select->master_write_fd_set);
        evtpoll_select->max_write++;
    }

    return ERR_SUCCESS;
}

int eventpoll_select_delete_event(eventpoll *evtpoll, eventpoll_event *evt)
{
    eventpoll_select *evtpoll_select = (eventpoll_select *)evtpoll->private_data;

    if(evt->readable)
    {
        FD_CLR(evt->sock, &evtpoll_select->master_read_fd_set);
        evtpoll_select->max_read--;
    }

    if(evt->writeable)
    {
        FD_CLR(evt->sock, &evtpoll_select->master_write_fd_set);
        evtpoll_select->max_write--;
    }

    return ERR_SUCCESS;
}

int eventpoll_select_poll_event(eventpoll *evtpoll)
{
    eventpoll_select *evtpoll_select = (eventpoll_select *)evtpoll->private_data;


    return ERR_SUCCESS;
}

eventpoll_actions eventpoll_actions_select =
{
    .type = EVENT_POLL_TYPE_SELECT,
    .initialize = eventpoll_select_initialize,
    .release = eventpoll_select_release,
    .add_event = eventpoll_select_add_event,
    .delete_event = eventpoll_select_delete_event,
    .poll_event = eventpoll_select_poll_event
};