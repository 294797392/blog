/***********************************************************************************
 * @ file    : cblog_xaml2html.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.23 17:05
 * @ brief   : xaml转html模块
 ************************************************************************************/

#ifndef __CBLOG_XAML2HTML_H__
#define __CBLOG_XAML2HTML_H__

#include <stdint.h>

#include <libY.h>

#include "cblog_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum cblog_xaml_parse_state_enum cblog_xaml_parse_state_enum;
	typedef struct cblog_xaml_property_s cblog_xaml_property;
	typedef struct cblog_xaml_tag_s cblog_xaml_tag;
	typedef struct cblog_xaml2html_s cblog_xaml2html;

	cblog_xaml2html *new_cblog_xaml2html();
	void free_cblog_xaml2html(cblog_xaml2html *x2h);

	/// <summary>
	/// 把xaml字符串转成html字符串
	/// </summary>
	/// <param name="x2h"></param>
	/// <param name="xaml"></param>
	void cblog_x2h_make_xaml_tree(cblog_xaml2html *x2h, cblog_buffer *xaml);

	/// <summary>
	/// 把当前的x2h渲染成html
	/// </summary>
	/// <param name="x2h"></param>
	/// <param name="buffer"></param>
	void cblog_x2h_render_html(cblog_xaml2html *x2h, cblog_buffer *buffer);

#ifdef __cplusplus
}
#endif

#endif