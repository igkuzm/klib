/**
 * File              : macros.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.03.2022
 * Last Modified Date: 21.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_macros_h__
#define k_lib_macros_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
	

//MACROS FOR EACH
// "length" is the length of the array.   
// can be used with for (as in for each (...))
#define each(type, item, array, length) \
(type *p = (array), (item) = *p; p < &((array)[length]); p++, (item) = *p)

#ifdef __cplusplus
}
#endif

#endif //k_lib_macros_h__


