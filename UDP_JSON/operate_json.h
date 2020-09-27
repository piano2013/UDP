#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

#include "cJSON.h"

cJSON* open_cfgfile(const char* fname);
int close_cfgfile(cJSON* json);
cJSON* replace_value_from_cfgfile_with_class(cJSON* json, char* node, char* item, int replaceint);
cJSON* replace_value_from_cfgfile(cJSON* json, char* item, int replaceint);