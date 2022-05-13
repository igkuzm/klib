/**
 * File              : list.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 23.03.2022
 * Last Modified Date: 13.05.2022
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
	struct list_node *next;
	void *data;
} LIST;

//allocate and init new LIST
#define LIST_NEW()\
({\
	LIST* const ___list = malloc(sizeof(LIST));\
	if(!___list) {perror("malloc"); exit(EXIT_FAILURE);}\
	___list->next = NULL; ___list->data = NULL;\
   	___list;\
})

//free memory of LIST
#define LIST_FREE(list) \
({\
	LIST* ___ptr = list; \
	while(___ptr) { \
		LIST* const ___next_list = ___ptr->next; \
		free(___ptr); \
		___ptr = ___next_list; \
	} \
})

//prepend list item
#define LIST_PREPEND(list, item) \
({ \
	LIST* const ___next_list = list; \
	LIST* const ___new_list = LIST_NEW(); \
	___new_list->next = ___next_list; \
	___new_list->data = item; \
	list = ___new_list; \
	list; \
})
#define LIST_POP(list, item) LIST_PREPEND(list, item)

//append list item
#define LIST_APPEND(list, item) \
({ \
	LIST* ___ptr = list; \
	if (___ptr->next) { \
		LIST* const ___new_list = LIST_NEW(); \
		___new_list->data = item; \
		while(___ptr->next->next) \
			___ptr = ___ptr->next; \
		LIST* const ___root_list = ___ptr->next; \
		___new_list->next = ___root_list; \
		___ptr->next = ___new_list; \
	} else { \
		LIST_PREPEND(list, item); \
	} \
	list; \
})
#define LIST_ADD(list, item) LIST_APPEND(list, item)
#define LIST_PUSH(list, item) LIST_APPEND(list, item)

//return data value of item at index
#define LIST_AT(list, index) \
({ \
	int ___i = 0; \
	void *___data = NULL; \
	LIST* ___ptr = list; \
	while(___ptr->next) {\
		if (___i == index) ___data = ___ptr->data;\
		___ptr = ___ptr->next; \
		___i++; \
	}\
___data; \
})
#define LIST_GET(list, index) LIST_AT(list, index)

//remove item at index, return data value
#define LIST_REMOVE(list, index) \
({ \
	void *___data = NULL; \
	LIST* ___ptr = list; \
	if (index == 0 ){\
		___data = ___ptr->data;\
		list = ___ptr->next;\
		free(___ptr);\
	} else {\
		int ___i = 0; \
		LIST* ___prev = NULL; \
		while(___ptr->next) {\
			if (___i == index) {\
				___data = ___ptr->data;\
				___prev->next = ___ptr->next; \
				free(___ptr); \
				break; \
			}\
			___prev = ___ptr; \
			___ptr = ___ptr->next; \
			___i++; \
		}\
	}\
___data; \
})

//insert item at index
#define LIST_INSERT(list, item, index) \
({ \
	int ___isAdded = 0;\
	LIST* ___ptr = list; \
	LIST* ___prev = NULL; \
	if (index == 0 ){\
		LIST_PREPEND(list, item);\
		___isAdded = 1;\
	} else {\
		int ___i = 0; \
		while(___ptr->next) {\
			if (___i == index) {\
				LIST* ___new = LIST_NEW();\
				___new->data = item;\
				___prev->next = ___new; \
				___new->next = ___ptr; \
				___isAdded = 1;\
			}\
			___prev = ___ptr; \
			___ptr = ___ptr->next; \
			___i++; \
		}\
	}\
	if (!___isAdded) {\
		LIST* ___new = LIST_NEW();\
		___new->data = item;\
		___prev->next = ___new; \
		___new->next = ___ptr; \
	};\
})
#define LIST_SET(list, item, index) LIST_INSERT(list, item, index)

//move item from index to index
#define LIST_MOVE(list, from, to) \
({ \
	void * ___data = LIST_REMOVE(list, from);\
	LIST_INSERT(list, ___data, to);\
})

//foreach item at list
#define LIST_FOR_EACH(list, item) \
	LIST* ___ptr = list; \
	void* item; \
	for (item = ___ptr->data; ___ptr->next; ___ptr=___ptr->next, item = ___ptr->data)

#define LIST_SIZE(list) \
({ \
	int ___size = 0; \
	LIST* ___ptr = list; \
	while(___ptr->next) {\
		___ptr = ___ptr->next; \
		___size++; \
	} \
___size; \
})
#define LIST_COUNT(list) LIST_SIZE(list)

#ifdef __cplusplus
}
#endif

#endif //k_lib_list_h__
