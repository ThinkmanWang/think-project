
#include "weibo_token.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"
#include "weibo_config.h"

DEFINE_EA_MOD(MOD_NETWORK);

pweibo_token weibo_token_create(char* szStr)
{
    POS();
    //DEBUG("%s\n", szStr);
    pweibo_token pToken = (pweibo_token)malloc(sizeof(weibo_token));

    char* szTemp = NULL;
    szTemp = strtok(szStr,"&");
    while (szTemp) {
        if (strstr(szTemp, "oauth_token=")) {
            //DEBUG("%s\n", szTemp);
            if (1 != sscanf(szTemp, "oauth_token=%s", pToken->m_szToken)) {
                PERR("weibo_token_create error\n");
                free(pToken);
                return NULL;
            }
            //DEBUG("%s\n", pToken->m_szToken);
        }
        else if (strstr(szTemp, "oauth_token_secret=")) {
            //DEBUG("%s\n", szTemp);
            if (1 != sscanf(szTemp, "oauth_token_secret=%s", pToken->m_szTokenSecret)) {
                PERR("weibo_token_create error\n");
                free(pToken);
                return NULL;
            }
            //DEBUG("%s\n", pToken->m_szTokenSecret);
        }
        else if (strstr(szTemp, "user_id=")) {
            //DEBUG("%s\n", szTemp);
            if (1 != sscanf(szTemp, "user_id=%s", pToken->m_szUserID)) {
                PERR("weibo_token_create error\n");
                free(pToken);
                return NULL;
            }
            //DEBUG("%s\n", pToken->m_szUserID);
        }
        szTemp = strtok (NULL, "&");
    }

    return pToken;
}

void weibo_token_clean(pweibo_token pToken)
{
    free(pToken);
    pToken = NULL;
}

pweibo_token weibo_token_load()
{
    char* szHome = getenv("HOME");
    char szThinkWeiboAccount[256];
    sprintf(szThinkWeiboAccount, "%s/%s/%s", szHome, WEIBO_HOME, WEIBO_TOKEN_FILE);
    int fdFile = open(szThinkWeiboAccount, O_RDONLY, 0600);
    if (fdFile < 0) {
        PERR("Create file failed!\n");
        goto err_ret1;
    }

    char szToken[512]; 
    int nRead = read(fdFile, szToken, 512);
    DEBUG("%s\n", szToken);
    return weibo_token_create(szToken);

err_ret1:
    g_free(szHome);
    return NULL;
}
