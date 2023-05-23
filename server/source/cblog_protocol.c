#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cblog_protocol.h"

typedef struct enum_string_s
{
	int enum_value;
	char *string;
}enum_string;

static enum_string cblog_http_method_list[] =
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

static enum_string cblog_http_version_list[] =
{
	{.enum_value = CBLOG_HTTP_VERSION_1,		.string = ""		},
	{.enum_value = CBLOG_HTTP_VERSION_1DOT1,	.string = "HTTP/1.1"},
	{.enum_value = -1,							.string = NULL		}
};

static enum_string cblog_http_status_code_list[] =
{
	{.enum_value = CBLOG_HTTP_STATUS_CODE_OK,				.string = "OK"},
	{.enum_value = CBLOG_HTTP_STATUS_NOT_FOUND,				.string = "Not Found"},
	{.enum_value = -1,										.string = NULL}
};

static char *cblog_http_header_list[] =
{
	CBLOG_HTTP_HEADER_STRING_CONTENT_LENGTH
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

static const char *enum2string(int value, enum_string *ess)
{
	for(int i = 0;; i++)
	{
		enum_string *es = &ess[i];

		if(es->enum_value == -1)
		{
			return NULL;
		}

		if(es->enum_value == value)
		{
			return es->string;
		}
	}

	return NULL;
}


int cblog_http_method_string2enum(cblog_string *str)
{
	return string2enum(str, cblog_http_method_list);
}

int cblog_http_version_string2enum(cblog_string *str)
{
	return string2enum(str, cblog_http_version_list);
}

const char *cblog_http_status_code_string(cblog_http_status_code_enum status_code)
{
	return enum2string(status_code, cblog_http_status_code_list);
}

const char *cblog_http_version_string(cblog_http_version_enum version)
{
	return enum2string(version, cblog_http_version_list);
}

const char *cblog_http_header_string(cblog_http_header_enum header)
{
	return cblog_http_header_list[header];
}