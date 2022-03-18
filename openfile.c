/**
 * File              : openfile.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.09.2021
 * Last Modified Date: 18.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "openfile.h"
#include <strings.h>
#include <stdlib.h>
#if defined _WIN32 || defined _WIN64
#include <windows.h>
#endif

int k_lib_openfile(const char *filename){
	char open_file_command[BUFSIZ];
#ifdef __APPLE__
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR
	#elif TARGET_OS_MACCATALYST
	#elif TARGET_OS_IPHONE
	#elif TARGET_OS_MAC
		sprintf(open_file_command, "open %s", filename);	
		system(open_file_command);
	#else
	#endif
#elif defined _WIN32 || defined _WIN64
	ShellExecute(NULL, "open", filename, NULL, NULL, SW_SHOWDEFAULT);
#else
	sprintf(open_file_command, "xdg-open %s", filename);	
	system(open_file_command);
#endif	
	
	return 0;
}

