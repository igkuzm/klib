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
#include <stdint.h>
#include <sys/types.h>
#else
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
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
	char path[BUFSIZ];
	off_t size;
	struct timeval crtime; //creation time
	struct timeval atime; //access time
	struct timeval mtime; //modifier time	
} file_t;

void file_init(file_t * file){
	file->filetype = FILETYPE_OTHER;
	file->filename = NULL;
	file->path[0] = '\0';
	file->size = 0;
	file->crtime.tv_sec = 0; file->crtime.tv_usec = 0;
	file->atime.tv_sec = 0; file->atime.tv_usec = 0;
	file->mtime.tv_sec = 0; file->mtime.tv_usec = 0;
}

#if defined _WIN32 || defined _WIN64
void convert_filetime(struct timeval *out_tv, const FILETIME *filetime)
{
    // Microseconds between 1601-01-01 00:00:00 UTC and 1970-01-01 00:00:00 UTC
    static const uint64_t EPOCH_DIFFERENCE_MICROS = 11644473600000000ull;

    // First convert 100-ns intervals to microseconds, then adjust for the
    // epoch difference
    uint64_t total_us = (((uint64_t)filetime->dwHighDateTime << 32) | (uint64_t)filetime->dwLowDateTime) / 10;
    total_us -= EPOCH_DIFFERENCE_MICROS;

    // Convert to (seconds, microseconds)
    out_tv->tv_sec = (time_t)(total_us / 1000000);
    out_tv->tv_usec = (useconds_t)(total_us % 1000000);
}
#endif

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
		file_init(&file);
		file.filename = findData.cFileName; 
		sprintf(file.path, "%s\\%s", dir, file.filename);
		file.size = (findData.nFileSizeHigh * (MAXDWORD+1)) + findData.nFileSizeLow; 
		convert_filetime(&file.crtime, &findData.ftCreationTime);
		convert_filetime(&file.atime, &findData.ftLastAccessTime);
		convert_filetime(&file.mtime, &findData.ftLastWriteTime);
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
		file_init(&file);
		file.filename = entry->d_name;
		sprintf(file.path, "%s/%s", dir, file.filename);
		switch (entry->d_type) {
			case  DT_REG: file.filetype = FILETYPE_FILE; break;
			case  DT_DIR: file.filetype = FILETYPE_DIR; break;
			case  DT_LNK: file.filetype = FILETYPE_LINK; break;
			default: file.filetype = FILETYPE_OTHER; break;
		}
		struct stat st;
		if (stat(file.path, &st) == 0) {
			file.size = st.st_size;
			file.crtime.tv_sec = st.st_birthtimespec.tv_sec; file.crtime.tv_usec = st.st_birthtimespec.tv_nsec;
			file.atime.tv_sec = st.st_atimespec.tv_sec; file.atime.tv_usec = st.st_atimespec.tv_nsec;
			file.mtime.tv_sec = st.st_mtimespec.tv_sec; file.mtime.tv_usec = st.st_mtimespec.tv_nsec;
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
