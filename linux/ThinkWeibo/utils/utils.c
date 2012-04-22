
#include "utils.h"

GString* get_timestamp(GString* szTime)
{
    char szCmd[MAX_FILE_LENGTH];
    sprintf(szCmd, "date -d '%s' +%cs", szTime->str, 37);
    //printf("%s\n", szCmd);
    FILE* pFile = popen(szCmd,"r");
    if (NULL == pFile) {
        return -1;
    }

    char* buffer[1024];
    fgets(buffer, 1024, pFile);

    return g_string_new(buffer);
}
