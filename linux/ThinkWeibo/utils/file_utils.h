
#ifndef _WEIBO_FILE_UTILS_H_
#define _WEIBO_FILE_UTILS_H_

#ifdef __cplusplus
    extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <openssl/md5.h>

#include "log.h"

GString* get_app_root();
char *dirname(const char *path);

#ifdef __cplusplus
    }
#endif

#endif
