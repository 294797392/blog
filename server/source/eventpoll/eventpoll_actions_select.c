#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#endif

#include <libY.h>

#include "errors.h"
#include "eventpoll.h"

typedef struct eventpoll_select_s
{
	FD_SET master_read_fd_set;
	FD_SET master_write_fd_set;
	//FD_SET work_read_fd_set;
	//FD_SET work_write_fd_set;
	//FD_SET work_except_fd_set;
	int max_read;
	int max_write;
	int nevents;
	int max_fd;

	struct timeval timeout;
}eventpoll_select;

int eventpoll_select_initialize(eventpoll *evtpoll)
{
	eventpoll_select *evtpoll_select = (eventpoll_select *)calloc(1, sizeof(eventpoll_select));
	if(evtpoll_select == NULL)
	{
		YLOGE("cannot create eventpoll_select instance, no memory");
		return STEAK_ERR_NO_MEM;
	}

	evtpoll->actions_data = evtpoll_select;

	// 初始化要监控的文件描述符列表
	FD_ZERO(&evtpoll_select->master_read_fd_set);
	FD_ZERO(&evtpoll_select->master_write_fd_set);

	// 设置超时时间
	evtpoll_select->timeout.tv_sec = 0;
	evtpoll_select->timeout.tv_usec = evtpoll->timeout_ms * 1000;

	return ERR_SUCCESS;
}

void eventpoll_select_release(eventpoll *evtpoll)
{

}

int eventpoll_select_add_event(eventpoll *evtpoll, eventpoll_event *evt)
{
	eventpoll_select *evtpoll_select = (eventpoll_select *)evtpoll->actions_data;

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

	if(evt->sock > evtpoll_select->max_fd)
	{
		evtpoll_select->max_fd = evt->sock;
	}

	return ERR_SUCCESS;
}

int eventpoll_select_delete_event(eventpoll *evtpoll, eventpoll_event *evt)
{
	eventpoll_select *evtpoll_select = (eventpoll_select *)evtpoll->actions_data;

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
	eventpoll_select *evtpoll_select = (eventpoll_select *)evtpoll->actions_data;

	FD_SET worker_read_fd_set = evtpoll_select->master_read_fd_set;

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	int ret = select(0, &worker_read_fd_set, NULL, NULL, &evtpoll_select->timeout);
#elif (defined(ENV_UNIX))
	int ret = select(evtpoll_select->max_fd + 1, &worker_read_fd_set, NULL, NULL, &evtpoll_select->timeout);
#endif
	if(ret == 0)
	{
		// timeout
		return ERR_SUCCESS;
	}
	else if(ret < 0)
	{
		// error
		switch(errno)
		{
			case EINTR:
			{
				// 等待时捕获了一个信号，可以重新发起调用
				return eventpoll_select_poll_event(evtpoll);
			}

			default:
			{
				YLOGE("select failed, %s", strerror(errno));
				return ERR_SUCCESS;
			}
		}
	}
	else
	{
		// one or more fd has io event
		int events;
		eventpoll_event **event_list = (eventpoll_event **)Y_list_to_array(evtpoll->event_list, &events);

		for(int i = 0; i < events; i++)
		{
			eventpoll_event *evt = event_list[i];
			if(FD_ISSET(evt->sock, &worker_read_fd_set))
			{
				evt->on_read(evtpoll, evt);
			}
		}
	}

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