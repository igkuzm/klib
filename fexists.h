/**
 * File              : fexists.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 29.05.2022
 * Last Modified Date: 14.08.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_fexists_h__
#define k_fexists_h__

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

bool fexists(const char *path){
	if (access(path, F_OK) == 0)
		return true;
	return false;
}

#ifdef __cplusplus
}
#endif

#endif //k_fexists_h__

