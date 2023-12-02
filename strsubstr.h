/**
 * File              : strsubstr.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 02.12.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/* 
 * Substring function
 */

#ifndef STRSUBSTR_H__
#define STRSUBSTR_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>
#include <string.h>

/* strsubstr
 * return allocated string with contents of str 
 * from position pos and lendth len 
 */
static char * strsubstr(
		const char *str, int pos, int len)
{
	char *substr = (char*)malloc(len+1);
	if (!substr)
		return NULL;
	
	memcpy(substr, &str[pos], len);
	substr[len] = '\0';

	return substr;
}
#ifdef __cplusplus
}
#endif

#endif //STRSUBSTR_H__
