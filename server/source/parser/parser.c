#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "errors.h"
#include "steak_string.h"
#include "parser.h"

typedef struct text2enum_s
{
	char *text;
	int value;
}text2enum;

static text2enum http_method_list[] =
{
	{.text = "GET",		.value = STEAK_HTTP_METHOD_GET },
	{.text = "POST",	.value = STEAK_HTTP_METHOD_POST },
	{.text = "HEAD",	.value = STEAK_HTTP_METHOD_HEAD },
	{.text = "PUT",		.value = STEAK_HTTP_METHOD_PUT },
	{.text = "TRACE",	.value = STEAK_HTTP_METHOD_TRACE },
	{.text = "OPTIONS", .value = STEAK_HTTP_METHOD_OPTIONS },
	{.text = "DELETE",	.value = STEAK_HTTP_METHOD_DELETE }
};

#define state_action(name) static void name(steak_parser *parser, steak_request *request, char c, int c_offset)
#define invoke_state_action(name) name(parser, request, c, c_offset)

static void enter_state(steak_parser *parser, steak_parser_state state)
{
	YLOGI("enter state, %d -> %d", parser->state, state);
	parser->state = state;
}

state_action(action_initial)
{
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		parser->seg_offset = c_offset;
		parser->seg_len = 1;
		enter_state(parser, STEAK_PARSER_METHOD);
	}
	else
	{
		// do noting
	}
}

state_action(action_method_content)
{
	if(c == ' ')
	{
		// method结束
		char *method = parser->raw_msg + parser->seg_offset;
		int method_len = parser->seg_len;
		size_t len = sizeof(http_method_list) / sizeof(text2enum);
		for(size_t i = 0; i < len; i++)
		{
			if(!strcasecmp(http_method_list[i].text, method, method_len))
			{
				request->method = http_method_list[i].value;
			}
		}
		enter_state(parser, STEAK_PARSER_METHOD_END);
	}
	else
	{
		parser->seg_len++;
	}
}

state_action(action_method_end)
{
	if(isprint(c))
	{
		parser->seg_offset = c_offset;
		parser->seg_len = 1;
		enter_state(parser, STEAK_PARSER_URL);
	}
}

state_action(action_url_content)
{
	if(c == ' ')
	{
		request->url = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(request->url, parser->raw_msg + parser->seg_offset, parser->seg_len);
		enter_state(parser, STEAK_PARSER_URL_END);
	}
	else
	{
		parser->seg_len++;
	}
}

state_action(action_url_end)
{
	if(isprint(c))
	{
		parser->seg_offset = c_offset;
		parser->seg_len = 1;
		enter_state(parser, STEAK_PARSER_VERSION);
	}
}

state_action(action_version_content)
{
	if(c == '\n')
	{
		request->version = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(request->version, parser->raw_msg + parser->seg_offset, parser->seg_len);
		enter_state(parser, STEAK_PARSER_VERSION_END);
	}
	else if(isprint(c))
	{
		parser->seg_len++;
	}
	else
	{
		// 不可见字符，可能是\r，啥都不做
	}
}

state_action(action_version_end)
{
	if(isprint(c))
	{
		// header key begin
		parser->seg_offset = c_offset;
		parser->seg_len = 1;
		enter_state(parser, STEAK_PARSER_HEADER_KEY);
	}
	else
	{
		// do noting
	}
}

state_action(action_header_key)
{
	if(c == ':')
	{
		// header_key结束了，开始header_value
		request->last_header->key = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(request->last_header->key, parser->raw_msg + parser->seg_offset, parser->seg_len);
		parser->seg_len = 0;
		parser->seg_offset = 0;
		enter_state(parser, STEAK_PARSER_HEADER_VALUE);
	}
	else
	{
		parser->seg_len++;
	}
}

