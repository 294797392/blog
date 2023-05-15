/***********************************************************************************
 * @ file    : session.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.13 08:29
 * @ brief   : ����HTTP���Ӷ���
 ************************************************************************************/

#ifndef __STEAK_CONNECTION_H__
#define __STEAK_CONNECTION_H__

#include <libY.h>

#include "session.h"
#include "parser.h"
#include "steak_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct steak_connection_s steak_connection;
	struct steak_connection_s
	{
		/// <summary>
		/// �ͻ��˵�socket
		/// </summary>
		steak_socket sock;

		/// <summary>
		/// HTTP���Ľ�����
		/// </summary>
		steak_parser parser;

		/// <summary>
		/// �����պܿ죬���ͺ�����ʱ�򣬿��ܻ�����ж����Ӧû���ͳ�ȥ�����
		/// �ö��д洢�����͵���Ӧ
		/// </summary>
		steak_session *first_session;

		/// <summary>
		/// session�б�������һ���Ự��Ҳ�����ڴ���ĻỰ
		/// </summary>
		steak_session *last_session;

		/// <summary>
		/// ��ǰ���ڽ��յ�HTTP����
		/// </summary>
		steak_session *active_session;
	};

#ifdef __cplusplus
}
#endif

#endif