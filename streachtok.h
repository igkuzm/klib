/**
 * File              : streachtok.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 05.12.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/* 
 *  run callback for each token in strtok
 */

#ifndef STRSPLIT_H__
#define STRSPLIT_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>
#include <string.h>
/* streachtok
 * split string with delims, 
 * do callback for each token
 * and return number of tokens
 * %str - string to split
 * %delim - string with delim chars
 * %ptr - pointer to transfer trough callback
 * %callback - return non-null to stop function
 */
static int streachtok(
		const char *str, const char *delim,
		void *ptr,
		int(*callback)(const char *token, void *ptr))
{
	int i;
	char *s, *t;
	// do safe strtok
	s = strdup(str);
	if (!s)
		return 0;

	// loop through the string to extract 
	// all other tokens
	for(t=strtok(s, delim), i=0; 
			t; 
			t=strtok(NULL, delim), ++i) 
		if (callback)
			if (callback(t, ptr))
				goto strsplit_free;

strsplit_free:
	free(s);
	return i;
}

#ifdef __cplusplus
}
#endif

#endif //STRSPLIT_H__
