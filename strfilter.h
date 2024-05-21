/**
 * File              : strfilter.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 21.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * strfilter.h
 * Copyright (c) 2022 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */  

#ifndef STRFILTER_H
#define STRFILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// allocate new string and fill with str filtred by 
// allowed chars
static char *strfilter(const char *str, const char *chars){

	int len = strlen(str);

  // allocate return buffer
  char *buf = (char *)malloc(len + 1);
  if (!buf)
    return NULL;

  // iterate string
  int i, l = 0;
  for (i=0; i < len; ++i) {
    // pointer to array
    char *ch = (char *)chars;
    while (*ch) {
			if (*ch++ == str[i]){
				buf[l++] = str[i];
				break;
			}
    }
  }

  // null-terminate string
  buf[l] = 0;

  return buf;
}

#ifdef __cplusplus
}
#endif

#endif // STRFILTER_H
