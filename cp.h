/**
 * File              : cp.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.09.2021
 * Last Modified Date: 21.01.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * copy file from path to destination (overwrite destination)
 */

#ifndef k_lib_cp_h__
#define k_lib_cp_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

int cp(const char *from, const char *to) {
  FILE *fp_from = fopen(from, "r");
  if (!fp_from)
    return -1;

  FILE *fp_to = fopen(to, "w");
  if (!fp_to) {
    fclose(fp_from);
    return -1;
  }

  int ch;
  while ((ch = fgetc(fp_from)) != EOF)
    fputc(ch, fp_to);

  fclose(fp_from);
  fclose(fp_to);

  return 0;
}

#ifdef __cplusplus
}
#endif

#endif // k_lib_cp_h__
