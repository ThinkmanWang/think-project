
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

#include "file_utils.h"

DEFINE_EA_MOD(MOD_MAIN);

int main ( int argc, char * argv[] ) 
{ 
    ELInit(NULL, log_module);
	ELSetDefaultLogTable(EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_MAIN, EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_UI, EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_DATA, EA_LOG_DEBUG);
	ELSetModuleLogLevel(MOD_NETWORK, EA_LOG_DEBUG);
	ELPrintLogTable();

    GString* szRoot = get_app_root();
    DEBUG("%s\n", szRoot->str);
    return 0;
} /* end of main */ 
