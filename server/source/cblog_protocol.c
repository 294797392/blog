#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cblog_protocol.h"

typedef struct enum_string_s
{
	int enum_value;
	char *string;
}enum_string;

enum_string cblog_http_method_list[] =
{
	{.enum_value = STEAK_HTTP_METHOD_GET,		.string = "GET"		},
	{.enum_value = STEAK_HTTP_METHOD_POST,		.string = "POST"	},
	{.enum_value = STEAK_HTTP_METHOD_HEAD,		.string = "HEAD"	},
	{.enum_value = STEAK_HTTP_METHOD_PUT,		.string = "PUT"		},
	{.enum_value = STEAK_HTTP_METHOD_TRACE,		.string = "TRACE"	},
	{.enum_value = STEAK_HTTP_METHOD_OPTIONS,	.string = "OPTIONS" },
	{.enum_value = STEAK_HTTP_METHOD_DELETE,	.string = "DELETE"	},
	{.enum_value = -1,							.string = NULL		}
};

enum_string cblog_http_version_list[] = 
{
	{.enum_value = CBLOG_HTTP_VERSION_1,		.string = ""		},
	{.enum_value = CBLOG_HTTP_VERSION_1DOT1,	.string = "HTTP/1.1"},
	{.enum_value = -1,							.string = NULL		}
};

static int string2enum(cblog_string *str, enum_string *tvs)
{
	for(int i = 0; ; i++)
	{
		enum_string *tv = &tvs[i];

		if(tv->string == NULL)
		{
			return 0;
		}

		if(!cblog_string_casecmp(str, tv->string))
		{
			return tv->enum_value;
		}
	}

	return 0;
}


int cblog_http_method_string2enum(cblog_string *str)
{
	return string2enum(str, cblog_http_method_list);
}

int cblog_http_version_string2enum(cblog_string *str)
{
	return string2enum(str, cblog_http_version_list);
}
