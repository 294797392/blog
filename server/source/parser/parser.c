#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "errors.h"
#include "steak_string.h"
#include "parser.h"

static void enter_state(steak_parser *parser, steak_parser_state state)
{
	YLOGI("enter state, %s -> %s", parser->state, state);
	parser->state = state;
}

int steak_parser_parse(steak_parser *parser)
{
	char *buffer = parser->raw_msg + parser->offset;
	char bufsize = parser->raw_msg_len - parser->offset;

	for(size_t i = 0; i < bufsize; i++)
	{
		char *str = buffer + i;
		char c = str[0];

		switch(parser->state)
		{
			case STEAK_PARSER_INITIAL:
			{
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
				{
					parser->seg_offset = parser->offset + i;
					parser->seg_len = 1;
					enter_state(parser, STEAK_PARSER_METHOD);
				}
				break;
			}

			case STEAK_PARSER_METHOD:
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
				break;
			}

			case STEAK_PARSER_METHOD_END:
			{
				if(isprint(c))
				{
					parser->seg_offset = parser->offset + i;
					parser->seg_len = 1;
					enter_state(parser, STEAK_PARSER_URL);
				}
				break;
			}

			case STEAK_PARSER_URL:
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
				break;
			}

			case STEAK_PARSER_URL_END:
			{
				if(isprint(c))
				{
					parser->seg_offset = parser->offset + i;
					parser->seg_len = 1;
					enter_state(parser, STEAK_PARSER_VERSION);
				}
				break;
			}

			case STEAK_PARSER_VERSION:
			{
				if(c == '\n')
				{
					parser->version = (char *)Y_pool_obtain(parser->seg_len + 1);
					strncpy(parser->version, parser->raw_msg + parser->seg_offset, parser->seg_len);
					enter_state(parser, STEAK_PARSER_VERSION_END);
				}
				else
				{
					parser->seg_len++;
				}
				break;
			}

			case STEAK_PARSER_VERSION_END:
			{
				if(isprint(c))
				{
					// header key begin
					parser->seg_offset = parser->offset + i;
					parser->seg_len = 1;
					enter_state(parser, STEAK_PARSER_HEADER_KEY);
				}
				else
				{
					// do noting
				}
				break;
			}

			case STEAK_PARSER_HEADER_KEY:
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
				break;
			}

			case STEAK_PARSER_HEADER_VALUE:
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
				else
				{
					if(parser->seg_offset == 0)
					{
						parser->seg_offset = parser->offset + i;
					}
					parser->seg_len++;
				}
				break;
			}

			case STEAK_PARSER_HEADER_VALUE_END:
			{
				if(c == '\n')
				{
					// ��ʱ˵�����е�header����������
					// ���濪ʼ����body��ע��Ҫ����bodyΪ0�����
					parser->seg_len = 0;
					parser->seg_offset = 0;

					if(parser->content_length == -1)
					{
						// ��ʱ˵���ͻ���û�з���content-length��ͷ������������
						// �ر�����?
						// TODO����δ������
						YLOGE("no content-length found, invalid request");
						return STEAK_ERR_REQUEST_INVALID;
					}
					else if(parser->content_length == 0)
					{
						// ˵��û��body
						YLOGI("content-length == 0, no body");
					}
					else
					{
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

					parser->seg_len = 1;
					parser->seg_offset = parser->offset + i;

					enter_state(parser, STEAK_PARSER_HEADER_KEY);
				}
				else
				{
					// do noting
				}
				break;
			}

			case STEAK_PARSER_BODY:
			{
				if(parser->seg_offset == 0)
				{
					parser->seg_offset = parser->offset + i;
				}
				parser->seg_len++;

				// seg_len����content_length��˵��body�������
				if(parser->seg_len == parser->content_length)
				{
					// ��ʱ˵��body�������
					parser->body = (char *)Y_pool_obtain(parser->seg_len + 1);
					strncpy(parser->body, parser->raw_msg + parser->seg_offset, parser->seg_len);
					return STEAK_ERR_REQUEST_COMPLETED;
				}
				break;
			}

			default:
				break;
		}
	}

	parser->offset += bufsize;

	return STEAK_ERR_OK;
}

