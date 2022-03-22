/**
 * File              : alloc.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 22.02.2022
 * Last Modified Date: 23.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_dyarray_h__
#define k_lib_dyarray_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

//Dynamyc array
//to use: 
//int* array = DYARRAY_NEW(int);
//DYARRAY_APPEND(array, int, 10)
//DYARRAY_APPEND(array, int, 12)

#define DYARRAY_ALLOC(size)	\
({	\
	size_t* const ret = malloc(sizeof(size_t) + size); \
	if(!ret) {perror("Malloc"); exit(EXIT_FAILURE);} \
	*ret = size; \
	void* const ___p = &ret[1]; \
	___p;\
})

#define DYARRAY_NEW(T) ((T*)DYARRAY_ALLOC(sizeof(T)))

#define DYARRAY_POINTER(array) ((size_t *)array -1)

#define DYARRAY_ALLOCATED_SIZE(array) ((size_t*)array)[-1]

#define DYARRAY_SIZE(array, type) DYARRAY_ALLOCATED_SIZE(array)/sizeof(type)

#define DYARRAY_REALLOC(array, size)	\
({	\
	size_t* const ret = realloc(DYARRAY_POINTER(array), sizeof(size_t) + size);	\
	if(!ret) { perror("Realloc"); exit(EXIT_FAILURE); }	\
	*ret = size; \
	void* const ___p = &ret[1]; \
	___p;\
})

#define DYARRAY_RESIZE(type, array, count) DYARRAY_REALLOC(array, DYARRAY_ALLOCATED_SIZE(array) + sizeof(type) * count);

#define DYARRAY_INCREASE(type, ptr) RESIZE_ARRAY(type, ptr, 1);

#define DYARRAY_APPEND(array, type, item) \
({ \
	array[DYARRAY_SIZE(array, type) - 1] = item; \
	INCREASE(type, array) \
})

#define DYARRAY_FREE(array) \
({\
	if (ptr)\
		free(DYARRAY_POINTER(array));\
	ptr = NULL;\
})

#ifdef __cplusplus
}
#endif

#endif //k_lib_dyarray_h__
