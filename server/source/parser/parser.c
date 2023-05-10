#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"

int steak_parser_parse(steak_parser *parser)
{
	char *buffer = parser->buffer + parser->offset;
	char bufsize = parser->buffer_size - parser->offset;

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
					parser->method->str = str;
					parser->method->len++;
					parser->state = STEAK_PARSER_METHOD;
				}
				break;
			}

			case STEAK_PARSER_METHOD:
			{
				if(c == ' ')
				{
					parser->state = STEAK_PARSER_METHOD_END;
				}
				else
				{
					parser->method->len++;
				}
				break;
			}

			case STEAK_PARSER_METHOD_END:
			{
				if(isprint(c))
				{
					parser->url->str = str;
					parser->url->len++;
					parser->state = STEAK_PARSER_URL;
				}
				break;
			}

			case STEAK_PARSER_URL:
			{
				if(c == ' ')
				{
					parser->state = STEAK_PARSER_URL_END;
				}
				else
				{
					parser->url->len++;
				}
				break;
			}

			case STEAK_PARSER_URL_END:
			{
				if(isprint(c))
				{
					parser->version->str = str;
					parser->version->len++;
					parser->state = STEAK_PARSER_VERSION;
				}
				break;
			}

			case STEAK_PARSER_VERSION:
			{
				if(c == '\r' || c == '\n')
				{
					parser->state = STEAK_PARSER_VERSION_END;
				}
				else
				{
					parser->version->len++;
				}
				break;
			}

			case STEAK_PARSER_VERSION_END:
			{
				if(c == '\r' || c == '\n')
				{
					// ���з������ָ���һ�����з�������û�������ַ�
					// ��ʱ������û��header?
					YLOGW("double crlf after version, no headers?");
					parser->state = STEAK_PARSER_BODY;
				}
				else if(isprint(c))
				{
					// header key begin
				}
				else
				{
					// do noting
				}
				break;
			}

			default:
				break;
		}
	}

	parser->offset += bufsize;
}