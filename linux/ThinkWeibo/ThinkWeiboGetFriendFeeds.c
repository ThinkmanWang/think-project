
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

    pweibo_token pToken = weibo_token_load();
    if (!pToken) {
        PERR("Login failed!\n");
        return -1;
    }

    DEBUG("%s\n", pToken->m_szToken); 
    DEBUG("%s\n", pToken->m_szTokenSecret); 
    DEBUG("%s\n", pToken->m_szUserID); 

    GList* pFeedList = weibo_get_friend_feeds(NULL, NULL, 20, 1, WEIBO_FEED_TYPE_ALL);
    PERR("%d\n", pFeedList);
    if (!pFeedList) {
        PERR("Parse feeds failed!\n");
        return -1;
    }
    DEBUG("Feed count: %d\n", g_list_length(pFeedList));

    int i, nSize = g_list_length(pFeedList);
    for (i = 0; i < nSize; ++i) {
        pweibo_feed pFeed = (pweibo_feed)g_list_nth_data(pFeedList, i);
        pweibo_user pUser = (pweibo_user)pFeed->m_pUser;
        DEBUG("[%s %s]-->%s-->%s\n", pUser->m_szID->str, pUser->m_szScreenName->str, pFeed->m_szID->str, pFeed->m_szText->str);
        if (pFeed->m_retweeted) {
            pweibo_feed retweeted = (pweibo_feed)pFeed->m_retweeted;
            if (retweeted->m_szID && retweeted->m_szText) {
                PWARN("repost  %s-->%s\n", retweeted->m_szID->str, retweeted->m_szText->str);
            }
        }
    }

    weibo_token_clean(pToken);
    return 0;
}
