/**
 * File              : buf.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 16.11.2024
 * Last Modified Date: 16.11.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * buf.h
 * Copyright (c) 2024 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * Simple dynamic buffer
 */

#ifndef STR_H_
#define STR_H_
#include <stdio.h>

/* dynamic string structure */
struct buf {
	unsigned char *data; // allocated data
	int   len;   //length of data
	int   size;  //allocated size
};

/* init buffer - return non-null on error */
static int buf_init(struct str *s);

/* append buffer */
static void buf_append(
		struct buf *b, const unsigned char *data, int len);

/* IMPLIMATION */
#include <string.h>
#include <stdlib.h>

int str_init(struct str *s)
{
	if (!s)
		return -1;

	// allocate data
	s->str = (char*)malloc(BUFSIZ);
	if (!s->str)
		return -1;

	// set dafaults
	s->str[0]  = 0;
	s->len     = 0;
	s->size    = BUFSIZ;

	return 0;
}

static int _str_realloc(
		struct str *s, int new_size)
{
	while (s->size < new_size){
		// do realloc
		void *p = realloc(s->str, s->size + BUFSIZ);
		if (!p)
			return -1;
		s->str = (char*)p;
		s->size += BUFSIZ;
	}
	return 0;
}

void str_append(
		struct str *s, const char *str, int len)
{
	if (!str || len < 1)
		return;

	int new_size, i;
	
	new_size = s->len + len + 1;
	// realloc if not enough size
	if (_str_realloc(s, new_size))
		return;

	// append string
	for (i = 0; i < len; ++i)
		s->str[s->len++] = str[i];
  
	s->str[s->len] = 0;
}

#undef  str_appendf
#define str_appendf(pStruct_str, ...)\
	({\
	 char __str[BUFSIZ];\
	 snprintf(__str, BUFSIZ-1, __VA_ARGS__);\
	 __str[BUFSIZ-1] = 0;\
	 str_append(pStruct_str, __str, strlen(__str));\
	 })

#endif /* ifndef STR_H_ */
