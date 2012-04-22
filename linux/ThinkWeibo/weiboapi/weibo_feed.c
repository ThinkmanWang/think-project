
#include "weibo_feed.h"

#include <glib.h>

#include "log.h"

DEFINE_EA_MOD(MOD_NETWORK);

pweibo_feed weibo_feed_init()
{
    pweibo_feed pFeed = (pweibo_feed)g_malloc(sizeof(weibo_feed));
    pFeed->m_szCreatedat = NULL;
    pFeed->m_szID = NULL;
    pFeed->m_szText = NULL;
    pFeed->m_szSource = NULL;
    pFeed->m_bFavorited = FALSE;
    pFeed->m_bTruncated = FALSE;
    pFeed->m_szGeo = NULL;
    pFeed->m_szin_reply_to_status_id = NULL;
    pFeed->m_szin_reply_to_user_id = NULL;
    pFeed->m_szin_reply_to_screen_name = NULL;
    pFeed->m_retweeted = NULL;
    pFeed->m_pUser = NULL; //weibo owner
}

pweibo_feed weibo_feed_create(const char* szStr)
{
    json_error_t error;
    json_t* pJson = json_loads(szStr, JSON_DECODE_ANY, &error);
    if (!pJson) {
        PERR("could not parse json!\n");
        return NULL; 
    }

    return weibo_feed_create_json(pJson);
}

pweibo_feed weibo_feed_create_json(json_t* pJson)
{
    //POS();
    void *iter = json_object_iter(pJson);
    pweibo_feed pFeed = weibo_feed_init();
    while (iter) {
        const char* key = json_object_iter_key(iter);
        json_t *value = json_object_iter_value(iter);
        //DEBUG("%s->%d\n", key, value->type);
        if (0 == strcmp("created_at", key)) {
            pFeed->m_szCreatedat = g_string_new(json_string_value(value));
        } else if (0 == strcmp("id", key)) {
            pFeed->m_szID = g_string_new("");
            g_string_printf(pFeed->m_szID, "%"JSON_INTEGER_FORMAT, json_integer_value(value));

        } else if (0 == strcmp("text", key)) {
            pFeed->m_szText = g_string_new(json_string_value(value));
        } else if (0 == strcmp("source", key)) {
            pFeed->m_szSource = g_string_new(json_string_value(value));
        } else if (0 == strcmp("favorited", key)) {
        } else if (0 == strcmp("truncated", key)) {
        } else if (0 == strcmp("geo", key)) {
        } else if (0 == strcmp("in_reply_to_status_id", key)) {
            pFeed->m_szin_reply_to_status_id = g_string_new(json_string_value(value));
        } else if (0 == strcmp("in_reply_to_user_id", key)) {
            pFeed->m_szin_reply_to_user_id = g_string_new(json_string_value(value));
        } else if (0 == strcmp("in_reply_to_screen_name", key)) {
            pFeed->m_szin_reply_to_screen_name = g_string_new(json_string_value(value));
        } else if (0 == strcmp("retweeted_status", key)) {
            //json_t* status = json_load(json_string_value(value), JSON_DECODE_ANY, &error);
            pFeed->m_retweeted = weibo_feed_create_json(value);
        } else if (0 == strcmp("user", key)) {
            pFeed->m_pUser = weibo_user_create_json(value);
//        } else if (0 == strcmp("", key)) {
        }

        iter = json_object_iter_next(pJson, iter);
    }
    return pFeed;
}

pweibo_feed weibo_feed_clean(pweibo_feed pFeed)
{
    return NULL;
}

GList* weibo_feed_friends_timeline_create(const char* szStr)
{
    json_error_t error;
    json_t* pJson = json_loads(szStr, JSON_DECODE_ANY, &error);
    if (!pJson) {
        PERR("could not parse json!\n");
        return NULL; 
    }

    if (!json_is_array(pJson)) {
        PERR("could not parse json!\n");
        return NULL; 
    }

    GList* pFeedList = NULL;
    size_t nSize = json_array_size(pJson);
    int i;
    for (i = 0; i < nSize; i++) {
        json_t* feed = json_array_get(pJson, i);
        pweibo_feed pFeed = weibo_feed_create_json(feed);
        if (!pFeed) {
            PWARN("call weibo_feed_create_json failed\n");
            continue; 
        }

        pFeedList = g_list_append(pFeedList, pFeed);
    }

    json_decref(pJson);
    return pFeedList;
}

void weibo_feed_friends_timeline_clean(GList* pFeedList)
{
}
