
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
    if (argc != 3) {
        printf("usage: \n");
        return -1;
    }

    ELInit(NULL, log_module);
	ELSetDefaultLogTable(EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_MAIN, EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_UI, EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_DATA, EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_NETWORK, EA_LOG_DEBUG);
	ELPrintLogTable();


    pweibo_token pToken = weibo_get_token(argv[1], argv[2]);
    if (!pToken) {
        PERR("Login failed!\n");
        return -1;
    }

    DEBUG("%s\n", pToken->m_szToken); 
    DEBUG("%s\n", pToken->m_szTokenSecret); 
    DEBUG("%s\n", pToken->m_szUserID); 

    char szAccountInfo[512];
    sprintf(szAccountInfo, "oauth_token=%s&oauth_token_secret=%s&user_id=%s\n"
            , pToken->m_szToken
            , pToken->m_szTokenSecret
            , pToken->m_szUserID);

    char* szHome = getenv("HOME");
    DEBUG("%s\n", szHome);
    char szThinkWeiboHome[256];
    sprintf(szThinkWeiboHome, "%s/.ThinkWeibo/", szHome);
    DEBUG("%s\n", szThinkWeiboHome);

    DEBUG("%s\n", szAccountInfo);
    if (-1 == g_mkdir_with_parents(szThinkWeiboHome, 0600)) {
        PERR("mkdir failed!\n");
        goto err_ret2;
    }

    char szThinkWeiboAccount[256];
    sprintf(szThinkWeiboAccount, "%stoken", szThinkWeiboHome);
    DEBUG("%s\n", szThinkWeiboAccount);
    int fdFile = open(szThinkWeiboAccount, O_CREAT|O_WRONLY, 0600);
    if (fdFile < 0) {
        PERR("Create file failed!\n");
        goto err_ret2;
    }

    write(fdFile, szAccountInfo, strlen(szAccountInfo));
    close(fdFile);

    DEBUG("Login Success!\n");
    weibo_token_clean(pToken);

    pweibo_token pToken1 = weibo_token_load();
    if (!pToken1) {
        PERR("Login failed!\n");
        return -1;
    }

    DEBUG("%s\n", pToken1->m_szToken); 
    DEBUG("%s\n", pToken1->m_szTokenSecret); 
    DEBUG("%s\n", pToken1->m_szUserID); 

    //get user info
    pweibo_user pUser = weibo_get_my_profile();
    if (pUser) {
        weibo_user_print(pUser);  
    }
    return 0;

err_ret2:
    weibo_token_clean(pToken);

err_ret1:
    g_free(szHome);
    return -1;
}
