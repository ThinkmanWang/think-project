
#ifndef _WEIBO_CONFIG_H_
#define _WEIBO_CONFIG_H_

#ifdef __cplusplus
    extern "C" {
#endif

#define WEIBO_HOME ".ThinkWeibo"
#define WEIBO_TOKEN_FILE "token"

#define WEIBO_API_BASEURL "http://api.t.sina.com.cn/"
#define TREND_URL WEIBO_API_BASEURL"trends/hourly.json?source=3551174478&base_app=0"
#define MAX_BUF 	 65536 
#define MAX_FILE_LENGTH 1024 

#define APP_KEY             "3551174478"
#define APP_SECRET          "c53f4123bdb7b39a0ae03e400b839934"
#define WEIBO_XTOKEN        "18d2d221a6bc0aa8743d3f30d6745b7e"
#define WEIBO_XTOKEN_SECRET "2de591d207b213e4f46a1d1170413367"

#ifdef __cplusplus
    }
#endif

#endif
