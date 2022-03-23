/**
 * File              : list.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 23.03.2022
 * Last Modified Date: 23.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#ifndef k_lib_list_h__
#define k_lib_list_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct list_node {
	struct list_node *prev;
	void *data;
} LIST;

#define LIST_MALLOC()	\
({	\
	LIST* const ___p = malloc(sizeof(LIST)); \
	if(!___p) {perror("Malloc"); exit(EXIT_FAILURE);} \
	___p;\
})

#define LIST_FREE(list) \
({\
	LIST* ___ptr = list; \
	while(___ptr) { \
		LIST* const ___prev_list = ___ptr->prev; \
		free(___ptr); \
		___ptr = ___prev_list; \
	} \
})

#define LIST_NEW() ({LIST* const ___list = LIST_MALLOC(); ___list->prev = NULL; ___list->data = NULL; ___list;})

#define LIST_PREPEND_ITEM(list, item) \
({ \
	LIST* const ___prev_list = list; \
	LIST* const ___new_list = LIST_NEW(); \
	___new_list->prev = ___prev_list; \
	___new_list->data = item; \
	list = ___new_list; \
	list; \
})


#define LIST_APPEND_ITEM(list, item) \
({ \
	LIST* ___ptr = list; \
	if (___ptr->prev) { \
		LIST* const ___new_list = LIST_NEW(); \
		___new_list->data = item; \
		while(___ptr->prev->prev) \
			___ptr = ___ptr->prev; \
		LIST* const ___root_list = ___ptr->prev; \
		___new_list->prev = ___root_list; \
		___ptr->prev = ___new_list; \
	} else { \
		LIST_PREPEND_ITEM(list, item); \
	} \
	list; \
})
#define LIST_ADD(list, item) LIST_APPEND_ITEM(list, item)

#define LIST_FOR_EACH(list, item) \
	LIST* ___ptr = list; \
	void* item; \
	for (item = ___ptr->data; ___ptr->prev; ___ptr=___ptr->prev, item = ___ptr->data)

#define LIST_SIZE(list) \
({ \
	int ___size = 0; \
	LIST* ___ptr = list; \
	while(___ptr->prev) {\
		___ptr = ___ptr->prev; \
		___size = ___size + 1; \
	} \
___size; \
})

#ifdef __cplusplus
}
#endif

#endif //k_lib_list_h__
