
#ifndef _WEIBO_TOKEN_H_
#define _WEIBO_TOKEN_H_

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct {
    char m_szToken[64];
    char m_szTokenSecret[64];
    char m_szUserID[32];
} weibo_token, *pweibo_token;

pweibo_token weibo_token_create(char* szStr);
void weibo_token_clean(pweibo_token pToken);
pweibo_token weibo_token_load();

#ifdef __cplusplus
    }
#endif

#endif
