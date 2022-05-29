/**
 * File              : ls.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 29.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_ls_h__
#define k_lib_ls_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <string.h>
#if defined _WIN32 || defined _WIN64
#include <Windows.h>
#else
#include <errno.h>
#include <dirent.h>
#endif	

typedef enum filetype_t {
	FILETYPE_FILE,
	FILETYPE_LINK,
	FILETYPE_DIR,
	FILETYPE_OTHER
} FILETYPE;

typedef struct k_lib_file_t {
	FILETYPE filetype;
	const char * filename;
	const char * dirpath;
} file_t;

//run callback for every file in dir. to stop execution - return non zero from callback
int ls(const char *dir, void *user_data, int (*callback)(file_t *file, void *user_data)){
	char error[BUFSIZ];
#if defined _WIN32 || defined _WIN64
	WIN32_FIND_DATAA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	char fullpath[BUFSIZ];
	sprintf(fullpath, "%s\\*", dir);

	hFind = FindFirstFileA(fullpath, &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		errno = ENOENT;
		sprintf(error, "Can't open directory: %s", dir);
		perror(error);
		return ENOENT;
	}
	while(FindNextFileA(hFind, &findData) != 0) {
		file_t file;
		file.filename = findData.cFileName; 
		file.dirpath = dir;
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			file.filetype = FILETYPE_DIR;
		else
			file.filetype = FILETYPE_FILE;
		
		if (callback(&file, user_data) != 0)
			return 1;
	}
	dwError = GetLastError();
	FindClose(hFind);
	if (dwError != ERROR_NO_MORE_FILES) {
		return dwError;
	}
#else	
	struct dirent *entry;
	DIR *dp;	

	dp = opendir(dir);
	if (dp == NULL){
		errno = ENOENT;
		sprintf(error, "Can't open directory: %s", dir);
		perror(error);
		return ENOENT;
	}	
	while((entry = readdir(dp))){ //reading files
		file_t file;
		file.filename = entry->d_name;
		file.dirpath = dir;
		switch (entry->d_type) {
			case  DT_REG: file.filetype = FILETYPE_FILE; break;
			case  DT_DIR: file.filetype = FILETYPE_DIR; break;
			case  DT_LNK: file.filetype = FILETYPE_LINK; break;
			default: file.filetype = FILETYPE_OTHER; break;
		}
		if (callback(&file, user_data) != 0)
			return 1;
	}
	closedir(dp);
#endif
	printf("%s\n", error);
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_ls_h__
