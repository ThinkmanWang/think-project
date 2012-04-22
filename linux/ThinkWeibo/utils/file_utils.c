
#include "file_utils.h"
#include "weibo_config.h"

DEFINE_EA_MOD(MOD_DATA);

GString* get_app_root()
{
    char buf[MAX_FILE_LENGTH]; 
    int count; 

    count = readlink("/proc/self/exe", buf, MAX_FILE_LENGTH); 
    if (count < 0 || count >= MAX_FILE_LENGTH) { 
        printf("Failed\n"); 
        return(EXIT_FAILURE); 
    } 

    buf[count] = '\0'; 

    return g_string_new(dirname(buf));
}

char *dirname(const char *path)
{
	const char *slash = strrchr(path, '/');

	if (slash) {
		int len = slash - path;
		char *dir = (char*)malloc(len + 1);

		memcpy(dir, path, len);
		dir[len] = '\0';
		return dir;
	}
	return NULL;
}
