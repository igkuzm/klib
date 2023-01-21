/**
 * File              : each.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.03.2022
 * Last Modified Date: 21.01.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_each_h__
#define k_lib_each_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

// MACROS "for each" for NULL-terminated array
/*
        //use:
        for earch(type, item, array) {
                //do your magic
        }
*/
#define each(type, item, array)                                                \
  (type *p = (array), (item) = *p; *p; p++, (item) = *p)

#ifdef __cplusplus
}
#endif

#endif // k_lib_each_h__
