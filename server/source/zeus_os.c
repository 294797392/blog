#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef ZEUS_WIN32
#include <Windows.h>
#endif

int zeus_os_error()
{
    #ifdef ZEUS_WIN32
    return GetLastError();
    #endif
}

