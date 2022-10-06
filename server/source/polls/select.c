#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(Y_WIN32)) || (defined(Y_MINGW))
#include <Windows.h>
#include <WinSock2.h>
#endif

#include "errors.h"
#include "blog.h"

typedef struct select_poll_actions_s
{
    FD_SET master_read_fd_set;
    FD_SET master_write_fd_set;
    FD_SET work_read_fd_set;
    FD_SET work_write_fd_set;
    FD_SET work_except_fd_set;
    int max_read;
    int max_write;
    int nevents;
}select_poll_actions;

int init_select_poll_actions(cJSON *config, void **ctx)
{
    select_poll_actions *select_actions = (select_poll_actions*)Ycalloc(1, sizeof(select_poll_actions));

    FD_ZERO(&select_actions->master_read_fd_set);
    FD_ZERO(&select_actions->master_write_fd_set);
    select_actions->nevents = 0;
    select_actions->max_read = 0;
    select_actions->max_write = 0;

    *ctx = select_actions;

    return BLOG_ERR_SUCCESS;
}

int select_poll_actions_add_event(event_poll *poll, event *evt)
{
    select_poll_actions *select_actions = (select_poll_actions*)poll->pollctx;

    if(evt->read)
    {
        FD_SET(evt->fd, &select_actions->master_read_fd_set);
        select_actions->max_read++;
    }

    if(evt->write)
    {
        FD_SET(evt->fd, &select_actions->master_write_fd_set);
        select_actions->max_write++;
    }

    return BLOG_ERR_SUCCESS;
}

int select_poll_actions_del_event(event_poll *poll, event *evt)
{
    select_poll_actions *select_actions = (select_poll_actions*)poll->pollctx;

    if(evt->read)
    {
        FD_CLR(evt->fd, &select_actions->master_read_fd_set);
        select_actions->max_read--;
    }

    if(evt->write)
    {
        FD_CLR(evt->fd, &select_actions->master_write_fd_set);
        select_actions->max_write--;
    }

    return BLOG_ERR_SUCCESS;
}

int select_poll_actions_poll(event_poll *poll)
{
    return BLOG_ERR_SUCCESS;
}

event_poll_actions poll_actions_select = 
{
    .name = "select",
    .init = init_select_poll_actions,
    .add_event = select_poll_actions_add_event,
    .del_event = select_poll_actions_del_event,
    .poll = select_poll_actions_poll
};