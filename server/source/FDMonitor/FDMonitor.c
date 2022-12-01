#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "DefaultOptions.h"
#include "FDMonitor.h"
#include "FDMonitorTypes.h"

extern FDMonitorActions FDMonitorActionsSelect;
FDMonitorActions **FDMonitorActionsList = 
{
    &FDMonitorActionsSelect
};

static FDMonitorActions *SelectActions(FDMonitorOptions *options)
{
    size_t len = sizeof(FDMonitorActionsList) / sizeof(FDMonitorActions*);

    for (size_t i = 0; i < len; i++)
    {
        FDMonitorActions *actions = FDMonitorActionsList[i];
        if(actions->Type == options->Type)
        {
            return actions;
        }
    }

    return NULL;
}


FDMonitor *FDMonitorCreate(FDMonitorOptions *options)
{
    FDMonitor *monitor = (FDMonitor*)calloc(1, sizeof(FDMonitor));
    monitor->FDQueue = Y_create_queue();
    monitor->SignaledQueue = Y_create_queue();
    monitor->FileDescriptorPool = Y_create_pool(sizeof(FileDescriptor), MAX_FD_COUNT);
    monitor->Options = options;
    monitor->Actions = SelectActions(options);
    monitor->ActionsData = monitor->Actions->Initialize(monitor);
    return monitor;
}

int FDMonitorAddFD(FDMonitor *monitor, FileDescriptor *fd)
{
}

int FDMonitorDelFD(FDMonitor *monitor, int fd)
{}

int FDMonitorPoll(FDMonitor *monitor)
{

}

FileDescriptor *FDMonitorCreateFD(FDMonitor *monitor)
{

}

void FDMonitorRelease(FDMonitor *monitor)
{

}



