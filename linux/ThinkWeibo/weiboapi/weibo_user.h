
#ifndef _WEIBO_USER_H_
#define _WEIBO_USER_H_

#ifdef __cplusplus
    extern "C" {
#endif
#include <glib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "weibo_feed.h"

#if 1
typedef struct {
    GString* m_szID;
    GString* m_szScreenName;
    GString* m_szName;
    glong m_nProvince;
    glong m_nCity;
    GString* m_szLocation;
    GString* m_szDescription;
    GString* m_szUrl;
    GString* m_szProfileImageUrl;
    GString* m_szDomain;
    GString* m_szGender;
    glong m_nFollowersCount;
    glong m_nFriendsCount;
    glong m_nStatusesCount;
    glong m_nFavouritesCount;
    GString* m_szCreatedAt;
    gboolean m_bFollowing;
    gboolean m_bAllowAllActMsg;
    gboolean m_bGeoEnabled;
    gboolean m_bVerified;
    //last weibo_status
    gpointer m_pFeed;
}weibo_user, *pweibo_user;
#else
typedef struct {
    GString m_szID;
    GString m_szName;
    GString m_szScreenName;
    GString m_szLocation;
    GString m_szDescription;
    GString m_szProfileImageUrl;
    GString m_szUrl;
    gboolean m_bIsProtected;
    gint m_nFollerCount;
    struct timeval m_tStatusCreatedAt;
    GString m_szStatusId;
    GString m_szStatusText;
    GString m_szStatusSource;
    gboolean m_bStatusTruncated;
    GString m_szStatusInReplyToStatusId;
    GString m_szStatusInReplyToUserId;
    gboolean m_bStatusFavorited;
    GString m_szStatusInReplyToScreenName;
    GString m_szProfileBackgroundColor;
    GString m_szProfileTextColor;
    GString m_szProfileLinkColor;
    GString m_szProfileSidebarFillColor;
    GString m_szProfileSidebarBorderColor;
    gint m_nFriendsCount;
    struct tm m_tmCreatedAt;
    gint m_nFavouritesCount;
    gint m_nUtcOffset;
    GString m_szTimeZone;
    GString m_szProfileBackgroundImageUrl;
    GString m_szProfileBackgroundTile;
    gboolean m_bFollowing;
    gboolean m_bNotificationEnabled;
    gint m_nStatusesCount;
    gboolean m_bGeoEnabled;
    gboolean m_bVerified;
    GString m_szUserDomain;
    glong m_lSerialVersionUID; //always -6345893237975349030L
}weibo_user, *pweibo_user;
#endif

pweibo_user weibo_user_create(const char* szStr);
pweibo_user weibo_user_create_json(json_t* obj);
void weibo_user_clean(pweibo_user pUser);
void weibo_user_print(pweibo_user pUser);

#ifdef __cplusplus
    }
#endif

#endif
