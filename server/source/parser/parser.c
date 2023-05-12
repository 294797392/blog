#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "errors.h"
#include "steak_string.h"
#include "parser.h"

static void enter_state(steak_parser *parser, steak_parser_state state)
{
	YLOGI("enter state, %d -> %d", parser->state, state);
	parser->state = state;
}

static void action_initial(steak_parser *parser, char c, int c_offset)
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

static void action_method_content(steak_parser *parser, char c, int c_offset)
{
	if(c == ' ')
	{
		parser->method = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(parser->method, parser->raw_msg + parser->seg_offset, parser->seg_len);
		enter_state(parser, STEAK_PARSER_METHOD_END);
	}
	else
	{
		parser->seg_len++;
	}
}

static void action_method_end(steak_parser *parser, char c, int c_offset)
{
	if(isprint(c))
	{
		parser->seg_offset = c_offset;
		parser->seg_len = 1;
		enter_state(parser, STEAK_PARSER_URL);
	}
}

static void action_url_content(steak_parser *parser, char c, int c_offset)
{
	if(c == ' ')
	{
		parser->url = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(parser->url, parser->raw_msg + parser->seg_offset, parser->seg_len);
		enter_state(parser, STEAK_PARSER_URL_END);
	}
	else
	{
		parser->seg_len++;
	}
}

static void action_url_end(steak_parser *parser, char c, int c_offset)
{
	if(isprint(c))
	{
		parser->seg_offset = c_offset;
		parser->seg_len = 1;
		enter_state(parser, STEAK_PARSER_VERSION);
	}
}

static void action_version_content(steak_parser *parser, char c, int c_offset)
{
	if(c == '\n')
	{
		parser->version = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(parser->version, parser->raw_msg + parser->seg_offset, parser->seg_len);
		enter_state(parser, STEAK_PARSER_VERSION_END);
	}
	else if(isprint(c))
	{
		parser->seg_len++;
	}
	else
	{
		// ���ɼ��ַ���������\r��ɶ������
	}
}

static void action_version_end(steak_parser *parser, char c, int c_offset)
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

static void action_header_key(steak_parser *parser, char c, int c_offset)
{
	if(c == ':')
	{
		// header_key�����ˣ���ʼheader_value
		parser->last_header->key = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(parser->last_header->key, parser->raw_msg + parser->seg_offset, parser->seg_len);
		parser->seg_len = 0;
		parser->seg_offset = 0;
		enter_state(parser, STEAK_PARSER_HEADER_VALUE);
	}
	else
	{
		parser->seg_len++;
	}
}

static void action_header_value(steak_parser *parser, char c, int c_offset)
{
	if(c == '\n')
	{
		// header_value������
		parser->last_header->value = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(parser->last_header->value, parser->raw_msg + parser->seg_offset, parser->seg_len);
		parser->seg_len = 0;
		parser->seg_offset = 0;

		if(!strcasecmp(parser->last_header->key, "content-length", strlen("content-length")))
		{
			// ��ʱ��http��ͷ��Content-Length
			parser->content_length = atoi(parser->last_header->value);
			YLOGI("content_length = %d", parser->content_length);
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
		// ���ɼ��ַ���������\r��ɶ������
	}
}

static void action_header_value_end(steak_parser *parser, char c, int c_offset)
{
	if(c == '\n')
	{
		// ��ʱ˵�����е�header����������
		// ���濪ʼ����body��ע��Ҫ����bodyΪ0�����
		
		if(parser->content_length == 0)
		{
			// ˵��û��body
			YLOGI("content-length == 0, no body");
			enter_state(parser, STEAK_PARSER_END);
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
		// ��ʱ˵������header
		steak_http_header *newhead = (steak_http_header *)Y_pool_obtain(sizeof(steak_http_header));
		parser->last_header->next = newhead;
		newhead->prev = parser->last_header;
		parser->last_header = newhead;

		parser->seg_offset = c_offset;
		parser->seg_len = 1;

		enter_state(parser, STEAK_PARSER_HEADER_KEY);
	}
	else
	{
		// do noting
	}
}

static void action_body(steak_parser *parser, char c, int c_offset)
{
	if(parser->seg_offset == 0)
	{
		parser->seg_offset = c_offset;
	}
	parser->seg_len++;

	// seg_len����content_length��˵��body�������
	if(parser->seg_len == parser->content_length)
	{
		// ��ʱ˵��body�������
		parser->body = (char *)Y_pool_obtain(parser->seg_len + 1);
		strncpy(parser->body, parser->raw_msg + parser->seg_offset, parser->seg_len);
		parser->completed = 1;
	}
}



int steak_parser_parse(steak_parser *parser)
{
	char *msg = parser->raw_msg + parser->raw_msg_offset;
	int msg_size = parser->readsize;

	for(int i = 0; i < msg_size; i++)
	{
		// ��ǰ�������ַ�
		char c = *(msg + i);

		// ����msg�ĵ�ǰ�ַ���ƫ����
		int c_offset = parser->raw_msg_offset + i;

		switch(parser->state)
		{
			case STEAK_PARSER_INITIAL:
			{
				action_initial(parser, c, c_offset);
				break;
			}

			case STEAK_PARSER_METHOD:
			{
				action_method_content(parser, c, c_offset);
				break;
			}

			case STEAK_PARSER_METHOD_END:
			{
				action_method_end(parser, c, c_offset);
				break;
			}

			case STEAK_PARSER_URL:
			{
				action_url_content(parser, c, c_offset);
				break;
			}

			case STEAK_PARSER_URL_END:
			{
				action_url_end(parser, c, c_offset);
				break;
			}

			case STEAK_PARSER_VERSION:
			{
				action_version_content(parser, c, c_offset);
				break;
			}

			case STEAK_PARSER_VERSION_END:
			{
				action_version_end(parser, c, c_offset);
				break;
			}

			case STEAK_PARSER_HEADER_KEY:
			{
				action_header_key(parser, c, c_offset);
				break;
			}

			case STEAK_PARSER_HEADER_VALUE:
			{
				action_header_value(parser, c, c_offset);
				break;
			}

			case STEAK_PARSER_HEADER_VALUE_END:
			{
				action_header_value_end(parser, c, c_offset);
				break;
			}

			case STEAK_PARSER_BODY:
			{
				action_body(parser, c, c_offset);
				break;
			}

			default:
				break;
		}
	}

	parser->raw_msg_offset += msg_size;

	return STEAK_ERR_OK;
}

