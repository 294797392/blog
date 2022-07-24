#include <stdlib.h>
#include <stdio.h>

#ifdef ZEUS_WIN32
#include <Windows.h>
#endif

#include "zeus_worker.h"
#include "zeus.h"

void zeus_worker_main(void *userdata)
{
    zeus_worker *zw = (zeus_worker*)userdata;

    while(1)
    {
        WaitForSingleObject(zw->event, INFINITE);

        
    }
}

zeus_worker *select_idle_worker(zeus *z)
{
    for(int i = 0; i < z->num_worker; i++)
    {
        zeus_worker *zw = z->workers[i];
        if(zw->state == ZW_STATE_IDLE)
        {
            return zw;
        }
    }

    return NULL;
}

