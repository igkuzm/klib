/**
 * File              : string.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 27.05.2022
 * Last Modified Date: 27.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 *	Dynamic string
 *	Usage:
 *		struct string s;
 *		string_init(&s);
 *		string_append(&s, "text", 4);
 *		string_free(&s);
 */

#ifndef k_lib_string_h__
#define k_lib_string_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct string {
	char * ptr;
	size_t len;
};

// memory allocation helpers
#define STRING_MALLOC(size)	\
({	\
	char* const ___p = (char *)malloc(size); \
	if(!___p) {perror("Malloc"); exit(EXIT_FAILURE);} \
	___p;\
})

#define STRING_REALLOC(ptr, size)	\
({	\
	char* const ___s = ptr; \
	char* const ___p = (char *)realloc(___s, size);	\
	if(!___p) { perror("Realloc"); exit(EXIT_FAILURE); }	\
	___p;\
})

void string_init(struct string * s){
	s->len = 0;
	s->ptr = STRING_MALLOC(s->len + 1);
	s->ptr[0] = '\0';
}

void string_append(struct string * s, void * data, size_t len){
	size_t new_len = s->len + len;
	s->ptr = STRING_REALLOC(s->ptr, new_len+ 1); //realoc with enough space
	memcpy(s->ptr+s->len, data, len); //copy data
	s->ptr[new_len] = '\0';
	s->len = new_len;
}

void string_free(struct string * s) {
	free(s->ptr);
}


#ifdef __cplusplus
}
#endif

#endif //k_lib_string_h__

