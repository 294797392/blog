#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <Windows.h>
#include <WinSock2.h>
#endif

#include "ResponseCode.h"
#include "FDMonitor.h"

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

int SelectFDMonitorActionsInitialize(FDMonitor *monitor)
{
    SelectFDMonitor *selectMonitor = (SelectFDMonitor*)calloc(1, sizeof(SelectFDMonitor));
    monitor->ActionsData = selectMonitor;

    FD_ZERO(&selectMonitor->master_read_fd_set);
    FD_ZERO(&selectMonitor->master_write_fd_set);

    return CODE_SUCCESS;
}

void SelectFDMonitorActionsRelease(FDMonitor *monitor)
{

}

int SelectFDMonitorActionsAddFD(FDMonitor *monitor, FileDescriptor *fd)
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

int SelectFDMonitorActionsDelFD(FDMonitor *monitor, FileDescriptor *fd)
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

int SelectFDMonitorActionsPollFDs(FDMonitor *monitor)
{
    SelectFDMonitor *selectMonitor = (SelectFDMonitor*)monitor->ActionsData;

    return CODE_SUCCESS;
}

FDMonitorActions SelectFDMonitorActions = 
{
    .Type = FDMON_TYPE_SELECT,
    .Initialize = SelectFDMonitorActionsInitialize,
    .Release = SelectFDMonitorActionsRelease,
    .AddFD = SelectFDMonitorActionsAddFD,
    .DelFD = SelectFDMonitorActionsDelFD,
    .PollFDs = SelectFDMonitorActionsPollFDs
};