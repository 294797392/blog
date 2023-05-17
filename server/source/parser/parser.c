#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "errors.h"
#include "steak_string.h"
#include "connection.h"
#include "parser.h"

#define state_action(name) static void name(steak_parser *parser, char *http_msg, char character, int character_offset)
#define invoke_state_action(name) name(parser, http_msg, c, c_offset)

static void enter_state(steak_parser *parser, steak_parser_state state)
{
	YLOGI("enter state, %d -> %d", parser->state, state);
	parser->state = state;
}

state_action(action_initial)
{
	if((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
	{
		parser->seg_offset = character_offset;
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
	if(character == ' ')
	{
		// method结束
		parser->on_event(parser, STEAK_PARSER_EVENT_METHOD, http_msg + parser->seg_offset, parser->seg_len, NULL, 0);
		enter_state(parser, STEAK_PARSER_METHOD_END);
	}
	else
	{
		parser->seg_len++;
	}
}

state_action(action_method_end)
{
	if(isprint(character))
	{
		parser->seg_offset = character_offset;
		parser->seg_len = 1;
		enter_state(parser, STEAK_PARSER_URL);
	}
}

state_action(action_url_content)
{
	if(character == ' ')
	{
		// uri结束
		parser->on_event(parser, STEAK_PARSER_EVENT_URI, http_msg + parser->seg_offset, parser->seg_len, NULL, 0);
		enter_state(parser, STEAK_PARSER_URL_END);
	}
	else
	{
		parser->seg_len++;
	}
}

state_action(action_url_end)
{
	if(isprint(character))
	{
		parser->seg_offset = character_offset;
		parser->seg_len = 1;
		enter_state(parser, STEAK_PARSER_VERSION);
	}
}

state_action(action_version_content)
{
	if(character == '\n')
	{
		parser->on_event(parser, STEAK_PARSER_EVENT_VERSION, http_msg + parser->seg_offset, parser->seg_len, NULL, 0);
		enter_state(parser, STEAK_PARSER_VERSION_END);
	}
	else if(isprint(character))
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
	if(isprint(character))
	{
		// header key begin
		parser->seg_offset = character_offset;
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
	if(character == ':')
	{
		// header_key结束了，开始header_value
		parser->seg2_len = 0;
		parser->seg2_offset = 0;
		enter_state(parser, STEAK_PARSER_HEADER_VALUE);
	}
	else
	{
		parser->seg_len++;
	}
}

state_action(action_header_value)
{
	if(character == '\n')
	{
		// header_value结束了

		char *key = http_msg + parser->seg_offset;
		int keylen = parser->seg_len;
		char *value = http_msg + parser->seg2_offset;
		int valuelen = parser->seg2_len;
		if(keylen == strlen("content-length") && !strcasecmp(key, "content-length", keylen))
		{
			// 此时的http标头是Content-Length
			char length_buffer[64] = { '\0' };
			strncpy(length_buffer, value, sizeof(length_buffer));
			parser->content_length = atoi(length_buffer);
			YLOGI("content_length = %d", parser->content_length);
		}

		// 触发事件
		parser->on_event(parser, STEAK_PARSER_EVENT_HEADER, key, keylen, value, valuelen);

		parser->seg_len = 0;
		parser->seg_offset = 0;
		parser->seg2_len = 0;
		parser->seg2_offset = 0;
		enter_state(parser, STEAK_PARSER_HEADER_VALUE_END);
	}
	else if(isprint(character))
	{
		if(parser->seg2_offset == 0)
		{
			parser->seg2_offset = character_offset;
		}
		parser->seg2_len++;
	}
	else
	{
		// 不可见字符，可能是\r，啥都不做
	}
}

state_action(action_header_value_end)
{
	if(character == '\n')
	{
		// 此时说明所有的header都解析完了
		// 下面开始解析body。注意要处理body为0的情况
		if(parser->content_length == 0)
		{
			// 没有body
			parser->on_event(parser, STEAK_PARSER_EVENT_BODY, NULL, 0, NULL, 0);
			
			// 恢复到初始状态
			enter_state(parser, STEAK_PARSER_INITIAL);
		}
		else
		{
			// 有body，进入解析body状态
			enter_state(parser, STEAK_PARSER_BODY);
		}
	}
	else if(isprint(character))
	{
		// 此时说明又是header
		parser->seg_offset = character_offset;
		parser->seg_len = 1;
		enter_state(parser, STEAK_PARSER_HEADER_KEY);
	}
	else
	{
		// do noting
	}
}




int steak_parser_parse(steak_parser *parser, char *http_msg, int offset, int size)
{
	char *msg = http_msg + offset;
	int msg_size = size;

	for(int i = 0; i < msg_size; i++)
	{
		// 当前解析的字符
		char c = *(msg + i);

		// 基于http_msg的当前字符的偏移量
		int c_offset = offset + i;

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
				if(parser->seg_offset == 0)
				{
					parser->seg_offset = c_offset;
				}

				// 还需要接收的字节数
				int left = parser->content_length - parser->seg_len;
				// 当前收到的字节数
				int size = msg_size - i;
				if(size == left)
				{
					// 收到的字节数等于需要接收的字节数，本次解析完毕
					parser->seg_len = parser->content_length;
					parser->on_event(parser, STEAK_PARSER_EVENT_BODY, http_msg + parser->seg_offset, parser->seg_len, NULL, 0);
					enter_state(parser, STEAK_PARSER_INITIAL);
					return 0;
				}
				else if(size < left)
				{
					// 收到的字节数小于要接收的字节数
					// 需要外部继续接收请求数据。直接返回
					return size;
				}
				else
				{
					// 收到的字节数大于要接收的字节数
					// 有可能数据里包含下一次请求
					parser->seg_len = parser->content_length;
					parser->on_event(parser, STEAK_PARSER_EVENT_BODY, http_msg + parser->seg_offset, parser->seg_len, NULL, 0);
					enter_state(parser, STEAK_PARSER_INITIAL);
					return i + left;
				}
				break;
			}

			default:
				break;
		}
	}

	return size;
}

