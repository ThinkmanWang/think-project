#ifndef EA_LOG_H
#define EA_LOG_H

#include <glib.h>

#define EALOG_WITH_COLOR

#define CNONE         "\033[m"
#define CRED          "\033[0;32;31m"
#define CLIGHT_RED    "\033[1;31m"
#define CGREEN        "\033[0;32;32m"
#define CLIGHT_GREEN  "\033[1;32m"
#define CBLUE         "\033[0;32;34m"
#define CLIGHT_BLUE   "\033[1;34m"
#define CDARY_GRAY    "\033[1;30m"
#define CCYAN         "\033[0;36m"
#define CLIGHT_CYAN   "\033[1;36m"
#define CPURPLE       "\033[0;35m"
#define CLIGHT_PURPLE "\033[1;35m"
#define CBROWN        "\033[0;33m"
#define CYELLOW       "\033[1;33m"
#define CLIGHT_GRAY   "\033[0;37m"
#define CWHITE        "\033[1;37m"

#ifdef EALOG_WITH_COLOR
#define CERR          CRED
#define CWARN         CLIGHT_RED
#define CNOTICE       CYELLOW
#define CINFO         CGREEN
#define CDEBUG        CBLUE
#define CPOS          CBROWN
#else
#define CERR          ""
#define CWARN         ""
#define CNOTICE       ""
#define CINFO         ""
#define CDEBUG        ""
#define CPOS          ""
#undef  CNONE         
#define CNONE         "" 
#endif



#define ENUM_BODY(name, value)  name value,
#define CREATE_ENUM(name, list)  typedef enum { list(ENUM_BODY) } name; 

#define TO_STRING_NAME(name, value)	case name: { return #name; }
#define DEC_TO_STRING_NAME(name, list)  const gchar* name##ToStringName(name n);


#define DEC_TO_STRING_NAME_FUNC(name, list)   \
	const gchar* name##ToStringName(name n) { \
	switch (n) {                              \
		list(TO_STRING_NAME)                  \
		default: return "";  }                \
}

#define TO_STRING_INT(name, value) if (strcmp(str, #name) == 0) { return name;  }
#define DEC_TO_STRING_INT(name, list) name name##ToStringInt(const gchar* str);
#define DEC_TO_STRING_INT_FUNC(name, list)      \
	name name##ToStringInt(const gchar* str) {   \
		if(str == NULL) { return 0; }           \
		list(TO_STRING_INT)                     \
		return 0; \
 }

//Log level define
#define EA_LOG_LEVEL_LIST(_)  \
	_(EA_LOG_FATAL, = 0)   \
	_(EA_LOG_ERROR, = 1)   \
	_(EA_LOG_WARNING, = 2) \
	_(EA_LOG_INFO, = 3)    \
	_(EA_LOG_POS, = 4)    \
	_(EA_LOG_DEBUG, = 5)   

CREATE_ENUM(eaLogLevel, EA_LOG_LEVEL_LIST)
DEC_TO_STRING_NAME(eaLogLevel, EA_LOG_LEVEL_LIST)
DEC_TO_STRING_INT(eaLogLevel, EA_LOG_LEVEL_LIST)

#define TO_MOD_STR(name, value)	 #name,
#define DEFINE_MOD_LIST(name, list) \
	static gchar* name##_str[] = { \
		list(TO_MOD_STR)            \
};

//Module define
#define MOD_LIST(_) \
	_(MOD_MAIN, = 0) \
	_(MOD_SMBCTRL, = 1) \
	_(MOD_CNTPTC, = 2) \
	_(MOD_CNTMGR, = 3) \
	_(MOD_EVTMGR, = 4) \
	_(MOD_ACONF, = 5) \
	_(MOD_PVIEW, = 6) \
	_(MOD_HTTP, = 7) \
	_(MOD_TOOLS, = 8) \
	_(MOD_SEARCH , = 9) \
	_(MOD_EDBUS , = 10) \
	_(MOD_EALOG , = 11) \
	_(MOD_DATA , = 12) \
	_(MOD_UI , = 13) \
	_(MOD_NETWORK , = 14) \
	_(MOD_LAST , = 15) 

CREATE_ENUM(eaModules, MOD_LIST)

#define DEFINE_EA_MOD(module) \
static gchar log_domain[24] = #module ; \
static eaModules log_module = module


#define FUNC  __FUNCTION__

#define PFATAL(format, args...) {                    \
	g_log (log_domain, G_LOG_LEVEL_ERROR,          \
	"Fatal Error: %s(): line %d: " format, FUNC , __LINE__, ## args);   \
}

#define PERR(format, args...) {                      \
	if (ELCheckModLogLevel(log_module, EA_LOG_ERROR)) {  \
		g_log (log_domain, G_LOG_LEVEL_CRITICAL,   \
		CERR "Error: %s(): line %d: " format CNONE, FUNC , __LINE__, ## args);     \
	}                                                \
}

#define PWARN(format, args...) {                       \
	if (ELCheckModLogLevel(log_module, EA_LOG_WARNING)) {  \
		g_log (log_domain, G_LOG_LEVEL_WARNING,      \
		CWARN "Warning: %s(): line %d: " format CNONE, FUNC , __LINE__, ## args);     \
	}                                                  \
}

#define PINFO(format, args...) {                       \
	if (ELCheckModLogLevel(log_module, EA_LOG_INFO)) { \
		g_log (log_domain, G_LOG_LEVEL_INFO,         \
		CINFO "Info: %s(): line %d: " format CNONE, FUNC , __LINE__, ## args);        \
	}                                                  \
}

#define DEBUG(format, args...) {                        \
	if (ELCheckModLogLevel(log_module, EA_LOG_DEBUG)) { \
		g_log (log_domain, G_LOG_LEVEL_DEBUG,         \
		CDEBUG "Debug: %s() line %d: " format CNONE, FUNC , __LINE__, ## args);        \
	}                                                   \
} 

#define POS()  {                          \
	if (ELCheckModLogLevel (log_module, EA_LOG_POS)) { \
		g_log (log_domain, G_LOG_LEVEL_DEBUG,          \
		CPOS "At %s %s() line %d: \n" CNONE, __FILE__, FUNC , __LINE__);  \
	}                                                    \
}

//Function defines
#ifdef __cplusplus
    extern "C" {
#endif
void ELSetModLog(eaModules modid);  //for module use
void ELInit(const gchar* logfilename, eaModules modid);
void ELUninit(void);
void ELSetModuleLogLevel(eaModules modid, eaLogLevel level); //for module use
void ELSetDefaultLogTable(eaLogLevel level);
void ELSetAllModLogLevel(eaLogLevel level);
void ELPrintLogTable(void); //for debug
gboolean ELCheckModLogLevel(eaModules modid, eaLogLevel level);
#ifdef __cplusplus
    }
#endif

#endif

