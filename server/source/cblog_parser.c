#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cblog_parser.h"
#include "cblog_errors.h"
#include "cblog_string.h"

#define state_action(name) static void name(cblog_parser *parser, char *http_msg, char character, int character_offset)
#define invoke_state_action(name) name(parser, http_msg, c, c_offset)

static void enter_state(cblog_parser *parser, cblog_parser_state_enum state)
{
	//YLOGI("state changed, %d -> %d", parser->state, state);
	parser->state = state;
}

state_action(action_initial)
{
	if((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
	{
		parser->seg_offset = character_offset;
		parser->seg_len = 1;
		enter_state(parser, CBLOG_PARSER_STATE_METHOD);
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
		parser->on_event(parser, CBLOG_PARSER_EVENT_METHOD);
		enter_state(parser, CBLOG_PARSER_STATE_METHOD_END);
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
		enter_state(parser, CBLOG_PARSER_STATE_URL);
	}
}

state_action(action_url_content)
{
	if(character == ' ')
	{
		// uri结束
		parser->on_event(parser, CBLOG_PARSER_EVENT_URI);
		enter_state(parser, CBLOG_PARSER_STATE_URL_END);
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
		enter_state(parser, CBLOG_PARSER_STATE_VERSION);
	}
}

state_action(action_version_content)
{
	if(character == '\n')
	{
		parser->on_event(parser, CBLOG_PARSER_EVENT_VERSION);
		enter_state(parser, CBLOG_PARSER_STATE_VERSION_END);
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
		enter_state(parser, CBLOG_PARSER_STATE_HEADER_KEY);
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
		enter_state(parser, CBLOG_PARSER_STATE_HEADER_VALUE);
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

		// 触发事件
		parser->on_event(parser, CBLOG_PARSER_EVENT_HEADER);

		parser->seg_len = 0;
		parser->seg_offset = 0;
		parser->seg2_len = 0;
		parser->seg2_offset = 0;
		enter_state(parser, CBLOG_PARSER_STATE_HEADER_VALUE_END);
	}
	else if(character != ' ' && isprint(character))
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




int cblog_parser_parse(cblog_parser *parser, char *http_msg, int offset, int size)
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
			case CBLOG_PARSER_STATE_INITIAL:
			{
				invoke_state_action(action_initial);
				break;
			}

			case CBLOG_PARSER_STATE_METHOD:
			{
				invoke_state_action(action_method_content);
				break;
			}

			case CBLOG_PARSER_STATE_METHOD_END:
			{
				invoke_state_action(action_method_end);
				break;
			}

			case CBLOG_PARSER_STATE_URL:
			{
				invoke_state_action(action_url_content);
				break;
			}

			case CBLOG_PARSER_STATE_URL_END:
			{
				invoke_state_action(action_url_end);
				break;
			}

			case CBLOG_PARSER_STATE_VERSION:
			{
				invoke_state_action(action_version_content);
				break;
			}

			case CBLOG_PARSER_STATE_VERSION_END:
			{
				invoke_state_action(action_version_end);
				break;
			}

			case CBLOG_PARSER_STATE_HEADER_KEY:
			{
				invoke_state_action(action_header_key);
				break;
			}

			case CBLOG_PARSER_STATE_HEADER_VALUE:
			{
				invoke_state_action(action_header_value);
				break;
			}

			case CBLOG_PARSER_STATE_HEADER_VALUE_END:
			{
				if(c == '\n')
				{
					// 此时说明所有的header都解析完了
					// 下面开始解析body。注意要处理body为0的情况
					if(parser->content_length == 0)
					{
						// 没有body，触发请求结束事件
						parser->on_event(parser, CBLOG_PARSER_EVENT_BODY);

						// 恢复到初始状态
						enter_state(parser, CBLOG_PARSER_STATE_INITIAL);

						return 0;
					}
					else
					{
						// 有body，进入解析body状态
						enter_state(parser, CBLOG_PARSER_STATE_BODY);
					}
				}
				else if(isprint(c))
				{
					// 此时说明又是header
					parser->seg_offset = c_offset;
					parser->seg_len = 1;
					enter_state(parser, CBLOG_PARSER_STATE_HEADER_KEY);
				}
				else
				{
					// do noting
				}
				break;
			}

			case CBLOG_PARSER_STATE_BODY:
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
					parser->on_event(parser, CBLOG_PARSER_EVENT_BODY);
					enter_state(parser, CBLOG_PARSER_STATE_INITIAL);
					return 0;
				}
				else if(size < left)
				{
					// 收到的字节数小于要接收的字节数，本次请求还没解析完
					// 需要外部继续接收请求数据。直接返回
					return -1;
				}
				else
				{
					// 收到的字节数大于要接收的字节数
					// 有可能数据里包含下一次请求
					parser->seg_len = parser->content_length;
					parser->on_event(parser, CBLOG_PARSER_EVENT_BODY);
					enter_state(parser, CBLOG_PARSER_STATE_INITIAL);
					return i + left;
				}
				break;
			}

			default:
				break;
		}
	}

	return -1;
}

