/**
 * File              : cp_recursive.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 21.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_cp_recursive_h__
#define k_lib_cp_recursive_h__
#endif

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

//copy file FROM path to destination TO path
int k_lib_cp_recursive(const char *from, const char *to);

#ifdef __cplusplus
}
#endif

