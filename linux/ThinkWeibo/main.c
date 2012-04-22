
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

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

    DEBUG("Hello World!\n");
    printf("Hello World!!!\n");

    pweibo_token pToken = weibo_get_token("wangxf1985@gmail.com", "258685");
    if (pToken) {
        DEBUG("%s\n", pToken->m_szToken); 
        DEBUG("%s\n", pToken->m_szTokenSecret); 
        DEBUG("%s\n", pToken->m_szUserID); 
    }
    weibo_token_clean(pToken);

    return 0;
}
