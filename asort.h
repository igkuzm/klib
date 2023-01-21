/**
 * File              : asort.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.09.2021
 * Last Modified Date: 21.01.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * asort - sort array
 * %cmp - compare function `int cmp(a, b)`;
 * return > 0 - sort array by descending mode
 * return < 0 - sort array by ascending mode
 *
 * example:
 *	int cmp(int a, int b) {
 *		return b - a;
 *	}
 *
 *  ***
 *	int ar[] = {12, 11, 3, 34, 26, 2, 0};
 *	asort(ar, int, 7, cmp);
 *	***
 */

#ifndef k_lib_cp_h__
#define k_lib_cp_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/*
 * void asort(void *array, T, int size, int (*cmp)(T a, T b))
 * % array
 * % T - array type 
 * % size - sizeof array
 * % cmp - compare function
 */
#define asort(array, T, size, cmp) \
({\
  T p;\
  int i, j;\
  for (i = 0; i < size; ++i) {\
    for (j = 0; j < size; ++j) {\
      if (cmp(array[i], array[j]) > 0) {\
        p = array[i];\
        array[i] = array[j];\
        array[j] = p;\
      }\
    }\
  }\
})
#ifdef __cplusplus
}
#endif

#endif // k_lib_cp_h__
