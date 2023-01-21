/**
 * File              : fsize.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 29.05.2022
 * Last Modified Date: 21.01.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * Get file size
 */

#ifndef k_lib_fsize_h_
#define k_lib_fsize_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#if defined _WIN32
#include <Windows.h>
#else
#include <sys/stat.h>
#endif

off_t fsize(const char *path) {
#if defined _WIN32
  WIN32_FIND_DATAA findData;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  hFind = FindFirstFileA(path, &findData);
  if (hFind != INVALID_HANDLE_VALUE) {
    return (findData.nFileSizeHigh * (MAXDWORD + 1)) + findData.nFileSizeLow;
  }
#else
  struct stat st;
  if (stat(path, &st) == 0) {
    return st.st_size;
  }
#endif
  return 0;
}

#ifdef __cplusplus
}
#endif

#endif // k_lib_fsize_h_
