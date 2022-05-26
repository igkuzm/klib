/**
 * File              : string_replace.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 26.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

//Usage:  char * new_string = string_replace(old_string, search_pattern, replace_string);

#ifndef k_lib_string_replace_h__
#define k_lib_string_replace_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char * //return result string
	k_lib_string_replace(
		const char *haystack,        //string to scan 
		const char *needle,			 //search pattern
		const char *replace			 //replace string
);
#define string_replace(haystack, needle, replace) (k_lib_string_replace(haystack, needle, replace))

char *k_lib_string_replace(const char *haystack, const char *needle, const char *replace) {
	int needlelen = strlen(needle);
	int replacelen = strlen(replace);

	//count cases of search word in haystack
	int i, cnt = 0;
	for (i = 0; haystack[i] != '\0'; i++) {
		if (strstr(&haystack[i], needle) == &haystack[i]) {
			cnt++;

			// Jumping to index after the needle word.
			i += needlelen - 1;
		}
	}

	//allocate result string with enough memory
	char *result = (char *)malloc(i + cnt * (replacelen - needlelen) + 1);
	if (result == NULL) {
		perror("result string malloc");
		exit(EXIT_FAILURE);
	}

	//replace search needle string with replace string
	i = 0;
	while (*haystack) {
		if (strstr(haystack, needle) == haystack) { //if found word
			strncpy(result, replace, replacelen); //copy new string to result
			i += replacelen; //move result counter
			haystack += needlelen; //move read string counter
		} else {
			result[i++] = *haystack++;
		}
	}
	result[i] = '\0';
	return result;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_string_replace_h__
