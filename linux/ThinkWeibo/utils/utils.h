
#ifndef _WEIBO_STRING_UTILS_H_
#define _WEIBO_STRING_UTILS_H_

#ifdef __cplusplus
    extern "C" {
#endif


#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <openssl/md5.h>

#include "log.h"
#include "weiboapi.h"

GString* get_timestamp(GString* szTime); 

#ifdef __cplusplus
    }
#endif

#endif
