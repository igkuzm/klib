/**
 * File              : openfile.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.09.2021
 * Last Modified Date: 22.02.2022
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
	sprintf(open_file_command, "open %s", filename);	
	system(open_file_command);
#elif defined _WIN32 || defined _WIN64
	ShellExecute(NULL, "open", filename, NULL, NULL, SW_SHOWDEFAULT);
#else
	sprintf(open_file_command, "xdg-open %s", filename);	
	system(open_file_command);
#endif	
	
	return 0;
}

