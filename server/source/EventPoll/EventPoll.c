#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "DefaultOptions.h"
#include "EventPoll.h"
#include "EventPollTypes.h"

extern EventPollActions SelectEventPollActions;
EventPollActions *EventPollActionsList[] = 
{
    &SelectEventPollActions
};

static EventPollActions *SelectActions(EventPollOptions *options)
{
    size_t len = sizeof(FDEventPollActionsList) / sizeof(FDEventPollActions*);

    for (size_t i = 0; i < len; i++)
    {
        FDEventPollActions *actions = FDEventPollActionsList[i];
        if(actions->Type == options->Type)
        {
            return actions;
        }
    }

    return NULL;
}


EventPoll *EventPollCreate(EventPollOptions *options)
{
    FDEventPoll *monitor = (FDEventPoll*)calloc(1, sizeof(FDEventPoll));
    monitor->FDQueue = Y_create_queue();
    monitor->SignaledQueue = Y_create_queue();
    monitor->FileDescriptorPool = Y_create_pool(sizeof(FileDescriptor), MAX_FD_COUNT);
    monitor->Options = options;
    monitor->Actions = SelectActions(options);
    monitor->Actions->Initialize(monitor);
    return monitor;
}

int EventPollAddEvent(EventPoll *evpoll, Event *evt)
{
}

int EventPollDelEvent(EventPoll *evpoll, int fd)
{

}

int EventPollPoll(EventPoll *evpoll)
{

}

Event *EventPollCreateEvent(EventPoll *evpoll)
{
}

void EventPollRelease(EventPoll *evpoll)
{

}



