/**
 * File              : streql.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 07.11.2023
 * Last Modified Date: 07.11.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_streql_h__
#define k_lib_streql_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/*
 * streql - true if strings are equal
 * %s1 - c null-terminated string
 * %s2 - c uill-terminated string
 */
static int streql(const char *s1, const char *s2) {
  // for each char in s1 check if matches s2
  while (*s1 == *s2++)
    if (*s1++ == 0)
      return 1; // return true

  return 0; // return false
}


#ifdef __cplusplus
}
#endif

#endif // k_lib_streql_h__
