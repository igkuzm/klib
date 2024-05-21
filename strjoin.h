/**
 * File              : strjoin.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 12.12.2023
 * Last Modified Date: 21.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * strjoin.h
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

/* 
 * Get string from array separated by delim
 */

#ifndef STRJOIN_H__
#define STRJOIN_H__
#include <stdlib.h>
#include <string.h>
/* strjoin
 * return allocated string with array members, 
 * separeted by delim
 * %array  - array with members
 * %len    - array length
 * %delim  - delim string
 */
static char * 
strjoin(const char **array, int len, const char *delim);

/* IMPLIMATION */
/* dynamic string structure */
struct _strjoin {
	char *str;   //null-terminated c string
	int   len;   //length of string (without last null char)
	int   size;  //allocated size
};

static int _strjoin_init(struct _strjoin *s)
{
	// allocate data
	s->str = (char*)malloc(1);
	if (!s->str)
		return -1;

	// set dafaults
	s->str[0] = 0;
	s->len     = 0;
	s->size    = 1;

	return 0;
}

static int _strjoin_realloc(
		struct _strjoin *s, int new_size)
{
	if (s->size < new_size){
		// do realloc
		void *p = realloc(s->str, new_size);
		if (!p)
			return -1;
		s->str = (char*)p;
		s->size = new_size;
	}
	return 0;
}

static void _strjoin_append(
		struct _strjoin *s, const char *str, const char *delim)
{
	if (!str)
		return;

	int len, dlen, new_size, i;
	
	len  = strlen(str);
	dlen = strlen(delim);
	new_size = s->len + len + dlen + 1;
	// realloc if not enough size
	if (_strjoin_realloc(s, new_size))
		return;

	// add delim
	if (s->str[0] != 0)
		for (i = 0; i < dlen; ++i)
			s->str[s->len++] = delim[i];

	// append string
	for (i = 0; i < len; ++i)
		s->str[s->len++] = str[i];
  
	s->str[s->len] = 0;
}

char * 
strjoin(const char **array, int len, const char *delim)
{
	struct _strjoin s;
	if (_strjoin_init(&s))
		return NULL;
	
	int i;
	for (i = 0; i < len; ++i)
		_strjoin_append(&s, array[i], delim);

	return s.str;
}
#endif /* ifndef STRJOIN_H__ */
