#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "DefaultOptions.h"
#include "FDEventPoll.h"
#include "FDEventPollTypes.h"

extern FDEventPollActions SelectFDEventPollActions;
FDEventPollActions *FDEventPollActionsList[] = 
{
    &SelectFDEventPollActions
};

static FDEventPollActions *SelectActions(FDEventPollOptions *options)
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


FDEventPoll *FDEventPollCreate(FDEventPollOptions *options)
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

int FDEventPollAddFD(FDEventPoll *monitor, FileDescriptor *fd)
{
}

int FDEventPollDelFD(FDEventPoll *monitor, int fd)
{}

int FDEventPollPoll(FDEventPoll *monitor)
{

}

FileDescriptor *FDEventPollCreateFD(FDEventPoll *monitor)
{

}

void FDEventPollRelease(FDEventPoll *monitor)
{

}



