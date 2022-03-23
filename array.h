/**
 * File              : array.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 22.02.2022
 * Last Modified Date: 23.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_array_h__
#define k_lib_array_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

//Dynamyc array
//to use: 
//char* array = ARRAY_NEW(char);
//char* word = "hello";
//for (int i = 0, i < strlen(word), i++)
//    array = ARRAY_APPEND(array, char, word[i]);
//ARRAY_APPEND(array, char, '');
//ARRAY_APPEND_ITEMS(array, char, 'w', 'o', 'r', 'l', 'd', '!');
//for EACH_IN_ARRAY(char, c, array)
//    printf("%c\n", c);

#define ARRAY_ALLOC(size)	\
({	\
	size_t* const ___ret = malloc(sizeof(size_t) + size); \
	if(!___ret) {perror("Malloc"); exit(EXIT_FAILURE);} \
	*___ret = size; \
	void* const ___p = &___ret[1]; \
	___p;\
})

#define ARRAY_NEW(T) ((T*)ARRAY_ALLOC(sizeof(T)))

#define ARRAY_POINTER(array) ((size_t *)array -1)

#define ARRAY_ALLOCATED_SIZE(array) ((size_t*)array)[-1]

#define ARRAY_SIZE(array, type) ({ARRAY_ALLOCATED_SIZE(array)/sizeof(type) - 1;})

#define ARRAY_REALLOC(array, size)	\
({	\
	size_t* const ___ret = realloc(ARRAY_POINTER(array), sizeof(size_t) + size);	\
	if(!___ret) { perror("Realloc"); exit(EXIT_FAILURE); }	\
	*___ret = size; \
	void* const ___p = &___ret[1]; \
	___p;\
})

#define ARRAY_APPEND_ITEM(array, type, item) \
({ \
	size_t ___size = ARRAY_SIZE(array, type); \
	size_t ___new_size = ARRAY_ALLOCATED_SIZE(array) + sizeof(type); \
	array[___size] = item; \
	array = ARRAY_REALLOC(array, ___new_size); \
})

#define ARRAY_APPEND(array, type, ...) \
({ \
	int ___count = sizeof((int[]){__VA_ARGS__})/sizeof(int); \
	size_t ___size = ARRAY_SIZE(array, type); \
	size_t ___new_size = ARRAY_ALLOCATED_SIZE(array) + sizeof(type) * ___count; \
	array = ARRAY_REALLOC(array, ___new_size); \
	int ___i; \
	for (___i = 0; ___i < ___count; ++___i) { \
		type const ___item = (type[]){__VA_ARGS__}[___i]; \
		array[___size + ___i] = ___item; \
	} \
	array; \
})

#define EACH_IN_ARRAY(type, item, array) \
(type *p = (array), (item) = *p; p < &((array)[ARRAY_SIZE(array, type)]); p++, (item) = *p)

#define ARRAY_FOR_EACH(array, type, item) for EACH_IN_ARRAY(type, item, array)

#define ARRAY_FREE(array) \
({\
	if (array)\
		free(ARRAY_POINTER(array));\
	array = NULL;\
})

#ifdef __cplusplus
}
#endif

#endif //k_lib_array_h__
