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

	typedef struct cblog_string_s cblog_string;
	typedef struct cblog_http_header_s cblog_http_header;
	typedef struct svchost_s svchost;
	typedef struct svchost_options_s svchost_options;
	typedef enum cblog_conn_status_enum cblog_conn_status_enum;
	typedef struct cblog_connection_s cblog_connection;
	typedef struct cblog_response_s cblog_response;
	typedef struct cblog_request_s cblog_request;
	typedef struct cblog_session_s cblog_session;

	struct cblog_string_s
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
		cblog_socket sock;
		svchost_options options;
	};

	/// <summary>
	/// ����HTTP��ͷ��Ϣ
	/// </summary>
	struct cblog_http_header_s
	{
		cblog_string key;
		cblog_string value;
	};

	enum cblog_conn_status_enum
	{
		/// <summary>
		/// ������
		/// </summary>
		CBLOG_CONN_STATUS_CONNECTED,

		/// <summary>
		/// �����ر�����
		/// </summary>
		CBLOG_CONN_STATUS_CLOSE,

		/// <summary>
		/// �Է��ѹر�����
		/// </summary>
		CBLOG_CONN_STATS_CLOSED_BY_PEER,

		/// <summary>
		/// ��/д����
		/// </summary>
		CBLOG_CONN_STATUS_ERROR,

		/// <summary>
		/// �����ѳ�ʱ
		/// </summary>
		CBLOG_CONN_STATUS_TIMEOUT
	};

	/// <summary>
	/// ��¼��ͻ��˵�HTTP����״̬
	/// </summary>
	struct cblog_connection_s
	{
		/// <summary>
		/// �ͻ��˵�socket
		/// </summary>
		cblog_socket sock;

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
		cblog_request *request;

		/// <summary>
		/// ������������һ����Ӧ����Ϣ
		/// </summary>
		cblog_response *response;

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

		/// <summary>
		/// �����ӵ�״̬
		/// </summary>
		cblog_conn_status_enum status;
	};

	/// <summary>
	/// �洢��ǰHTTP�������ϸ��Ϣ
	/// </summary>
	struct cblog_request_s
	{
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
		cblog_string method_string;
		cblog_string url;
		cblog_string version;
		cblog_string body;

		/// <summary>
		/// HTTP��ͷ
		/// </summary>
		cblog_http_header headers[STEAK_DEFAULT_HEADER_COUNT];

		/// <summary>
		/// HTTP��ͷ����
		/// </summary>
		int nheader;
	};

	/// <summary>
	/// ��¼HTTP��Ӧ����ϸ��Ϣ
	/// </summary>
	struct cblog_response_s
	{
		/// <summary>
		/// HTTP��Ӧͷ��
		/// </summary>
		cblog_http_header *header;

		char *content;
		int content_length;
	};

	/// <summary>
	/// �洢��ǰ�Ự����ϸ��Ϣ
	/// һ��HTTP�������Ӿ���һ���Ự
	/// </summary>
	struct cblog_session_s
	{
		/// <summary>
		/// �ûỰ������״̬
		/// </summary>
		cblog_request request;

		/// <summary>
		/// �ûỰ����Ӧ״̬
		/// </summary>
		cblog_response response;

		/// <summary>
		/// ��һ���Ự
		/// </summary>
		cblog_session *prev;

		/// <summary>
		/// ��һ���Ự
		/// </summary>
		cblog_session *next;
	};

#ifdef __cplusplus
}
#endif

#endif