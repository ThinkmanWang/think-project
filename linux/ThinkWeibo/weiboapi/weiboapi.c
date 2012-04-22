
#include "weiboapi.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <jansson.h>

#include <pthread.h>
#include <oauth.h>
#include "log.h"
#include "weibo_token.h"
#include "../utils/file_utils.h"

static char wr_buf[MAX_BUF+1]; 
static int  wr_index; 
pthread_mutex_t g_mutex=PTHREAD_MUTEX_INITIALIZER;

DEFINE_EA_MOD(MOD_NETWORK);

/////////////////////////////////////////////////////////////////////////
static char* weibo_do_oauth_request(char* szUrl, char* szToken, char* szTokenSecret, gboolean bPost);

typedef struct {
    GString* m_szFullUrl;
    gint m_nTime;
}weibo_do_api_t, *pweibo_do_api_t;

static void iterator(gpointer key, gpointer value, gpointer user_data);
#define ENABLE_WEIBO_DO_API
static char* weibo_do_api(char* szApi, GHashTable* pArgs, char* szToken, char* szTokenSecret, gboolean bPost);
/////////////////////////////////////////////////////////////////////////

/* 
 * Write data callback function (called within the context of 
 * curl_easy_perform. 
 */ 
static size_t write_data( void *buffer, size_t size, size_t nmemb, void *userp ) 
{ 
    int segsize = size * nmemb; 

    /* Check to see if this data exceeds the size of our buffer. If so, 
     * set the user-defined context value and return 0 to indicate a 
     * problem to curl. 
     */ 
    if ( wr_index + segsize > MAX_BUF ) { 
        *(int *)userp = 1; 
        return 0; 
    } 

    /* Copy the data from the curl buffer into our buffer */ 
    memcpy( (void *)&wr_buf[wr_index], buffer, (size_t)segsize ); 

    /* Update the write index */ 
    wr_index += segsize; 

    /* Null terminate the buffer */ 
    wr_buf[wr_index] = 0; 

    /* Return the number of bytes received, indicating to curl that all is okay */ 
    return segsize; 
} 

CURLcode get_weibo_trends(CURL* pCurl, char* szBuf, size_t nBufSize)
{
    POS();
    pthread_mutex_lock(&g_mutex);
    /* Tell curl the URL of the file we're going to retrieve */ 
    curl_easy_setopt(pCurl, CURLOPT_URL, TREND_URL ); 

    /* Tell curl that we'll receive data to the function write_data, and 
     * also provide it with a context pointer for our error return. 
     */ 
    int  wr_error = 0; 
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, (void *)&wr_error ); 
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data ); 

    /* Allow curl to perform the action */ 
    CURLcode ret = curl_easy_perform(pCurl); 

    if (CURLE_OK == ret) {
        memcpy(szBuf, wr_buf, nBufSize);
    }
    pthread_mutex_unlock(&g_mutex);

    return ret;
}


CURLcode submit_weibo_with_url_image(CURL* pCurl, char* szContent, char* szImageUrl)
{
    POS();
    pCurl = curl_easy_init();
    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;

    static const char buf[] = "Content-Type:content_type";
    headerlist = curl_slist_append(headerlist, buf);
    curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER,  0L );
    curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST,  0L );
    curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, NULL);
    curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(pCurl, CURLOPT_URL, "http://api.t.sina.com.cn/statuses/update.json");
    curl_easy_setopt(pCurl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); 
    curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, "access_token=18d2d221a6bc0aa8743d3f30d6745b7e&status=teeeest1314353455");
    curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, strlen("access_token=18d2d221a6bc0aa8743d3f30d6745b7e&status=teeeest1314353455")); 
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data );
    CURLcode ret = curl_easy_perform(pCurl);
    DEBUG("%d\n", ret);
    //DEBUG("%d\n", wr_buf);
    curl_easy_cleanup( pCurl );

    return ret;
}

CURLcode submit_weibo_with_url_image1(char* szContent, char* szImageUrl)
{
    POS();
    char *postargs = NULL;
    char* post = NULL;
    post = oauth_sign_url2("http://api.t.sina.com.cn/statuses/update.json?status=teeeest1314353455", &postargs, OA_HMAC, NULL, "3551174478", "c53f4123bdb7b39a0ae03e400b839934", "18d2d221a6bc0aa8743d3f30d6745b7e", "2de591d207b213e4f46a1d1170413367");

    DEBUG("%s\n", post);
    DEBUG("%s\n", postargs);
    char *reply = oauth_http_post(post,postargs);
    if(reply){
        DEBUG("%s\n", reply);
    }

    return 0;
}

