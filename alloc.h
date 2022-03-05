/**
 * File              : alloc.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 22.02.2022
 * Last Modified Date: 23.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_alloc_h__
#define k_lib_alloc_h__
#endif

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

// memory allocation helpers
#define ALLOC(size)	\
({	\
	void* const ___p = malloc(size);	\
	if(!___p) {perror("Malloc"); exit(EXIT_FAILURE);}	\
	___p;	\
})

#define REALLOC(ptr, size)	\
({	\
	void* const ___s = (ptr);	\
	void* const ___p = realloc(___s, (size));	\
	if(!___p) { perror("Realloc"); exit(EXIT_FAILURE); }	\
	___p;	\
})

#define FREE(ptr) \
({\
	if (ptr)\
		free(ptr);\
	ptr = NULL;\
})


#ifdef __cplusplus
}
#endif
