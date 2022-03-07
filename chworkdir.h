/**
 * File              : chworkdir.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 07.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_chworkdir_h__
#define k_lib_chworkdir_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

int k_lib_chWorkDir(char *argv[]);

#ifndef changeWorkDir 
#define changeWorkDir(argv)	\
({	\
	int ___c = k_lib_chWorkDir(argv);\
	___c;	\
})
#endif

#ifdef __cplusplus
}
#endif

#endif //k_lib_chworkdir_h__
