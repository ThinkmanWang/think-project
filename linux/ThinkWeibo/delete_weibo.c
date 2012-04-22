
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "log.h"
#include "weiboapi.h"

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

    if (argc < 2) {
        DEBUG("Usage %s ID", argv[0]);
        return -1;
    }

    CHECK_WEIBO_TOKEN();
    DEBUG("%s\n", pToken->m_szToken); 
    DEBUG("%s\n", pToken->m_szTokenSecret); 
    DEBUG("%s\n", pToken->m_szUserID); 


    weibo_destroy(g_string_new(argv[1]));

    return 0;
}
