
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <openssl/md5.h>

#include "log.h"
#include "weiboapi.h"
#include "downloader.h"
#include "string_utils.h"

DEFINE_EA_MOD(MOD_MAIN);

int main(int argc,char *argv[])
{
    ELInit(NULL, log_module);
    ELSetDefaultLogTable(EA_LOG_DEBUG);
    ELSetModuleLogLevel(MOD_MAIN, EA_LOG_DEBUG);
    ELSetModuleLogLevel(MOD_UI, EA_LOG_DEBUG);
    ELSetModuleLogLevel(MOD_DATA, EA_LOG_DEBUG);
    ELSetModuleLogLevel(MOD_NETWORK, EA_LOG_DEBUG);
    ELPrintLogTable();

    if (argc != 3) {
        DEBUG("usage: %s url file", argv[0]);
        return -1;
    }


    GString* szUrl = g_string_new(argv[1]);
    GString* szPath = g_string_new(argv[2]);
    GString* szPathRet = download(szUrl, szPath);
    if (szPathRet) {
        DEBUG("%s\n", szPathRet->str);
    }

    GString* szMd5 = get_md5(szUrl); 
    DEBUG("%s\n", szMd5->str);

    return 0;
}
