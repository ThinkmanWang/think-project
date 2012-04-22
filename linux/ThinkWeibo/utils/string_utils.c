
#include "string_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <openssl/md5.h>

#include "log.h"
#include "weiboapi.h"
#include "downloader.h"

DEFINE_EA_MOD(MOD_DATA);

static char* hash_to_str(const unsigned char* key, int len, char * str)
{
    g_return_val_if_fail(key != NULL, NULL);
    g_return_val_if_fail(str != NULL, NULL);
    int i = 0;
    char *buf = str;

    for (i = 0; i < len; i++) {
        sprintf(buf,"%02x",(unsigned char)(key[i]) );
        buf++;
        sprintf(buf,"%02x",(unsigned char)(key[i]<<4));
        buf++;
    }
    *buf = '\0';
    return str;
}

GString* get_md5(GString* szStr)
{
    unsigned char *p = NULL;
    unsigned char key[16];
    char md5_str[40];
    p = MD5((unsigned char*)szStr->str, strlen(szStr->str), key);

    if (!p) {
        PERR("get md5 failed\n");
        return NULL;
    }

    hash_to_str(key, 16, md5_str);
    return g_string_new(md5_str);
}
