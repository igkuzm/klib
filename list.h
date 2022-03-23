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

typedef struct list_node {
	struct list_node *prev;
	void *data;
} LIST;

#include <stdlib.h>

// memory allocation helpers
#define LIST_MALLOC()	\
({	\
	void* const ___p = malloc(sizeof(LIST)); \
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

#define LIST_PREPPEND(list, item) \
({ \
	LIST* const ___prev_list = list; \
	LIST* const ___new_list = LIST_NEW(); \
	___new_list->prev = ___prev_list; \
	___new_list->data = item; \
	list = ___new_list; \
	list; \
})

#define LIST_APPEND(list, item) \
({ \
	LIST* ___ptr = list; \
	while(___ptr->prev->prev) \
		___ptr = ___ptr->prev; \
	LIST* const ___root_list = ___ptr->prev; \
	LIST* const ___new_list = LIST_NEW(); \
	___new_list->prev = ___root_list; \
	___ptr->prev = ___new_list; \
	___new_list->data = item; \
	list; \
})

#define LIST_FOR_EACH(list, type, item) \
LIST* ___ptr = list; \
type item; \
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
