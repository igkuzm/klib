/**
 * File              : str_find_replace.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 26.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**Usage:  
 *to replace all matches:
 *		  char * new_string = strarep(old_string, search_pattern, replace_string);
 *to replace from string position:
 *		  char * new_string = strrep(old_string, position, length, replace_string);
 *to find position of first match:
 *		  int pos = strfind(string, search_pattern)
 *to count matches:
 *		  int count = strcnt(string, search_pattern)
 */


#ifndef k_lib_str_find_replace_h__
#define k_lib_str_find_replace_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//find needle in haystack and return its position 
static size_t strfind(const char * haystack, const char * needle);

//return count of  needle matches in haystack
static int strcnt(const char * haystack, const char * needle);

//return new allocated string with contents of haystack replaced from position to position+len by replace string  
static char * strrep(const char * haystack, size_t pos, size_t len, const char * replace);

//return new allocated string with contents of haystack replaced all needle matches by replace string
static char * strarep(const char * haystack, const char * needle, const char * replace);

/**
 * implimation of functions
 */

size_t 
strfind( 
		const char * haystack, 
		const char * needle
		)
{
	//found position of search word in haystack
	size_t i;
	for (i = 0; haystack[i] != '\0'; i++) {
		if (strstr(&haystack[i], needle) == &haystack[i]) {
			return i;
		}
	}	

	return -1;
}

int
strcnt(
		const char * haystack,   //string to scan 
		const char * needle		 //search pattern
		)
{
	int needlelen = strlen(needle);

	//count cases of search word in haystack
	int i, cnt = 0;
	for (i = 0; haystack[i] != '\0'; i++) {
		if (strstr(&haystack[i], needle) == &haystack[i]) {
			cnt++;

			// Jumping to index after the needle word.
			i += needlelen - 1;
		}
	}

	return cnt;
}

char * 
strrep(
		const char * haystack,       //string
		size_t pos,		             //start position
		size_t len,					 //length of replace string
		const char * replace	     //replace string
		)
{
	//find len of string
	size_t stringlen = strlen(haystack);
	size_t replacelen = strlen(replace);

	//allocate result string with enough memory
	char * result = (char *)malloc(stringlen - len + replacelen + 1);
	if (result == NULL) {
		perror("result string malloc");
		exit(EXIT_FAILURE);
	}	

	//replace string
	int i = 0;
	while (*haystack) {
		if (i == pos) { //if found word
			strncat(result, replace, replacelen); //copy new string to result
			i += replacelen; //move result counter
			haystack += len; //move read string counter
		} else {
			result[i++] = *haystack++;
		}
	}
	result[i] = '\0';
	return result;
}

char * //return result string
strarep(
		const char * haystack,       //string to scan 
		const char * needle,		 //search pattern
		const char * replace		 //replace string
	){

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
			strncat(result, replace, replacelen); //copy new string to result
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

#endif //k_lib_str_find_replace_h__
