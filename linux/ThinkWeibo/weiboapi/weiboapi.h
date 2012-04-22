
#ifndef _WEIBOAPI_H_
#define _WEIBOAPI_H_

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdio.h> 
#include <string.h> 
#include <curl/curl.h> 

#include "weibo_config.h"
#include "weibo_token.h"
#include "weibo_user.h"
#include "weibo_feed.h"

#define CHECK_WEIBO_TOKEN() \
    pweibo_token pToken = weibo_token_load(); \
    if (!pToken) { \
        PERR("Load token failed!\n"); \
        return NULL; \
    }

CURLcode get_weibo_trends(CURL* pCurl, char* szBuf, size_t nBufSize);
CURLcode submit_weibo_with_url_image(CURL* pCurl, char* szContent, char* szImageUrl);
CURLcode submit_weibo_with_url_image1(char* szContent, char* szImageUrl);

pweibo_token weibo_get_token(const char* szUser, const char* szPwd);
pweibo_user weibo_get_my_profile();
GList* weibo_get_friend_feeds(const char* szSinceID, const char* szMaxID, int nCount, int nPage, weibo_feed_type type);
GList* weibo_get_at_me(const char* szSinceID, const char* szMaxID, int nCount, int nPage);
GList* weibo_get_user_weibo(const char* szUserId, const char* szSinceID, const char* szMaxID, int nCount, int nPage, int nBaseApp, weibo_feed_type type);
GString* submit_weibo_with_image(GString* szStatus, GString* szPic, GString* szLat, GString* szLong);
pweibo_feed weibo_show(const GString* szID);
pweibo_feed weibo_destroy(const GString* szID);
pweibo_feed weibo_update(GString* szStatus, GString* in_reply_to_status_id, GString* szLat, GString* szLong, GString* annotations);

#ifdef __cplusplus
    }
#endif

#endif
