/***********************************************************************************
 * @ file    : cblog_types.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.10 19:06
 * @ brief   : ����cblogʹ�õ���Ҫ�����ݽṹ������ģ�鵥�����嵽ģ���ͷ�ļ���
 ************************************************************************************/

#ifndef __CBLOG_TYPES_H__
#define __CBLOG_TYPES_H__

#include "cblog_protocol.h"
#include "cblog_default.h"
#include "cblog_socket.h"
#include "cblog_parser.h"
#include "cblog_string.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct svchost_s svchost;
	typedef struct svchost_options_s svchost_options;
	typedef enum cblog_conn_status_enum cblog_conn_status_enum;
	typedef struct cblog_connection_s cblog_connection;
	typedef struct cblog_connection_options_s cblog_connection_options;
	typedef struct cblog_http_header_s cblog_http_header;
	typedef struct cblog_http_headers_s cblog_http_headers;
	typedef struct cblog_request_s cblog_request;
	typedef struct cblog_pending_response_s cblog_pending_response;
	typedef struct cblog_pending_response_chain_s cblog_pending_response_chain;
	typedef struct cblog_response_s cblog_response;
	typedef struct cblog_session_s cblog_session;

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
		cblog_http_header *next;
		cblog_http_header *prev;

		cblog_string key;
		cblog_string value;
	};

	struct cblog_http_headers_s
	{
		cblog_http_header *first;
		cblog_http_header *last;
		int count;
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
		cblog_http_method_enum method;
		cblog_string method_string;

		/// <summary>
		/// HTTP�汾��
		/// </summary>
		cblog_http_version_enum version;
		cblog_string version_string;
		cblog_string url;
		cblog_string body;

		/// <summary>
		/// HTTP��ͷ����
		/// </summary>
		cblog_http_headers *header_chain;
	};

	/// <summary>
	/// ����Ӧ����
	/// ��Ҫ���͸��ͻ��˵�����
	/// </summary>
	struct cblog_pending_response_s
	{
		cblog_pending_response *prev;
		cblog_pending_response *next;

		char *buffer;
		int buflen;
	};

	/// <summary>
	/// ����Ӧ�б�
	/// ��ʾҪ���͸��ͻ��˵���Ӧ�б�
	/// ���ǵ�send��recv������ô��ʱ���ܴ洢�˶�������û����Ӧ
	/// Ϊ�˽��������⣬�����������Ӧ����Ӧ�洢��һ���б�����������
	/// </summary>
	struct cblog_pending_response_chain_s
	{
		cblog_pending_response *first;
		cblog_pending_response *last;
		int count;
	};

	/// <summary>
	/// ��¼HTTP��Ӧ����ϸ��Ϣ
	/// </summary>
	struct cblog_response_s
	{
		/// <summary>
		/// �洢����HTTP��ͷ�Ļ�����
		/// </summary>
		cblog_buffer *header_buffer;

		/// <summary>
		/// HTTP��ͷ����
		/// </summary>
		cblog_http_headers *header_chain;

		/// <summary>
		/// HTTP�汾��
		/// </summary>
		cblog_http_version_enum version;

		/// <summary>
		/// HTTP״̬��
		/// </summary>
		cblog_http_status_code_enum status_code;

		/// <summary>
		/// body������
		/// </summary>
		cblog_buffer *body_buffer;

		/// <summary>
		/// �洢ԭʼHTTP��Ӧ���ĵĻ�����
		/// </summary>
		cblog_buffer *buffer;
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

	/// <summary>
	/// HTTP���ӵ�ѡ�����
	/// </summary>
	struct cblog_connection_options_s
	{
		/// <summary>
		/// �Ƿ񱣴�ԭʼ��HTTP������
		/// </summary>
		int save_request;

		/// <summary>
		/// ����HTTP�����ĵ��ļ�·��
		/// </summary>
		char save_request_path[256];
	};

	/// <summary>
	/// ��¼��ͻ��˵�HTTP����״̬
	/// ��¼һ��HTTP���ӵ�״̬
	/// </summary>
	struct cblog_connection_s
	{
		/// <summary>
		/// �ͻ��˵�socket
		/// </summary>
		cblog_socket sock;

		/// <summary>
		/// �����ӵ�ѡ��
		/// </summary>
		cblog_connection_options options;

		/// <summary>
		/// HTTP���Ľ�����
		/// </summary>
		cblog_parser parser;

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
		/// ��ʾҪ���͸��ͻ��˵���Ӧ�б�
		/// ���ǵ�send��recv�����������ô��ʱ���ܴ洢�˶�������û����Ӧ
		/// Ϊ�˽��������⣬�����������Ӧ����Ӧ�洢��һ���б�����������
		/// </summary>
		cblog_pending_response_chain *pending_response_list;

		/// <summary>
		/// ���ջ�����
		/// ���浱ǰrequest��ԭʼHTTP����
		/// </summary>
		cblog_buffer *recvbuf;

		/// <summary>
		/// �����ӵ�״̬
		/// </summary>
		cblog_conn_status_enum status;

		/// <summary>
		/// Connection��ͷ�Ƿ���Ҫkeep-alive
		/// </summary>
		int keep_alive;
	};

#ifdef __cplusplus
}
#endif

#endif