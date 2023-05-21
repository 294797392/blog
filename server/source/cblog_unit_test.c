#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cblog_unit_test.h"

static void http_parser_event_handler(cblog_parser *parser, cblog_parser_event_enum evt)
{
	switch(evt)
	{
		case CBLOG_PARSER_EVENT_METHOD:
		{
			break;
		}

		case CBLOG_PARSER_EVENT_URI:
		{
			break;
		}

		case CBLOG_PARSER_EVENT_VERSION:
		{
			break;
		}

		case CBLOG_PARSER_EVENT_HEADER:
		{
			break;
		}

		case CBLOG_PARSER_EVENT_BODY:
		{
			break;
		}

		default:
			break;
	}
}


void cblog_parser_unit_test()
{
	char *buf =
		"GET /favicon.ico HTTP/1.1\r\n"
		"Host: 127.0.0.1 : 1018\r\n"
		"Connection : keep - alive\r\n"
		"sec - ch - ua : \"Not_A Brand\"; v = \"99\", \"Microsoft Edge\"; v = \"109\", \"Chromium\"; v = \"109\"\r\n"
		"sec - ch - ua - mobile: ? 0\r\n"
		"User - Agent : Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 109.0.0.0 Safari / 537.36 Edg / 109.0.1518.52\r\n"
		"sec - ch - ua - platform : \"Windows\"\r\n"
		"Accept : image / webp, image / apng, image / svg + xml, image/*,*/ *; q = 0.8\r\n"
		"Sec - Fetch - Site: same - origin\r\n"
		"Sec - Fetch - Mode : no - cors\r\n"
		"Sec - Fetch - Dest : image\r\n"
		"Referer : http://127.0.0.1:1018/\r\n"
		"Accept - Encoding : gzip, deflate, br\r\n"
		"Accept - Language : zh - CN, zh; q = 0.9, en; q = 0.8, en - GB; q = 0.7, en - US; q = 0.6\r\n";
	int buflen = strlen(buf);

	cblog_parser parser;
	parser.on_event = http_parser_event_handler;
	cblog_parser_parse(&parser, buf, 0, buflen);
	printf("parse ok\n");
}