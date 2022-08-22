#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "zeus_errno.h"
#include "zeus_log.h"
#include "zeus_config.h"
#include "zeus_event_mgr.h"
#include "zeus_os.h"

extern struct zeus_event_mgr_actions_s zeus_event_driver_select;
static struct zeus_event_mgr_actions_s *zeus_event_driver_list[] =
{
	&zeus_event_driver_select,
};

zeus_event_mgr *new_event_mgr(zeus_config *config)
{
	zeus_event_mgr *mgr = (zeus_event_mgr *)zeus_calloc(1, sizeof(zeus_event_mgr));

	// 根据mode找到对应的IO模型
	int num_actions = sizeof(zeus_event_driver_list) / sizeof(struct zeus_event_driver_operation_s *);
	for(int i = 0; i < num_actions; i++)
	{
		zeus_event_mgr_actions *actions = zeus_event_driver_list[i];
		if(actions->type == config->event_driver_type)
		{
			mgr->actions = actions;
			break;
		}
	}

	if(mgr->actions == NULL)
	{
		zloge(ZTEXT("find event_mgr_actions failed, mode = %d"), config->event_action_type);
		return NULL;
	}

	// 初始化
	int code = mgr->actions->initialize(mgr);
	if(code != ZERR_OK)
	{
		zloge(ZTEXT("initialize event mgr actions failed, %d"), code);
		return NULL;
	}

	return mgr;
}

void free_event_mgr(zeus_event_mgr *mgr)
{
	free(mgr);
}

int zeus_event_mgr_add_event(zeus_event_mgr *mgr, zeus_event *evt)
{
	mgr->actions->add_event(mgr, evt);
}

int zeus_event_mgr_remove_event(zeus_event_mgr *mgr, zeus_event *evt)
{
	mgr->actions->remove_event(mgr, evt);
}

