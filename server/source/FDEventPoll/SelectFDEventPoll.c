#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#endif

#include "ResponseCode.h"
#include "FDEventPoll.h"

typedef struct SelectFDMonitor
{
    FD_SET master_read_fd_set;
    FD_SET master_write_fd_set;
    FD_SET work_read_fd_set;
    FD_SET work_write_fd_set;
    FD_SET work_except_fd_set;
    int max_read;
    int max_write;
    int nevents;
}SelectFDMonitor;

int SelectFDMonitorActionsInitialize(FDEventPoll *monitor)
{
    SelectFDMonitor *selectMonitor = (SelectFDMonitor*)calloc(1, sizeof(SelectFDMonitor));
    monitor->ActionsData = selectMonitor;

    FD_ZERO(&selectMonitor->master_read_fd_set);
    FD_ZERO(&selectMonitor->master_write_fd_set);

    return CODE_SUCCESS;
}

void SelectFDMonitorActionsRelease(FDEventPoll *monitor)
{

}

int SelectFDMonitorActionsAddFD(FDEventPoll *monitor, FileDescriptor *fd)
{
    SelectFDMonitor *selectMonitor = (SelectFDMonitor*)monitor->ActionsData;

    if(fd->Read)
    {
        FD_SET(fd->Socket, &selectMonitor->master_read_fd_set);
        selectMonitor->max_read++;
    }

    if(fd->Write)
    {
        FD_SET(fd->Socket, &selectMonitor->master_write_fd_set);
        selectMonitor->max_write++;
    }

    return CODE_SUCCESS;
}

int SelectFDMonitorActionsDelFD(FDEventPoll *monitor, FileDescriptor *fd)
{
    SelectFDMonitor *selectMonitor = (SelectFDMonitor*)monitor->ActionsData;

    if(fd->Read)
    {
        FD_CLR(fd->Socket, &selectMonitor->master_read_fd_set);
        selectMonitor->max_read--;
    }

    if(fd->Write)
    {
        FD_CLR(fd->Socket, &selectMonitor->master_write_fd_set);
        selectMonitor->max_write--;
    }

    return CODE_SUCCESS;
}

int SelectFDMonitorActionsPollFDs(FDEventPoll *monitor)
{
    SelectFDMonitor *selectMonitor = (SelectFDMonitor*)monitor->ActionsData;

    return CODE_SUCCESS;
}

FDEventPollActions SelectFDEventPollActions = 
{
    .Type = FDMON_TYPE_SELECT,
    .Initialize = SelectFDMonitorActionsInitialize,
    .Release = SelectFDMonitorActionsRelease,
    .AddFD = SelectFDMonitorActionsAddFD,
    .DelFD = SelectFDMonitorActionsDelFD,
    .PollFDs = SelectFDMonitorActionsPollFDs
};