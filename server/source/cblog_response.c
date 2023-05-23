#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "cblog_response.h"
#include "cblog_factory.h"
#include "cblog_protocol.h"
#include "cblog_buffer.h"

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
