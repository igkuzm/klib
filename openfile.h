/**
 * File              : openfile.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.09.2021
 * Last Modified Date: 16.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_openfile_h__
#define k_lib_openfile_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

int k_lib_openfile(const char *filename);

#ifndef openfile
#define openfile(filename)	\
({	\
	int ___c = k_lib_openfile(filename);\
	___c;	\
})
#endif

#ifdef __cplusplus
}
#endif

#endif //k_lib_openfile_h__

