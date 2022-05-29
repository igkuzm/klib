/**
 * File              : is_directory.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 29.05.2022
 * Last Modified Date: 29.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_is_directory_h_
#define k_lib_is_directory_h_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#if defined _WIN32 || defined _WIN64
#include <Windows.h>
#else
#include <errno.h>
#include <dirent.h>
#endif	

//run callback for every file in dir. to stop execution - return non zero from callback
bool is_directory(const char *dir){
#if defined _WIN32 || defined _WIN64
	WIN32_FIND_DATAA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char fullpath[BUFSIZ];
	sprintf(fullpath, "%s\\*", dir);

	hFind = FindFirstFileA(fullpath, &findData);
	if (hFind != INVALID_HANDLE_VALUE) {
		return true;
	}
#else	
	struct dirent *entry;
	DIR *dp;	

	dp = opendir(dir);
	if (dp != NULL){
		closedir(dp);
		return true;
	}	
#endif
	return false;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_is_directory_h_
