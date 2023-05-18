/***********************************************************************************
 * @ file    : cblog_types.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.10 19:06
 * @ brief   : ����cblogʹ�õ���Ҫ�����ݽṹ������ģ�鵥�����嵽ģ���ͷ�ļ���
 ************************************************************************************/

#ifndef __CBLOG_TYPES_H__
#define __CBLOG_TYPES_H__

#include "parser.h"
#include "default.h"
#include "protocol.h"
#include "steak_socket.h"
#include "cblog_sockbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct steak_string_s steak_string;
	typedef struct steak_http_header_s steak_http_header;
	typedef struct svchost_s svchost;
	typedef struct svchost_options_s svchost_options;
	typedef struct steak_connection_s steak_connection;
	typedef struct steak_response_s steak_response;
	typedef struct steak_request_s steak_request;
	typedef struct steak_session_s steak_session;

	struct steak_string_s
	{
		int offset;
		int length;
	};

	struct svchost_options_s
	{
		char bindaddr[64];
		int port;
		char root[256];
		char index_page[256];
	};

	struct svchost_s
	{
		steak_socket sock;
		svchost_options options;
	};

	/// <summary>
	/// ����HTTP��ͷ��Ϣ
	/// </summary>
	struct steak_http_header_s
	{
		steak_string key;
		steak_string value;
	};

	/// <summary>
	/// ��¼��ͻ��˵�HTTP����״̬
	/// </summary>
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
		/// ������������service
		/// </summary>
		svchost *svc;

		/// <summary>
		/// ������������һ���������Ϣ
		/// </summary>
		steak_request *request;

		/// <summary>
		/// ������������һ����Ӧ����Ϣ
		/// </summary>
		steak_response *response;

		/// <summary>
		/// ���ջ�����
		/// ���浱ǰrequest��ԭʼHTTP����
		/// </summary>
		cblog_sockbuf *recvbuf;

		/// <summary>
		/// ���ͻ�����
		/// ���浱ǰ��Ҫ���͸��ͻ��˵�ԭʼHTTP����
		/// </summary>
		cblog_sockbuf *sendbuf;
	};

	/// <summary>
	/// �洢��ǰHTTP�������ϸ��Ϣ
	/// </summary>
	struct steak_request_s
	{
		/// <summary>
		/// ԭʼHTTP����
		/// </summary>
		char *raw_msg;

		/// <summary>
		/// ����ʱ��
		/// </summary>
		uint64_t timestamp;

		/// <summary>
		/// Content-Length��ͷ��ֵ
		/// 0��content-lengthΪ0����ʾû��body
		/// </summary>
		int content_length;

		/// <summary>
		/// ���󷽷�
		/// </summary>
		http_method_enum method;
		steak_string url;
		steak_string version;
		steak_string body;

		/// <summary>
		/// HTTP��ͷ
		/// </summary>
		steak_http_header headers[STEAK_DEFAULT_HEADER_COUNT];

		/// <summary>
		/// HTTP��ͷ����
		/// </summary>
		int nheader;
	};

	/// <summary>
	/// ��¼HTTP��Ӧ����ϸ��Ϣ
	/// </summary>
	struct steak_response_s
	{
		/// <summary>
		/// HTTP��Ӧͷ��
		/// </summary>
		steak_http_header *header;

		char *content;
		int content_length;
	};

	/// <summary>
	/// �洢��ǰ�Ự����ϸ��Ϣ
	/// һ��HTTP�������Ӿ���һ���Ự
	/// </summary>
	struct steak_session_s
	{
		/// <summary>
		/// �ûỰ������״̬
		/// </summary>
		steak_request request;

		/// <summary>
		/// �ûỰ����Ӧ״̬
		/// </summary>
		steak_response response;

		/// <summary>
		/// ��һ���Ự
		/// </summary>
		steak_session *prev;

		/// <summary>
		/// ��һ���Ự
		/// </summary>
		steak_session *next;
	};

#ifdef __cplusplus
}
#endif

#endif