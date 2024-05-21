/**
 * File              : fstrfind.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.05.2024
 * Last Modified Date: 21.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * fstrfind.h
 * Copyright (c) 2024 Igor V. Sementsov <ig.kuzm@gmail.com>
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


/* fstrfind - find strings in file stream */

#ifndef FSTRFIND_H
#define FSTRFIND_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool _sarray_matches(int argc, char *argv[], char *buf);

/* copy %in stream to %out and, if found one of argv string, 
 * callback buffer with it before writing to %out */
static void fstrfind(
		FILE *in, FILE *out,
		int argc, char *argv[], 
		void *userdata,
    int (*callback)(void *userdata, char *buffer)) 
{
  int i, maxlen = 0;

  // count len
  for (i=0; i < argc; ++i) {
    int len = strlen(argv[i]);
    if (len > maxlen)
      maxlen = len;
  }

  char buf[maxlen + 1];
	memset(buf, 0, sizeof(buf));

	int l = 0; // length of buffer
  int ch = 1;
	while (ch != EOF) {
    ch = fgetc(in);
		if (ch == EOF)
			break;
		
		// put char to buffer
		buf[l++] = ch;
		buf[l] = 0;

		// check buffer matches search strings
		int match = false;
		while (_sarray_matches(argc, argv, buf) && l<maxlen){
			match = true;
			ch = fgetc(in);
			if (ch == EOF)
				break;
			buf[l++] = ch;
			buf[l] = 0; // null-terminate buffer
		} 

		if (match){
			if (l==maxlen){
				// callback founded string
				if (callback)
					if (callback(userdata, buf))
						break;
			}
			
			// put buf to out and free it's content
			char *p = (char *)buf;
			while (*p)
				fputc(*p++, out);
			memset(buf, 0, sizeof(buf));
			l=0;
			continue;
		}

		// put first char of buffer to out and move it to 1 left 
    fputc(buf[0], out);
		memmove(&buf[i], &buf[i+1], l--);
  }
}

static bool _string_matches(char *s1, char *s2)
{
	while (*s1 && *s2)
		if (*s1++ != *s2++)
			return false;
	return true;
}

bool _sarray_matches(int argc, char *argv[], char *buf)
{
	int i;
	for (i=0; i<argc; ++i) {
		char *arg = argv[i];
		if (_string_matches(arg, buf))
			return true;
  }
	return false;
}

#ifdef __cplusplus
}
#endif
#endif /* ifndef FSTRFIND_H */
