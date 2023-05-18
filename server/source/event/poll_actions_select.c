#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#endif

#include <libY.h>

#include "errors.h"
#include "event.h"

typedef struct poll_select_s
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
}poll_select;

int eventpoll_select_initialize(event_module *evm)
{
	poll_select *pollselect = (poll_select *)calloc(1, sizeof(poll_select));
	if(pollselect == NULL)
	{
		YLOGE("cannot create poll_select instance, no memory");
		return STEAK_ERR_NO_MEM;
	}

	evm->actions_data = pollselect;

	// 初始化要监控的文件描述符列表
	FD_ZERO(&pollselect->master_read_fd_set);
	FD_ZERO(&pollselect->master_write_fd_set);

	// 设置超时时间
	pollselect->timeout.tv_sec = 0;
	pollselect->timeout.tv_usec = evm->timeout_ms * 1000;

	return STEAK_ERR_OK;
}

void eventpoll_select_release(event_module *evm)
{

}

int eventpoll_select_add_event(event_module *evm, steak_event *evt)
{
	poll_select *pollselect = (poll_select *)evm->actions_data;

	if(evt->read)
	{
		FD_SET(evt->sock, &pollselect->master_read_fd_set);
		pollselect->max_read++;
	}

	if(evt->write)
	{
		FD_SET(evt->sock, &pollselect->master_write_fd_set);
		pollselect->max_write++;
	}

	if(evt->sock > pollselect->max_fd)
	{
		pollselect->max_fd = evt->sock;
	}

	return STEAK_ERR_OK;
}

int eventpoll_select_delete_event(event_module *evm, steak_event *evt)
{
	poll_select *pollselect = (poll_select *)evm->actions_data;

	if(evt->read)
	{
		FD_CLR(evt->sock, &pollselect->master_read_fd_set);
		pollselect->max_read--;
	}

	if(evt->write)
	{
		FD_CLR(evt->sock, &pollselect->master_write_fd_set);
		pollselect->max_write--;
	}

	return STEAK_ERR_OK;
}

int eventpoll_select_modify_event(event_module *evm, steak_event *evt, int read, int write)
{
	poll_select *pollselect = (poll_select *)evm->actions_data;

	if(read)
	{
		if(!FD_ISSET(evt->sock, &pollselect->master_read_fd_set))
		{
			FD_SET(evt->sock, &pollselect->master_read_fd_set);
			pollselect->max_read++;
		}
	}
	else
	{
		FD_CLR(evt->sock, &pollselect->master_read_fd_set);
		pollselect->max_read--;
	}

	if(write)
	{
		if(!FD_ISSET(evt->sock, &pollselect->master_write_fd_set))
		{
			FD_SET(evt->sock, &pollselect->master_write_fd_set);
			pollselect->max_write++;
		}
	}
	else
	{
		FD_CLR(evt->sock, &pollselect->master_write_fd_set);
		pollselect->max_write--;
	}

	return STEAK_ERR_OK;
}

int eventpoll_select_poll_event(event_module *evm, steak_event **events, int nevent)
{
	poll_select *pollselect = (poll_select *)evm->actions_data;

	FD_SET readfds = pollselect->master_read_fd_set;
	FD_SET writefds = pollselect->master_write_fd_set;

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	int ret = select(0, &readfds, &writefds, NULL, &pollselect->timeout);
#elif (defined(ENV_UNIX))
	int ret = select(pollselect->max_fd + 1, &readfds, &writefds, NULL, &pollselect->timeout);
#endif
	if(ret == 0)
	{
		// timeout
		return STEAK_ERR_OK;
	}
	else if(ret < 0)
	{
		// error
		switch(errno)
		{
			case EINTR:
			{
				// 等待时捕获了一个信号，可以重新发起调用
				return eventpoll_select_poll_event(evm, events, nevent);
			}

			default:
			{
				YLOGE("select failed, %s", strerror(errno));
				return STEAK_ERR_OK;
			}
		}
	}
	else
	{
		// one or more fd has io event

		for(int i = 0; i < nevent; i++)
		{
			steak_event *evt = events[i];

			int got_signal = 0;

			if(FD_ISSET(evt->sock, &readfds))
			{
				evt->sigread = 1;
				got_signal = 1;
			}

			if(FD_ISSET(evt->sock, &writefds))
			{
				evt->sigwrite = 1;
				got_signal = 1;
			}

			if(got_signal)
			{
				// 添加到处理列表里
				Y_list_add(evm->process_event_list, evt);
			}
		}
	}

	return STEAK_ERR_OK;
}

eventpoll_actions eventpoll_actions_select =
{
	.type = EVENT_POLL_TYPE_SELECT,
	.initialize = eventpoll_select_initialize,
	.release = eventpoll_select_release,
	.add_event = eventpoll_select_add_event,
	.delete_event = eventpoll_select_delete_event,
	.modify_event = eventpoll_select_modify_event,
	.poll_event = eventpoll_select_poll_event
};