state_action(action_header_value)
{
	if(c == '\n')
	{
		// header_value结束了
		request->last_header->value = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(request->last_header->value, parser->raw_msg + parser->seg_offset, parser->seg_len);
		parser->seg_len = 0;
		parser->seg_offset = 0;

		if(!strcasecmp(request->last_header->key, "content-length", strlen("content-length")))
		{
			// 此时的http标头是Content-Length
			request->content_length = atoi(request->last_header->value);
			YLOGI("content_length = %d", request->content_length);
		}

		enter_state(parser, STEAK_PARSER_HEADER_VALUE_END);
	}
	else if(isprint(c))
	{
		if(parser->seg_offset == 0)
		{
			parser->seg_offset = c_offset;
		}
		parser->seg_len++;
	}
	else
	{
		// 不可见字符，可能是\r，啥都不做
	}
}

state_action(action_header_value_end)
{
	if(c == '\n')
	{
		// 此时说明所有的header都解析完了
		// 下面开始解析body。注意要处理body为0的情况

		if(request->content_length == 0)
		{
			// 说明没有body
			YLOGI("content-length == 0, no body");
			enter_state(parser, STEAK_PARSER_COMPLETED);
		}
		else
		{
			parser->seg_len = 0;
			parser->seg_offset = 0;
			enter_state(parser, STEAK_PARSER_BODY);
		}
	}
	else if(isprint(c))
	{
		// 此时说明又是header
		steak_http_header *newhead = (steak_http_header *)Y_pool_obtain(sizeof(steak_http_header));
		request->last_header->next = newhead;
		newhead->prev = request->last_header;
		request->last_header = newhead;

		parser->seg_offset = c_offset;
		parser->seg_len = 1;

		enter_state(parser, STEAK_PARSER_HEADER_KEY);
	}
	else
	{
		// do noting
	}
}

state_action(action_body)
{
	if(parser->seg_offset == 0)
	{
		parser->seg_offset = c_offset;
	}
	parser->seg_len++;

	// seg_len等于content_length，说明body接收完毕
	if(parser->seg_len == request->content_length)
	{
		// 此时说明body接收完毕
		request->content = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(request->content, parser->raw_msg + parser->seg_offset, parser->seg_len);
		enter_state(parser, STEAK_PARSER_COMPLETED);
	}
}



int steak_parser_parse(steak_parser *parser, steak_request *request)
{
	char *msg = parser->raw_msg + parser->raw_msg_offset;
	int msg_size = parser->readsize;

	for(int i = 0; i < msg_size; i++)
	{
		// 当前解析的字符
		char c = *(msg + i);

		// 基于raw_msg的当前字符的偏移量
		int c_offset = parser->raw_msg_offset + i;

		switch(parser->state)
		{
			case STEAK_PARSER_INITIAL:
			{
				invoke_state_action(action_initial);
				break;
			}

			case STEAK_PARSER_METHOD:
			{
				invoke_state_action(action_method_content);
				break;
			}

			case STEAK_PARSER_METHOD_END:
			{
				invoke_state_action(action_method_end);
				break;
			}

			case STEAK_PARSER_URL:
			{
				invoke_state_action(action_url_content);
				break;
			}

			case STEAK_PARSER_URL_END:
			{
				invoke_state_action(action_url_end);
				break;
			}

			case STEAK_PARSER_VERSION:
			{
				invoke_state_action(action_version_content);
				break;
			}

			case STEAK_PARSER_VERSION_END:
			{
				invoke_state_action(action_version_end);
				break;
			}

			case STEAK_PARSER_HEADER_KEY:
			{
				invoke_state_action(action_header_key);
				break;
			}

			case STEAK_PARSER_HEADER_VALUE:
			{
				invoke_state_action(action_header_value);
				break;
			}

			case STEAK_PARSER_HEADER_VALUE_END:
			{
				invoke_state_action(action_header_value_end);
				break;
			}

			case STEAK_PARSER_BODY:
			{
				invoke_state_action(action_body);
				break;
			}

			case STEAK_PARSER_ERROR:
			{
				return STEAK_ERR_REQUEST_INVALID;
			}

			default:
				break;
		}
	}

	parser->raw_msg_offset += msg_size;

	return STEAK_ERR_OK;
}

