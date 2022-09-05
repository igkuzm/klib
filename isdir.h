/**
 * File              : isdir.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 29.05.2022
 * Last Modified Date: 06.09.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * checks if is directory at path
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
#include <dirent.h>
#endif	

bool isdir(const char *path){
#if defined _WIN32 || defined _WIN64
	WIN32_FIND_DATAA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char fullpath[BUFSIZ];
	sprintf(fullpath, "%s\\*", path);

	hFind = FindFirstFileA(fullpath, &findData);
	if (hFind != INVALID_HANDLE_VALUE) {
		return true;
	}
#else	
	struct dirent *entry;
	DIR *dp;	

	dp = opendir(path);
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
