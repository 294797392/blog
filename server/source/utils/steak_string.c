#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <libY.h>

#include "steak_string.h"

int strcasecmp(const char *str1, const char *str2, int ncount)
{
	for(int i = 0; i < ncount; i++)
	{
		char a = tolower(str1[i]);
		char b = tolower(str2[i]);

		if(a != b)
		{
			return 1;
		}
	}

	return 0;
}
