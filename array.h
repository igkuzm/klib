/**
 * File              : array.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 22.02.2022
 * Last Modified Date: 22.07.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
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

#define array_new(type)\
	({\
		struct array_t * ___array = malloc(sizeof(struct array_t));\
		if (___array == NULL) {\
			perror("array_new malloc");\
			exit(EXIT_FAILURE);\
		}\
		___array->data = malloc(sizeof(type));\
		if (___array->data == NULL) {\
			perror("array_new data malloc");\
			exit(EXIT_FAILURE);\
		}\
		___array->len = 0;\
		___array;\
	 })

#define array_append(array, type, item) \
	({ \
		type * ___data = array->data;\
		___data[array->len] = item;\
		array->len++;\
		array->data = realloc(array->data, sizeof(type) + sizeof(type) * array->len);\
		if (array->data == NULL) {\
			perror("array_append_item realloc");\
			exit(EXIT_FAILURE);\
		}\
	})

#define array_at(array, type, i)\
	({\
		type * ___data = array->data;\
		___data[i];\
	})

#define array_for_each(array, type, item)\
		type * ___data = array->data;\
		type * ___p, item;\
		for (___p = (___data), (item) = *___p; ___p < &((___data)[array->len]); ___p++, (item) = *___p)\

#define array_free(array)\
	free(array->data);\
	free(array);

#ifdef __cplusplus
}
#endif

#endif //k_lib_array_h__
