#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "cblog_buffer.h"
#include "cblog_xaml2html.h"

#define state_action(name) static void name(cblog_xaml2html *x2h, char c)
#define invoke_state_action(name) name(x2h, c)

enum cblog_xaml_parse_state_enum
{
	CBLOG_XAML_PARSE_STATE_INITIAL,
	CBLOG_XAML_PARSE_STATE_TAG_BEGIN,
	CBLOG_XAML_PARSE_STATE_TAG_END,
	CBLOG_XAML_PARSE_STATE_PROPERTY_KEY_BEGIN,
	CBLOG_XAML_PARSE_STATE_PROPERTY_VALUE_BEGIN,
};

struct cblog_xaml_property_s
{
	cblog_string key;
	cblog_string value;
};

struct cblog_xaml_tag_s
{
	/// <summary>
	/// 同级别上一个节点
	/// </summary>
	cblog_xaml_tag *prev;

	/// <summary>
	/// 同级别下一个节点
	/// </summary>
	cblog_xaml_tag *next;

	/// <summary>
	/// 父节点
	/// </summary>
	cblog_xaml_tag *parent;

	/// <summary>
	/// 子节点列表
	/// </summary>
	cblog_xaml_tag *child;
};

struct cblog_xaml2html_s
{
	cblog_xaml_tag *first;
	cblog_xaml_tag *last;

	/// <summary>
	/// 当前正在解析的标签
	/// </summary>
	cblog_xaml_tag *current;

	cblog_xaml_parse_state_enum state;
};


state_action(action_initial)
{
	if(c == '<')
	{

	}
	else
	{
		// ignore
	}
}

state_action(action_tag_begin)
{}

state_action(action_tag_end)
{}

state_action(action_property_key_begin)
{}

state_action(action_property_value_begin)
{}



cblog_xaml2html *new_cblog_xaml2html()
{
	cblog_xaml2html *x2h = (cblog_xaml2html *)calloc(1, sizeof(cblog_xaml2html));
	return x2h;
}

void free_cblog_xaml2html(cblog_xaml2html *x2h)
{
	free(x2h);
}

void cblog_x2h_make_xaml_tree(cblog_xaml2html *x2h, cblog_buffer *xaml)
{
	int len = xaml->offset;
	char *str = xaml->pdata;

	for(int i = 0; i < len; i++)
	{
		char c = *(str + i);

		switch(x2h->state)
		{
			case CBLOG_XAML_PARSE_STATE_INITIAL:
			{
				invoke_state_action(action_initial);
				break;
			}

			case CBLOG_XAML_PARSE_STATE_TAG_BEGIN:
			{
				invoke_state_action(action_tag_begin);
				break;
			}

			case CBLOG_XAML_PARSE_STATE_TAG_END:
			{
				invoke_state_action(action_tag_end);
				break;
			}

			case CBLOG_XAML_PARSE_STATE_PROPERTY_KEY_BEGIN:
			{
				invoke_state_action(action_property_key_begin);
				break;
			}

			case CBLOG_XAML_PARSE_STATE_PROPERTY_VALUE_BEGIN:
			{
				invoke_state_action(action_property_value_begin);
				break;
			}

			default:
				break;
		}
	}
}

void cblog_x2h_render_html(cblog_xaml2html *x2h, cblog_buffer *buffer)
{

}


