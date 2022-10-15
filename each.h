/**
 * File              : each.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.03.2022
 * Last Modified Date: 15.10.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_each_h__
#define k_lib_each_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

//MACROS FOR EACH for NULL-terminated array
#define each(type, item, array) \
(type *p = (array), (item) = *p; *p; p++, (item) = *p)

#ifdef __cplusplus
}
#endif

#endif //k_lib_each_h__


