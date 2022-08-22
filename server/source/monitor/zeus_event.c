#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "zeus_os.h"
#include "zeus_event.h"

struct zeus_event_s
{
	zfd fd;											// �¼����������ļ�������
	zeus_event_handler handler;						// �����¼���������ִ�еĶ���
	
	void *data;										// �¼����������ݣ�����Ǽ���socket����ô����zeus_svc������ǿͻ���socket����ô����zeus_client

	int write;										// ���¼��Ƿ�ɶ�
	int read;										// ���¼��Ƿ��д
	
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




