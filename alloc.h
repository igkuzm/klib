/**
 * File              : alloc.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 22.02.2022
 * Last Modified Date: 22.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_alloc_h__
#define k_lib_alloc_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

// memory allocation helpers
#define ALLOC_SIZE(ptr) ((size_t*)ptr)[-1]

#define MALLOC(size)	\
({	\
	size_t* const ret = malloc(sizeof(size_t) + size); \
	if(!ret) {perror("Malloc"); exit(EXIT_FAILURE);} \
	*ret = size; \
	void* const ___p = &ret[1]; \
	___p;\
})

#define REALLOC(ptr, size)	\
({	\
	size_t* const ret = realloc((size_t *)ptr -1, sizeof(size_t) + size);	\
	if(!ret) { perror("Realloc"); exit(EXIT_FAILURE); }	\
	*ret = size; \
	void* const ___p = &ret[1]; \
	___p;\
})

#define RESIZE_ARRAY(array, count) \
({ \
	size_t size = ALLOC_SIZE(array); \
	size_t new_size = size + sizeof(typeof(array)) * count; \
	REALLOC(array, new_size); \
})	

#define INCREASE(ptr) RESIZE_ARRAY(ptr, 1);

#define FREE(ptr) \
({\
	if (ptr)\
		free((size_t *)ptr -1);\
	ptr = NULL;\
})

#define NEW(T) ((T*)MALLOC(sizeof(T)))

#ifdef __cplusplus
}
#endif

#endif //k_lib_alloc_h__
