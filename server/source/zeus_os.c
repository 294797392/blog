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

void *zeus_calloc(int count, int size)
{
    void *mem = calloc(count, size);
    if(mem == NULL)
    {
        perror("应用程序发生致命错误");
    }
    return mem;
}

void zeus_free(void *mem)
{
    free(mem);
}
