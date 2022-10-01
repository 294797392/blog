
#ifndef __CJSON_HELPER_H__
#define __CJSON_HELPER_H__

#include <cJSON.h>

#ifdef __cplusplus
extern "C" {
#endif

    void cJSON_GetStringValue2(cJSON *cJSON, const char *key, char *string, size_t string_size, char *defval);
    int cJSON_GetIntValue2(cJSON *cJSON, const char *key, int defval);
    double cJSON_GetDoubleValue2(cJSON *cJSON, const char *key, double defval);

#ifdef __cplusplus
}
#endif



#endif
