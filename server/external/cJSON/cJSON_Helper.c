#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cJSON.h>

void cJSON_GetStringValue2(cJSON *Object, const char *key, char *string, size_t string_size, char *defval)
{
    cJSON *json = cJSON_GetObjectItem(Object, key);
    if(json == NULL || !cJSON_IsString(json))
    {
        strncpy(string, defval, string_size);
    }
    else
    {
        strncpy(string, json->valuestring, string_size);
    }
}

int cJSON_GetIntValue2(cJSON *Object, const char *key, int defval)
{
    cJSON *json = cJSON_GetObjectItem(Object, key);
    if(json == NULL || !cJSON_IsNumber(json, key))
    {
        return defval;
    }
    else
    {
        return json->valueint;
    }
}

double cJSON_GetDoubleValue2(cJSON *Object, const char *key, double defval)
{
    cJSON *json = cJSON_GetObjectItem(Object, key);
    if(json == NULL || !cJSON_IsNumber(json, key))
    {
        return defval;
    }
    else
    {
        return json->valuedouble;
    }
}


