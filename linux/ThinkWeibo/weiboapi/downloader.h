
#ifndef _WEIBO_DOWNLOADER_H_
#define _WEIBO_DOWNLOADER_H_

#ifdef __cplusplus
    extern "C" {
#endif
#include <glib.h>
#include <stdio.h>
#include <curl/curl.h> 

GString* download(GString* szUrl, GString* szPath);

#ifdef __cplusplus
    }
#endif

#endif
