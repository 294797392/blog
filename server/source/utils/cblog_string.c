#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <libY.h>

#include "cblog_string.h"

int cblog_string_casecmp(const char *str1, const char *str2, int ncount)
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

void cblog_string_print(char *str, cblog_string *cbstr)
{
	char format[64] = { '\0' };
	snprintf(format, sizeof(format), "%%.%ds\r\n", cbstr->length);
	printf(format, str + cbstr->offset);
}

void cblog_string_print2(char *prepend, char *str, cblog_string *cbstr)
{
	printf(prepend);
	cblog_string_print(str, cbstr);
}

void cblog_string_print_header(char *str, cblog_http_header *header)
{
	char format[64] = { '\0' };
	snprintf(format, sizeof(format), "%%.%ds: %%.%ds\r\n", header->key.length, header->value.length);
	printf(format, str + header->key.offset, str + header->value.offset);
}