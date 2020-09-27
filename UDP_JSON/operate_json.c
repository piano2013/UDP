#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

#include "cJSON.h"
#include "operate_json.h"

cJSON* open_cfgfile(const char* fname){
    FILE* fp;
    cJSON* root;
    int nbyte, size;

    fp = fopen(fname, "rb");
    if (fp == NULL) {
        perror("fopen");
        return NULL;
    }
    char* src;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    src = malloc(size + 1);
    nbyte = fread(src, 1, size, fp);
    if (nbyte < 0) {
        free(src);
        return NULL;
    }
    cJSON_Minify(src);
    // 解析数据包
    root = cJSON_Parse(src);
    if (!root){
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return NULL;
    }

    free(src);
    fclose(fp);

    return root;
}

int close_cfgfile(cJSON* json){
    // 释放内存空间
    cJSON_Delete(json);

    return 0;
}

cJSON* replace_value_from_cfgfile_with_class(cJSON* json, char* node, char* item, int replaceint){
    cJSON* json_value, * root;
    root = cJSON_GetObjectItem(json, node);
    if (!root)
        return NULL;

    json_value = cJSON_GetObjectItem(root, item);
    if (json_value && json_value->type == cJSON_Number){
        json_value->valueint = replaceint;
        json_value->valuedouble = replaceint;
    }
    else
        return NULL;

    return json;
}

cJSON* replace_value_from_cfgfile(cJSON* json, char* item, int replaceint){
    cJSON* json_value;
    json_value = cJSON_GetObjectItem(json, item);
    if (!json_value)
        return NULL;

    if (json_value && json_value->type == cJSON_Number){
        json_value->valueint = replaceint;
        json_value->valuedouble = replaceint;
    }
    else
        return NULL;

    return json;
}