
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "log.h"
#include "weiboapi.h"
#include "utils.h"

DEFINE_EA_MOD(MOD_MAIN);

int main(int argc,char *argv[])
{
    ELInit(NULL, log_module);
	ELSetDefaultLogTable(EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_MAIN, EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_UI, EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_DATA, EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_NETWORK, EA_LOG_DEBUG);
	ELPrintLogTable();

    time_t t;
    time (&t);
    GString* szStatus = g_string_new("test ");
    g_string_append(szStatus, ctime(&t));

    GString* szPic = g_string_new("/root/htc.jpg");

    DEBUG("%s -- %s", szStatus->str, szPic->str);
    GString* szRet = submit_weibo_with_image(szStatus, szPic, NULL, NULL);
    DEBUG("%s\n", szRet->str);

    GString* szTime = get_timestamp(g_string_new(ctime(&t)));
    DEBUG("%s\n", szTime->str);

    szTime = get_timestamp(g_string_new("Sun Apr 08 04:26:11 CST 2012"));
    DEBUG("%s\n", szTime->str);

    weibo_show(g_string_new("3433362725292601"));

    return 0;
}
