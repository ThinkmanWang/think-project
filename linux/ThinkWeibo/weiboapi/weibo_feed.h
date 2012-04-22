
#ifndef _WEIBO_FEED_H_
#define _WEIBO_FEED_H_

#ifdef __cplusplus
    extern "C" {
#endif
#include <glib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <jansson.h>
#include "weibo_user.h"

typedef enum {
    WEIBO_FEED_TYPE_ALL,
    WEIBO_FEED_TYPE_ORIGINAL,
    WEIBO_FEED_TYPE_IMAGE,
    WEIBO_FEED_TYPE_VIDEO,
    WEIBO_FEED_TYPE_MUSIC
}weibo_feed_type;

typedef struct {
    GString* m_szCreatedat;
    GString* m_szID;
    GString* m_szText;
    GString* m_szSource;
    gboolean m_bFavorited;
    gboolean m_bTruncated;
    GString* m_szGeo;
    GString* m_szin_reply_to_status_id;
    GString* m_szin_reply_to_user_id;
    GString* m_szin_reply_to_screen_name;
    gpointer m_retweeted;
    gpointer m_pUser;
}weibo_feed, *pweibo_feed;

pweibo_feed weibo_feed_create(const char* szStr);
pweibo_feed weibo_feed_create_json(json_t* obj);
pweibo_feed weibo_feed_clean(pweibo_feed pFeed);
GList* weibo_feed_friends_timeline_create(const char* szStr);
void weibo_feed_friends_timeline_clean(GList* pFeedList);


#ifdef __cplusplus
    }
#endif

#endif
