/**
 * File              : array.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 22.02.2022
 * Last Modified Date: 06.09.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * C library for dynamic arrays
 * struct array_t * array_new(T) - allocate and return new array of types T
 * void array_append(struct array_t * array, T, T item) - add item to array with type T
 * T item * array_at(struct array_t * array, T, int i) - return item from array at position i
 * array_for_each(struct array_t * array, T, T item){} - iterate the array
 * void array_free(struct array_t * array) - free array
 */

#ifndef k_lib_array_h__
#define k_lib_array_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

//Dynamyc array
struct array_t {
	void * data;
	int len;
};

#define array_new(T)\
	({\
		struct array_t * ___array = malloc(sizeof(struct array_t));\
		if (___array == NULL) {\
			perror("array_new malloc");\
			exit(EXIT_FAILURE);\
		}\
		___array->data = malloc(sizeof(T));\
		if (___array->data == NULL) {\
			perror("array_new data malloc");\
			exit(EXIT_FAILURE);\
		}\
		___array->len = 0;\
		___array;\
	 })

#define array_append(array, T, item) \
	({ \
		T * ___data = array->data;\
		___data[array->len] = item;\
		array->len++;\
		array->data = realloc(array->data, sizeof(T) + sizeof(T) * array->len);\
		if (array->data == NULL) {\
			perror("array_append_item realloc");\
			exit(EXIT_FAILURE);\
		}\
	})

#define array_at(array, T, i)\
	({\
		T * ___data = array->data;\
		___data[i];\
	})

#define array_for_each(array, T, item)\
		T * ___data = array->data;\
		T * ___p, item;\
		for (___p = (___data), (item) = *___p; ___p < &((___data)[array->len]); ___p++, (item) = *___p)\

#define array_free(array)\
	free(array->data);\
	free(array);

#ifdef __cplusplus
}
#endif

#endif //k_lib_array_h__
