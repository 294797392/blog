#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <libY.h>

#include "cblog_string.h"
#include "cblog_buffer.h"

int cblog_string_casecmp(cblog_string *str1, const char *str2)
{
	// 先判断字符串长度
	if(str1->length != strlen(str2))
	{
		return 1;
	}

	// 再逐个比较字符
	for(int i = 0; i < str1->length; i++)
	{
		char a = tolower(cblog_string_charat(str1, i));
		char b = tolower(str2[i]);

		if(a != b)
		{
			return 1;
		}
	}

	return 0;
}

int cblog_string_casecmp2(char *str1, const char *str2, int ncount)
{
	// 再逐个比较字符
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



void cblog_string_print(cblog_string *str)
{
	char format[64] = { '\0' };
	snprintf(format, sizeof(format), "%%.%ds\r\n", str->length);
	printf(format, cblog_string_string(str));
}

void cblog_string_print2(char *prepend, cblog_string *str)
{
	printf(prepend);
	cblog_string_print(str);
}

int cblog_string_to_int32(cblog_string *str)
{
	char buffer[32] = { '\0' };
	memset(buffer, '\0', sizeof(buffer));
	strncpy(buffer, str->buffer->pdata + str->offset, str->length);
	return atoi(buffer);
}

void cblog_string_copy(cblog_string *str, char *buf, size_t bufsize)
{
	memset(buf, '\0', bufsize);
	strncpy(buf, str->buffer->pdata + str->offset, str->length);
}