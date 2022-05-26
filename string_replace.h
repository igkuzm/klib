/**
 * File              : string_replace.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 26.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

//Usage:  
//		  char * new_string;
//		  strrep(old_string, search_pattern, replace_string, &new_string);
//OR: to find number of cases:
//		  int count = strfind(string, search_pattern)

#ifndef k_lib_string_replace_h__
#define k_lib_string_replace_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int //return number of cases of search word in haystack
	k_lib_string_replace(
		const char * haystack,       //string to scan 
		const char * needle,		 //search pattern
		const char * replace,		 //replace string
			  char ** result		 //result string pointer
);
#define strrep(haystack, needle, replace, result)\
	(k_lib_string_replace(haystack, needle, replace, result))

#define strfind(haystack, needle)\
	(k_lib_string_replace(haystack, needle, NULL, NULL))

int k_lib_string_replace(const char *haystack, const char *needle, const char *replace, char ** _result) {
	int needlelen = strlen(needle);
	int replacelen; 
	if (replace)
		replacelen = strlen(replace);

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
	if (_result && replace){
		char *result = (char *)malloc(i + cnt * (replacelen - needlelen) + 1);
		if (result == NULL) {
			perror("result string malloc");
			exit(EXIT_FAILURE);
		}
		*_result = result;

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
	}
	return cnt;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_string_replace_h__
