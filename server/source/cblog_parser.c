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
		// method����
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
		// uri����
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
		// ���ɼ��ַ���������\r��ɶ������
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
		// header_key�����ˣ���ʼheader_value
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
		// header_value������

		// �����¼�
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
		// ���ɼ��ַ���������\r��ɶ������
	}
}




int cblog_parser_parse(cblog_parser *parser, char *http_msg, int offset, int size)
{
	char *msg = http_msg + offset;
	int msg_size = size;

	for(int i = 0; i < msg_size; i++)
	{
		// ��ǰ�������ַ�
		char c = *(msg + i);

		// ����http_msg�ĵ�ǰ�ַ���ƫ����
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
					// ��ʱ˵�����е�header����������
					// ���濪ʼ����body��ע��Ҫ����bodyΪ0�����
					if(parser->content_length == 0)
					{
						// û��body��������������¼�
						parser->on_event(parser, CBLOG_PARSER_EVENT_BODY);

						// �ָ�����ʼ״̬
						enter_state(parser, CBLOG_PARSER_STATE_INITIAL);

						return 0;
					}
					else
					{
						// ��body���������body״̬
						enter_state(parser, CBLOG_PARSER_STATE_BODY);
					}
				}
				else if(isprint(c))
				{
					// ��ʱ˵������header
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

				// ����Ҫ���յ��ֽ���
				int left = parser->content_length - parser->seg_len;
				// ��ǰ�յ����ֽ���
				int size = msg_size - i;
				if(size == left)
				{
					// �յ����ֽ���������Ҫ���յ��ֽ��������ν������
					parser->seg_len = parser->content_length;
					parser->on_event(parser, CBLOG_PARSER_EVENT_BODY);
					enter_state(parser, CBLOG_PARSER_STATE_INITIAL);
					return 0;
				}
				else if(size < left)
				{
					// �յ����ֽ���С��Ҫ���յ��ֽ�������������û������
					// ��Ҫ�ⲿ���������������ݡ�ֱ�ӷ���
					return -1;
				}
				else
				{
					// �յ����ֽ�������Ҫ���յ��ֽ���
					// �п��������������һ������
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

