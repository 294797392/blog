#ifndef __ZEUS_H__
#define __ZEUS_H__

#ifdef ZEUS_WIN32
#include <Windows.h>
#endif
#include <stdint.h>

#include "zeus_errno.h"
#include "zeus_config.h"
#include "zeus_worker.h"

typedef struct zeus_conn_s
{
    SOCKET fd;

    uint64_t timestamp;

}zeus_conn;

typedef struct zeus_s
{
    zeus_config *config;

    SOCKET fd;

    zeus_worker **workers;
    int num_worker;

}zeus;

#endif