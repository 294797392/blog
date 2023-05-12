/***********************************************************************************
 * @ file    : session.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.09 19:11
 * @ brief   : ����session����
 ************************************************************************************/

#ifndef __STEAK_SESSION_H__
#define __STEAK_SESSION_H__

#include <stdint.h>

#include "steak_socket.h"
#include "request.h"
#include "response.h"
#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// �洢��ǰ�Ự����ϸ��Ϣ
	/// һ��HTTP�������Ӿ���һ���Ự
	/// </summary>
	typedef struct steak_session_s steak_session;

	struct steak_session_s
	{
		/// <summary>
		/// HTTP���Ľ�����
		/// </summary>
		steak_parser *parser;

		/// <summary>
		/// ��ǰ���ڴ��������
		/// </summary>
		steak_request *request;

		/// <summary>
		/// ��ǰ���ڴ������Ӧ
		/// </summary>
		steak_response *response;

		/// <summary>
		/// �ͻ��˵�socket
		/// </summary>
		steak_socket sock;
	};

#ifdef __cplusplus
}
#endif

#endif