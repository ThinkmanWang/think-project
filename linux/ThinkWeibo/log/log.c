#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <glib.h>
#include <pthread.h>
#include "log.h"

static GHashTable *log_table = NULL;
static gboolean elinit = FALSE;
static FILE *fout = NULL;
DEFINE_EA_MOD(MOD_EALOG);

//define the MOD string
DEFINE_MOD_LIST(eaModules, MOD_LIST)

//define the string function, get string
DEC_TO_STRING_NAME_FUNC(eaLogLevel, EA_LOG_LEVEL_LIST)

//define the strings function, get level number
DEC_TO_STRING_INT_FUNC(eaLogLevel, EA_LOG_LEVEL_LIST)


static void log_handler(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data)
{
	FILE *fp = user_data;
	//fprintf (fp, "[%s:%lu] %s", log_domain, (unsigned long) pthread_self(), message);
	fprintf (fp, "[%s] %s", log_domain, message);
	fflush(fp);
}

//for single module init
void ELSetModLog(eaModules modid)
{
	if (elinit) {
		//printf("set log handler %s\n", modname);
		g_log_set_handler (eaModules_str[modid], G_LOG_LEVEL_MASK, log_handler, fout);
	}
	else {
		ELInit(NULL, modid);
		ELSetDefaultLogTable(EA_LOG_DEBUG);
	}
}

//main function use
void ELInit(const gchar* logfilename, eaModules modid)
{
	if (!elinit) {
		if (!logfilename) {
			fout = stderr;
		}
		else {
			fout = fopen(logfilename, "w");
			if (fout == NULL) {
				g_print("Can not open the log file [%s]\n", logfilename);
			}
		}
		elinit = TRUE;
		ELSetModLog(modid);
	}
}

void ELUninit(void)
{
	if (elinit) {
		if (fout && fout != stderr) {
			fclose(fout);
		}
		if (log_table) {
			g_hash_table_destroy(log_table);
			log_table = NULL;
		}
		elinit = FALSE;
	}
}

static void gen_free(gpointer data)
{
	g_free(data);
}

//Set the logging level of single module
void ELSetModuleLogLevel(eaModules modid, eaLogLevel level)
{
	//gchar *level_string = g_strdup(eaLogLevelToStringName(level));
	gchar *ea_module = g_strdup(eaModules_str[modid]);
	gint * plevel = g_new0(gint, 1);
 	*plevel = level;

	if(!log_table)
	{
		log_table = g_hash_table_new_full(g_str_hash, g_str_equal, gen_free, gen_free);
	}
	g_hash_table_insert(log_table, (gpointer)ea_module, plevel);
}

void ELSetDefaultLogTable(eaLogLevel level)
{
	static int init = 0;
	if (!init) {
		int i;
		init = 1;

		for (i=MOD_MAIN; i<MOD_LAST; i++) {
			ELSetModuleLogLevel(i, level);
			g_log_set_handler (eaModules_str[i], G_LOG_LEVEL_MASK, log_handler, fout);
		}
	}
	else {
		PWARN("Reinit the %s, should not do this, please check\n", __FUNCTION__);
	}
}

static void update_log_table_foreach(gpointer key, gpointer value, gpointer data)
{
	g_hash_table_insert(log_table, key, data);
}

void ELSetAllModLogLevel(eaLogLevel level)
{
	//gchar *levelstr = g_strdup(eaLogLevelToStringName(level));
	gint * plevel = g_new0(gint, 1);

	*plevel = level;
	
	g_hash_table_foreach(log_table, update_log_table_foreach, plevel);
}

static void print_log_table_foreach(gpointer key, gpointer value, gpointer data)
{
	g_print("Dbg %s %s\n", (gchar *)key, eaLogLevelToStringName(*((gint *)value)));
}

void ELPrintLogTable(void)
{
	g_hash_table_foreach(log_table, print_log_table_foreach, NULL);
}

gboolean ELCheckModLogLevel(eaModules modid, eaLogLevel level)
{
	gint maxlvl; //smaller than this will be logged
	gint *log_level = NULL;

	if(!log_table) {
		return FALSE;
	}
	
	log_level = (gint *) g_hash_table_lookup(log_table, eaModules_str[modid]);

	if(!log_level) {
		//if log_module not found, do not log
		return FALSE;
	}

	//maxlvl = eaLogLevelToStringInt(log_string);
	maxlvl = *log_level;

	if (level <= maxlvl) {
		//g_print("Dennis2 %s %d %d\n", log_string, level, maxlvl);
		return TRUE;
	}
	//g_print("Dennis3 %s %d %d\n", log_string, level, maxlvl);
	return FALSE;
}
