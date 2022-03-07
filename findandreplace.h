/**
 * File              : findandreplace.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 07.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_find_and_replace_h__
#define k_lib_find_and_replace_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

char *k_lib_find_and_replace(const char *haystack, const char *needle, const char *replace);

#ifdef __cplusplus
}
#endif

#endif //k_lib_find_and_replace_h__
