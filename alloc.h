/**
 * File              : alloc.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 22.02.2022
 * Last Modified Date: 23.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_alloc_h__
#define k_lib_alloc_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

// memory allocation helpers
#define MALLOC(size)	\
({	\
	void* const ___p = malloc(size); \
	if(!___p) {perror("Malloc"); exit(EXIT_FAILURE);} \
	___p;\
})

#define REALLOC(ptr, size)	\
({	\
	void* const ___s = ptr; \
	void* const ___p = realloc(___s, size);	\
	if(!___p) { perror("Realloc"); exit(EXIT_FAILURE); }	\
	___p;\
})

#define FREE(ptr) \
({\
	if (ptr)\
		free(ptr);\
	ptr = NULL;\
})

#define NEW(T) ((T*)MALLOC(sizeof(T)))

#ifdef __cplusplus
}
#endif

#endif //k_lib_alloc_h__
