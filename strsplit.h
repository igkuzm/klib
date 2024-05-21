/**
 * File              : strsplit.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 21.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * strsplit.h
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
 * Get array from string separated by delims
 */

#ifndef STRSPLIT_H
#define STRSPLIT_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>
#include <string.h>
/* strsplit
 * split string with delims, fill allocated NULL-terminated 
 * array of allocated tokens and return number of tokens
 * %str    - string to split
 * %delim  - string with delim chars
 * %tokens - pointer to tokens array
 */
static int strsplit(
		const char *str, const char *delim, char ***tokens)
{
	int i;
	char *s, *t, **arr = (char **)malloc(sizeof(char*));
	if (!arr)
		return 0;
	
	// do safe strtok
	s = strdup(str);
	if (!s)
		return 0;

	// loop through the string to extract 
	// all tokens
	for(t=strtok(s, delim), i=0; 
			t; 
			t=strtok(NULL, delim), ++i) 
	{
		arr = 
			(char **)realloc(arr, (i+2)*sizeof(char*));
		arr[i] = strdup(t);
	}

	arr[i] = NULL;
	if (tokens)
		*tokens = arr;
	else
		free(arr);

	free(s);
	
	return i;
}

static void strsplit_free(char **tokens)
{
	char **ptr = tokens;
	while (*ptr)
		free(*ptr++);
	free(tokens);
}

#ifdef __cplusplus
}
#endif

#endif // STRSPLIT_H
