#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "zeus_os.h"
#include "zeus_event.h"

struct zeus_event_s
{
	zfd fd;											// 事件所关联的文件描述符
	zeus_event_handler handler;						// 当该事件触发后，所执行的动作
	
	void *data;										// 事件所带的数据，如果是监听socket，那么就是zeus_svc，如果是客户端socket，那么就是zeus_client

	int write;										// 该事件是否可读
	int read;										// 该事件是否可写
};

zeus_event *new_zeus_event(zfd fd, zeus_event_handler handler)
{
	zeus_event *event = (zeus_event *)zeus_calloc(1, sizeof(zeus_event));
	event->fd = fd;
	event->handler = handler;
	return event;
}

void free_zeus_event(zeus_event *evt)
{
	zeus_free(evt);
}