pweibo_token weibo_get_token(const char* szUser, const char* szPwd)
{
    POS();
    GHashTable* pArgsTable = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(pArgsTable, "x_auth_username", szUser);
    g_hash_table_insert(pArgsTable, "x_auth_password", szPwd);
    g_hash_table_insert(pArgsTable, "x_auth_mode", "client_auth");

    char* reply =  weibo_do_api("oauth/access_token", pArgsTable, "", "", TRUE);
    if(!reply){
        return NULL;
    }

    DEBUG("%s\n", reply);
    return weibo_token_create(reply);
}

static char* weibo_do_oauth_request(char* szUrl, char* szToken, char* szTokenSecret, gboolean bPost)
{
    POS();
    char *postargs = NULL;
    char* post = NULL;
    post = oauth_sign_url2(
            szUrl
            , &postargs
            , OA_HMAC
            , NULL
            , APP_KEY
            , APP_SECRET
            , szToken
            , szTokenSecret);

    char *reply = oauth_http_post(post,postargs);
    return reply;
    if (bPost) {
        return oauth_http_post(post,postargs);  
    } else {
        return oauth_http_get(post,postargs);       
    }
}

static void iterator(gpointer key, gpointer value, gpointer user_data)
{
    g_return_if_fail(user_data != NULL);
    pweibo_do_api_t pUserData = user_data;
    pUserData->m_nTime++;

    g_return_if_fail(key != NULL);
    g_return_if_fail(value != NULL);

    DEBUG("%s --> %s\n", key, value);
    if (1 == pUserData->m_nTime) {
        pUserData->m_szFullUrl = g_string_append(pUserData->m_szFullUrl, "?");
    } else {
        pUserData->m_szFullUrl = g_string_append(pUserData->m_szFullUrl, "&");
    }
    pUserData->m_szFullUrl = g_string_append(pUserData->m_szFullUrl, key);
    pUserData->m_szFullUrl = g_string_append(pUserData->m_szFullUrl, "=");
    pUserData->m_szFullUrl = g_string_append(pUserData->m_szFullUrl, value);

    DEBUG("%s\n", pUserData->m_szFullUrl->str);
}

/*
typedef struct {
    GString* m_szFullUrl;
    gint m_nTime;
}weibo_do_api_t, *pweibo_do_api_t;
 */
static char* weibo_do_api(char* szApi, GHashTable* pArgs, char* szToken, char* szTokenSecret, gboolean bPost)
{
    POS();
    g_return_val_if_fail(szToken != NULL, NULL);
    g_return_val_if_fail(szTokenSecret != NULL, NULL);

    pweibo_do_api_t pApiArgs = (pweibo_do_api_t)g_malloc(sizeof(weibo_do_api_t));
    pApiArgs->m_szFullUrl = g_string_new(WEIBO_API_BASEURL);
    pApiArgs->m_szFullUrl = g_string_append(pApiArgs->m_szFullUrl, szApi);
    pApiArgs->m_nTime = 0;

    if (pArgs) {
        g_hash_table_foreach(pArgs, (GHFunc)iterator, pApiArgs);
    }

    DEBUG("%s\n", pApiArgs->m_szFullUrl->str);

    return weibo_do_oauth_request(pApiArgs->m_szFullUrl->str, szToken, szTokenSecret, bPost);
}


pweibo_user weibo_get_my_profile()
{
    POS();
    pweibo_token pToken = weibo_token_load();
    if (NULL == pToken) {
        PERR("User not login\n");
        return NULL;
    }

#ifdef ENABLE_WEIBO_DO_API
    GHashTable* pArgsTable = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(pArgsTable, "user_id", pToken->m_szUserID);
    char* reply =  weibo_do_api("users/show.json", pArgsTable, pToken->m_szToken, pToken->m_szTokenSecret, TRUE);
#else
    char szUrl[512];
    sprintf(szUrl, "%susers/show.json?"
            "user_id=%s", WEIBO_API_BASEURL, pToken->m_szUserID);
    char *reply = weibo_do_oauth_post(szUrl , pToken->m_szToken, pToken->m_szTokenSecret);
#endif

    if(!reply){
        PERR("do oauth post failed!\n");
        return NULL;
    }
    DEBUG("%s\n", reply);

    return weibo_user_create(reply);
}

