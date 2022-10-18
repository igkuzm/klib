/**
 * File              : smsearch.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 22.02.2022
 * Last Modified Date: 18.10.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * 
 */

#ifndef k_lib_smsearch_h__
#define k_lib_smsearch_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct needle_list_t{
	char *needle;
	struct needle_list_t *next;
} needle_list;

static needle_list * needle_list_new(char *needle);
static void needle_list_add(needle_list *list, char *needle);

static void smsearch(
		const char *haystack,
		needle_list *list,
		void * userdata,
		int (*callback)(
			void * userdata,
			char * haystack_ptr,
			char * needle
			)
		);
/*
 * IMP
 */

needle_list * needle_list_new(char *needle) {
	needle_list *list = malloc(sizeof(needle_list));
	if (!list)
		return NULL;
	list->needle = needle;
	list->next = NULL;
	return list;
}

void needle_list_add(needle_list *list, char *needle){
	needle_list *ptr = list;
	//goto last list
	while(ptr->next)
		ptr = ptr->next;
	//add new list
	needle_list *new_list = needle_list_new(needle);
	new_list->next = NULL;
	ptr->next = new_list;
}

bool strmatch(char *haystack, char *needle){
	while(*needle)
		if (*needle++ != *haystack++)
			return false;
	return true;
}

void smsearch(
		const char *haystack,
		needle_list *list,
		void * userdata,
		int (*callback)(
			void * userdata,
			char * haystack_ptr,
			char * needle
			)
		)
{
	char * hp = (char *)haystack; //pointer to haystack
	
	size_t i = 0;
	needle_list *ptr = list; //pointer to list
	needle_list *prev = NULL;
	while(hp[i]){ //iterate haystack
		iterate:;
		if (*hp == 32){ //if new word
			ptr = list; //pointer to list - update search list
		}

		needle_list *p = ptr; //pointer to iterate search strings
		prev = NULL;
		while(p){
			char * np = p->needle;
			char * hpp = &*hp;
			if (strmatch(np, hpp)){
				//callback result - stop if return non zero
				if (callback)
					if (callback(userdata, (char *)&haystack[i], p->needle))
						return;
				//skip needle_len
				size_t len = strlen(p->needle);
				i += len;
				ptr = list;
				prev = NULL;
				goto iterate;
			} else {
				//remove needle from search;
				if (prev)
					prev->next = p->next;
				else 
					ptr = p->next;
			}

			//iterate list
			prev = p;
			p = p->next;
		}

		i++;
	}
}


#ifdef __cplusplus
}
#endif

#endif //k_lib_smsearch_h__
