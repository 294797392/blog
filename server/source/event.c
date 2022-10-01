#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>
#include <cJSON.h>
#include <cJSON_Helper.h>

#include "errors.h"
#include "default.h"
#include "blog.h"
#include "event.h"

extern event_poll_actions poll_actions_select;
extern event_poll_actions poll_actions_iocp;
extern event_poll_actions poll_actions_epoll;
static event_poll_actions *poll_actions[] = 
{
#ifdef POLL_SELECT
    &poll_actions_select,
#endif
#ifdef POLL_IOCP
    &poll_actions_iocp,
#endif
#ifdef POLL_EPOLL
    &poll_actions_epoll,
#endif
    NULL
};

event_poll *create_event_poll(cJSON *config)
{
    int rc                              =               0;
    char config_action[64]              =               {'\0'};
    event_poll_actions *actions         =               NULL;
    void *pollctx                       =               NULL;

    // 解析配置文件
    cJSON *event_poll_json = cJSON_GetObjectItem(config, "event_poll");
    cJSON_GetStringValue2(event_poll_json, "action", config_action, sizeof(config_action), DEF_POLL_ACTION);

    // 遍历event_poll_actions并选择合适的poll_actions
    size_t num_actions = sizeof(poll_actions) / sizeof(event_poll_actions*);
    for(size_t i = 0; i < num_actions; i++)
    {
        if(strncmp(poll_actions[i]->name, config_action, strlen(poll_actions[i]->name)) == 0)
        {
            actions = poll_actions[i];
            break;
        }
    }

    if(actions == NULL)
    {
        YLOGE(YTEXT("select poll_actions failed, %s"), YTEXT(config_action));
        return NULL;
    }

    YLOGI(YTEXT("use %s poll_actions"), YTEXT(config_action));

    if((rc = actions->init(event_poll_json, &pollctx)) != 0)
    {
        YLOGE(YTEXT("init_poll failed, %d"), rc);
        return NULL;
    }

    event_poll *poll = (event_poll*)Ycalloc(1, sizeof(event_poll));
    poll->actions = actions;
    poll->pollctx = pollctx;
    poll->event_list = Y_create_list();
    poll->event_queue = Y_create_queue();
    return poll;
}

int poll_events(event_poll *poll)
{
    return poll->actions->poll(poll);
}

int process_events(event_poll *poll)
{
}


