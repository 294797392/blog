#ifndef __MM_READ_HEADERS_H__
#define __MM_READ_HEADERS_H__

// 表示http头部的数据类型
typedef enum mm_header_type
{
    MM_HDR_CONTENT_TYPE,
    MM_HDR_CONTENT_TYPE
};

// 存储一个http头部的信息
typedef struct mm_header_s
{
    enum mm_header_type type;
    char *value;
}mm_header;

int mm_read_headers(mm_header **header, int *count);

#endif