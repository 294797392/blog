/***********************************************************************************
 * @ file    : steak_module.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.15 11:00
 * @ brief   : ��HTTP������Ӧ����֮���û��Զ���Ĵ���HTTP����Ķ���
 ************************************************************************************/

#ifndef __STEAK_MODULE_H__
#define __STEAK_MODULE_H__

#include "request.h"
#include "response.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// �洢ģ��״̬
	/// </summary>
	typedef struct steak_module_s steak_module;

	/// <summary>
	/// ģ����������
	/// </summary>
	typedef struct steak_module_input_s steak_module_input;

	struct steak_module_s
	{
		char *name;
		char *desc;
		char *author;

		/// <summary>
		/// ģ������
		/// </summary>
		int type;

		/// <summary>
		/// ������Ӧ��������ʱ�����
		/// ֻ����һ��
		/// </summary>
		int(*initialize)();

		/// <summary>
		/// ������Ӧ�ý�����ʱ�����
		/// ֻ����һ��
		/// </summary>
		void(*release)();

		/// <summary>
		/// ÿ�յ�һ��HTTP����͵���һ��
		/// </summary>
		int(*process)(steak_module_input *input);
		void *context;
	};

	struct steak_module_input_s
	{
		steak_request *request;
		steak_response *response;
	};

#ifdef __cplusplus
}
#endif


#endif