GList* weibo_get_friend_feeds(const char* szSinceID, const char* szMaxID, int nCount, int nPage, weibo_feed_type type)
{
    POS();
    pweibo_token pToken = weibo_token_load();
    if (NULL == pToken) {
        PERR("User not login\n");
        return NULL;
    }

#ifdef ENABLE_WEIBO_DO_API
    GHashTable* pArgsTable = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(pArgsTable, "since_id", szSinceID);
    g_hash_table_insert(pArgsTable, "max_id", szMaxID);

    char buf[32];
    sprintf(buf, "%d", nCount);
    g_hash_table_insert(pArgsTable, "count", buf);

    char buf1[32];
    sprintf(buf1, "%d", nPage);
    g_hash_table_insert(pArgsTable, "page", buf1);

    char buf2[32];
    sprintf(buf2, "%d", type);
    g_hash_table_insert(pArgsTable, "feature", buf2);
    char* reply =  weibo_do_api("statuses/friends_timeline.json", NULL, pToken->m_szToken, pToken->m_szTokenSecret, TRUE);
#else
    char szUrl[512];
    sprintf(szUrl, "%sstatuses/friends_timeline.json?"
            , WEIBO_API_BASEURL);

    char *reply = weibo_do_oauth_post(szUrl , pToken->m_szToken, pToken->m_szTokenSecret);
#endif

    if(!reply){
        PERR("do oauth post failed!\n");
        return NULL;
    }
    //DEBUG("%s\n", reply);

    GList* feedList = weibo_feed_friends_timeline_create(reply);
    PERR("%d\n", feedList);
    return feedList;
}

GList* weibo_get_at_me(const char* szSinceID, const char* szMaxID, int nCount, int nPage)
{
    POS();
    pweibo_token pToken = weibo_token_load();
    if (NULL == pToken) {
        PERR("User not login\n");
        return NULL;
    }

#ifdef ENABLE_WEIBO_DO_API
    GHashTable* pArgsTable = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(pArgsTable, "since_id", szSinceID);
    g_hash_table_insert(pArgsTable, "max_id", szMaxID);

    char buf[32];
    sprintf(buf, "%d", nCount);
    DEBUG("%s\n", buf);
    GString* szCount = g_string_new("");
    g_hash_table_insert(pArgsTable, "count", buf);

    char buf1[32];
    sprintf(buf1, "%d", nPage);
    DEBUG("%s\n", buf1);
    g_hash_table_insert(pArgsTable, "page", buf1);

    char* reply =  weibo_do_api("statuses/mentions.json", pArgsTable, pToken->m_szToken, pToken->m_szTokenSecret, TRUE);
#else
    char szUrl[512];
    sprintf(szUrl, "%sstatuses/mentions.json?"
            , WEIBO_API_BASEURL);
    char *reply = weibo_do_oauth_post(szUrl , pToken->m_szToken, pToken->m_szTokenSecret);
#endif

    if(!reply){
        PERR("do oauth post failed!\n");
        return NULL;
    }
    //DEBUG("%s\n", reply);

    GList* feedList = weibo_feed_friends_timeline_create(reply);
    PERR("%d\n", feedList);
    return feedList;
}

GList* weibo_get_user_weibo(const char* szUserId, const char* szSinceID, const char* szMaxID, int nCount, int nPage, int nBaseApp, weibo_feed_type type)
{
    POS();
    pweibo_token pToken = weibo_token_load();
    if (NULL == pToken) {
        PERR("User not login\n");
        return NULL;
    }

    GHashTable* pArgsTable = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(pArgsTable, "user_id", szUserId);
    g_hash_table_insert(pArgsTable, "since_id", szSinceID);
    g_hash_table_insert(pArgsTable, "max_id", szMaxID);

    char* buf = (char*)g_malloc(32);
    sprintf(buf, "%d", nCount);
    g_hash_table_insert(pArgsTable, "count", buf);

    buf = (char*)g_malloc(32);
    sprintf(buf, "%d", nPage);
    g_hash_table_insert(pArgsTable, "page", buf);

    buf = (char*)g_malloc(32);
    sprintf(buf, "%d", nBaseApp);
    g_hash_table_insert(pArgsTable, "base_app", buf);

    buf = (char*)g_malloc(32);
    sprintf(buf, "%d", type);
    g_hash_table_insert(pArgsTable, "feature", buf);

    char* reply =  weibo_do_api("statuses/user_timeline.json", pArgsTable, pToken->m_szToken, pToken->m_szTokenSecret, TRUE);

    if(!reply){
        PERR("do oauth post failed!\n");
        return NULL;
    }
    //DEBUG("%s\n", reply);

    GList* feedList = weibo_feed_friends_timeline_create(reply);
    PERR("%d\n", feedList);
    return feedList;

}

