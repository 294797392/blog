#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "cblog_response.h"
#include "cblog_factory.h"
#include "cblog_protocol.h"


static void write_header(cblog_buffer *buffer, cblog_http_header *header, const char *k, int klen, const char *v, int vlen)
{
	cblog_string *key = &header->key;
	cblog_string *value = &header->value;

	// Ð´key
	key->buffer = buffer;
	key->offset = buffer->offset;
	key->length = klen;
	cblog_buffer_write(buffer, k, klen);

	//// Ð´Ã°ºÅ·Ö¸ô·û
	//cblog_buffer_write(buffer, ": ", 2);

	// Ð´value
	value->buffer = buffer;
	value->offset = buffer->offset;
	value->length = vlen;
	cblog_buffer_write(buffer, v, vlen);

	//// Ð´»»ÐÐ·û
	//cblog_buffer_write(buffer, "\r\n", 2);
}


void cblog_response_write_header_string(cblog_response *response, cblog_http_header_enum k, char *v, int vlen)
{
	cblog_http_header *header = new_cblog_http_header();

	const char *key = cblog_http_header_string(k);
	int keylen = strlen(key);
	write_header(response->header_buffer, header, key, keylen, v, vlen);

	Y_chain_add(cblog_http_header, response->header_chain, header);
}

void cblog_response_write_header_int(cblog_response *response, cblog_http_header_enum k, int v)
{
	cblog_http_header *header = new_cblog_http_header();

	const char *key = cblog_http_header_string(k);
	int keylen = strlen(key);
	char value[64] = { '\0' };
	snprintf(value, sizeof(value), "%d", v);
	int valuelen = strlen(value);
	write_header(response->header_buffer, header, key, keylen, value, valuelen);

	Y_chain_add(cblog_http_header, response->header_chain, header);
}

cblog_pending_response *new_cblog_pending_response(cblog_response *response)
{
	cblog_pending_response *pending_response = (cblog_pending_response *)calloc(1, sizeof(cblog_pending_response));
	cblog_buffer *buffer = new_cblog_buffer(CBLOG_DEFAULT_RESPONSE_BUFFER_SIZE);
	pending_response->buffer = buffer;

	// ¹¹ÔìHTTPÏìÓ¦±¨ÎÄ
	const char *http_ver = cblog_http_version_string(response->version);
	const char *status_text = cblog_http_status_code_string(response->status_code);
	char status_line[256] = { '\0' };
	snprintf(status_line, sizeof(status_line), "%s %d %s\r\n", http_ver, response->status_code, status_text);

	// Ð´×´Ì¬ÐÐ
	cblog_buffer_write(buffer, status_line, strlen(status_line));

	// Ð´±êÍ·
	Y_chain_foreach(cblog_http_header, response->header_chain,
		{
			cblog_buffer_write2(buffer, &current->key);
	cblog_buffer_write(buffer, ": ", 2);
	cblog_buffer_write2(buffer, &current->value);
	cblog_buffer_write(buffer, "\r\n", 2);
		});

	// Ð´¿ÕÐÐ
	cblog_buffer_write(buffer, "\r\n", 2);

	// Ð´body
	cblog_buffer_write2(buffer, response->body_buffer);

	return pending_response;
}

void free_cblog_pending_response(cblog_pending_response *response)
{
	free_cblog_buffer(response->buffer);
	free(response);
}
