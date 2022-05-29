/**
 * File              : file_exists.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 29.05.2022
 * Last Modified Date: 29.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_file_exists_h__
#define k_file_exists_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdbool.h>
#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

//copy file FROM path to destination TO path
bool file_exists(const char *path){
	if (access(path, F_OK) == 0)
		return true;
	return false;
}

#ifdef __cplusplus
}
#endif

#endif //k_file_exists_h__

