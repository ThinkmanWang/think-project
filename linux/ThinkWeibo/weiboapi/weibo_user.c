
#include "weibo_user.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <jansson.h>

#include <pthread.h>
#include <oauth.h>
#include "log.h"

DEFINE_EA_MOD(MOD_NETWORK);

pweibo_user weibo_user_init()
{
    pweibo_user pUser = (pweibo_user)g_malloc(sizeof(weibo_user));

    pUser->m_szID = NULL;
    pUser->m_szScreenName = NULL;
    pUser->m_szName = NULL;
    pUser->m_nProvince = 0;
    pUser->m_nCity = 0;
    pUser->m_szLocation = NULL;
    pUser->m_szDescription = NULL;
    pUser->m_szUrl = NULL;
    pUser->m_szProfileImageUrl = NULL;
    pUser->m_szDomain = NULL;
    pUser->m_szGender = NULL;
    pUser->m_nFollowersCount = 0;
    pUser->m_nFriendsCount = 0;
    pUser->m_nStatusesCount = 0;
    pUser->m_nFavouritesCount = 0;
    pUser->m_szCreatedAt = NULL;
    pUser->m_bFollowing = FALSE;
    pUser->m_bAllowAllActMsg = FALSE;
    pUser->m_bGeoEnabled = FALSE;
    pUser->m_bVerified = FALSE;
    pUser->m_pFeed = NULL;
    
    return pUser;
}

pweibo_user weibo_user_create(const char* szStr)
{
    json_error_t error;
    json_t* pJson = json_loads(szStr, JSON_DECODE_ANY, &error);

    return weibo_user_create_json(pJson);
}

pweibo_user weibo_user_create_json(json_t* pJson)
{
    if (!pJson) {
        PERR("could not parse json!\n");
        return NULL; 
    }

    void *iter = json_object_iter(pJson);
    pweibo_user pUser = weibo_user_init();

    while (iter) {
        const char* key = json_object_iter_key(iter);
        //DEBUG("%s\n", key);
        json_t *value = json_object_iter_value(iter);
        if(!value) {
            iter = json_object_iter_next(pJson, iter);
            continue;
        }
        //DEBUG("%d\n", value->type);

        if (0 == strcmp("id", key)) {
            pUser->m_szID = g_string_new("");
            g_string_printf(pUser->m_szID, "%"JSON_INTEGER_FORMAT, json_integer_value(value));
        } else if (0 == strcmp("screen_name", key)) {
            pUser->m_szScreenName = g_string_new(json_string_value(value));
        } else if (0 == strcmp("name", key)) {
            pUser->m_szName = g_string_new(json_string_value(value));
        } else if (0 == strcmp("province", key)) {
            pUser->m_nProvince = json_integer_value(value);
        } else if (0 == strcmp("city", key)) {
            pUser->m_nCity = json_integer_value(value);
        } else if (0 == strcmp("location", key)) {
            pUser->m_szLocation = g_string_new(json_string_value(value));
        } else if (0 == strcmp("description", key)) {
            pUser->m_szDescription = g_string_new(json_string_value(value));
        } else if (0 == strcmp("url", key)) {
            pUser->m_szUrl = g_string_new(json_string_value(value));
        } else if (0 == strcmp("profile_image_url", key)) {
            pUser->m_szProfileImageUrl = g_string_new(json_string_value(value));
        } else if (0 == strcmp("domain", key)) {
            pUser->m_szDomain = g_string_new(json_string_value(value));
        } else if (0 == strcmp("gender", key)) {
            pUser->m_szGender = g_string_new(json_string_value(value));
        } else if (0 == strcmp("followers_count", key)) {
            pUser->m_nFollowersCount = json_integer_value(value);
        } else if (0 == strcmp("friends_count", key)) {
            pUser->m_nFriendsCount = json_integer_value(value);
        } else if (0 == strcmp("statuses_count", key)) {
            pUser->m_nStatusesCount = json_integer_value(value);
        } else if (0 == strcmp("favourites_count", key)) {
            pUser->m_nFavouritesCount = json_integer_value(value);
        } else if (0 == strcmp("created_at", key)) {
            pUser->m_szCreatedAt = g_string_new(json_string_value(value));
        } else if (0 == strcmp("following", key)) {
            pUser->m_bFollowing = json_is_true(value) ? TRUE : FALSE;
        } else if (0 == strcmp("allow_all_act_msg", key)) {
            pUser->m_bAllowAllActMsg = json_is_true(value) ? TRUE : FALSE;
        } else if (0 == strcmp("geo_enabled", key)) {
            pUser->m_bGeoEnabled = json_is_true(value) ? TRUE : FALSE;
        } else if (0 == strcmp("verified", key)) {
            pUser->m_bVerified = json_is_true(value) ? TRUE : FALSE;
        } else if (0 == strcmp("status", key)) {
            PERR("%s-->%d\n", key, value->type);
            POS();
            pUser->m_pFeed = weibo_feed_create_json(value);
            PERR("%d\n", pUser->m_pFeed);
            PERR("%s\n", ((pweibo_feed)(pUser->m_pFeed))->m_szID->str);
        }

        iter = json_object_iter_next(pJson, iter);
    }
    json_decref(pJson);

    return pUser;
}

void weibo_user_clean(pweibo_user pUser)
{
}

void weibo_user_print(pweibo_user pUser)
{
    POS();
    if (NULL == pUser) {
        return;
    }

    DEBUG("%s\n", pUser->m_szID->str);
    DEBUG("%s\n", pUser->m_szName->str);
    DEBUG("%s\n", pUser->m_szScreenName->str);
    DEBUG("%s\n", pUser->m_szLocation->str);
    DEBUG("%s\n", pUser->m_szDescription->str);
    DEBUG("%s\n", pUser->m_szProfileImageUrl->str);
    DEBUG("%s\n", pUser->m_szUrl->str);
    if (pUser->m_pFeed && ((pweibo_feed)(pUser->m_pFeed))->m_szText) {
        DEBUG("%s\n", ((pweibo_feed)(pUser->m_pFeed))->m_szText->str);
    }
}
