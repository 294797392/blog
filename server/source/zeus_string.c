
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "zeus_string.h"

void zprintf(zchar *format, ...)
{
    printf("%s\n", format);
    //wprintf(L"%s", format);
}