GString* submit_weibo_with_image(GString* szStatus, GString* szPic, GString* szLat, GString* szLong)
{
    pweibo_token pToken = weibo_token_load();
    if (!pToken) {
        PERR("Login failed!\n");
        return NULL;
    }

    GString* szJarfile = get_app_root();
    g_string_append(szJarfile, "/");
    g_string_append(szJarfile, "uploader/Uploader.jar");
    GString* szCmd = g_string_new("java -jar ");
    g_string_append(szCmd, szJarfile->str);
    g_string_append(szCmd, " ");
    g_string_append(szCmd, pToken->m_szToken);
    g_string_append(szCmd, " ");
    g_string_append(szCmd, pToken->m_szTokenSecret);
    g_string_append(szCmd, " \"");
    g_string_append(szCmd, szPic->str);
    g_string_append(szCmd, "\" \"");
    g_string_append(szCmd, szStatus->str);
    g_string_append(szCmd, "\" ");
    DEBUG("%s\n", szCmd->str);

    FILE* pFile = popen(szCmd->str,"r");
    if (NULL == pFile) {
        PERR("do popen failed\n");
        return NULL;
    }

    char* buffer[1024];
    while (fgets(buffer, 1024, pFile)) {
        DEBUG("%s\n", buffer);
    }

    weibo_token_clean(pToken);

    return g_string_new("[SUCCESS]");
}

pweibo_feed weibo_show(const GString* szID)
{
    pweibo_token pToken = weibo_token_load();
    if (!pToken) {
        PERR("Login failed!\n");
        return NULL;
    }

    GString* szApi = g_string_new("statuses/show/:");
    g_string_append(szApi, szID->str);
    g_string_append(szApi, ".json");
    char* reply =  weibo_do_api(szApi->str, NULL, pToken->m_szToken, pToken->m_szTokenSecret, FALSE);

    if(!reply){
        PERR("do oauth post failed!\n");
        return NULL;
    }

    DEBUG("%s\n", reply);

    return weibo_feed_create(reply);
}

pweibo_feed weibo_destroy(const GString* szID)
{
    CHECK_WEIBO_TOKEN();

    GString* szApi = g_string_new("statuses/destroy/:");
    g_string_append(szApi, szID->str);
    g_string_append(szApi, ".json");
    char* reply =  weibo_do_api(szApi->str, NULL, pToken->m_szToken, pToken->m_szTokenSecret, FALSE);

    if(!reply){
        PERR("do oauth post failed!\n");
        return NULL;
    }

    DEBUG("%s\n", reply);

    return weibo_feed_create(reply);
    return NULL;
}

pweibo_feed weibo_update(GString* szStatus, GString* in_reply_to_status_id, GString* szLat, GString* szLong, GString* annotations)
{
    g_return_val_if_fail(szStatus != NULL, NULL);

    CHECK_WEIBO_TOKEN();

    GHashTable* pArgsTable = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(pArgsTable, "status", szStatus->str);
    if (in_reply_to_status_id) {
        g_hash_table_insert(pArgsTable, "in_reply_to_status_id", in_reply_to_status_id->str);
    }
    if (szLat) {
        g_hash_table_insert(pArgsTable, "lat", szLat->str);
    }
    if (szLong) {
        g_hash_table_insert(pArgsTable, "long", szLong->str);
    }
    if (annotations) {
        g_hash_table_insert(pArgsTable, "annotations", annotations->str);
    }
    
    char* reply =  weibo_do_api("statuses/update.json", pArgsTable, pToken->m_szToken, pToken->m_szTokenSecret, TRUE);
    if(!reply){
        return NULL;
    }

    DEBUG("%s\n", reply);
    return weibo_feed_create(reply);
}
