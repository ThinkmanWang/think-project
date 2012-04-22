
#include "downloader.h"

#include <unistd.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <pthread.h>

#include "log.h"

__thread FILE *fp;
__thread GString* g_szPath;
pthread_mutex_t g_mutex=PTHREAD_MUTEX_INITIALIZER;

DEFINE_EA_MOD(MOD_NETWORK);

size_t write_data (void *ptr, size_t size, size_t nmemb, void *stream)
{
    int written = fwrite (ptr, size, nmemb, (FILE *) fp);
    return written;
}

GString* download(GString* szUrl, GString* szPath)
{
    pthread_mutex_lock(&g_mutex);
    CURL *curl;

    curl_global_init (CURL_GLOBAL_ALL);
    curl = curl_easy_init ();
    curl_easy_setopt (curl, CURLOPT_URL, szUrl->str);

    if ((fp = fopen(szPath->str, "w")) == NULL)
    {
        curl_easy_cleanup (curl);
        PERR("Open %s failed\n", szPath->str);
        pthread_mutex_unlock(&g_mutex);
        return NULL;
    }


    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_perform (curl);
    curl_easy_cleanup (curl);
    pthread_mutex_unlock(&g_mutex);
    return szPath;
}
