/**
 * File              : translit.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 07.03.2022
 * Last Modified Date: 07.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */


#ifndef k_lib_translit_h__
#define k_lib_translit_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

char* k_lib_translit(const char* string);

#ifndef translit
#define translit(string)	\
({	\
	char* ___c = k_lib_translit(string);\
	___c;	\
})
#endif


#ifdef __cplusplus
}
#endif

#endif //k_lib_translit_h__
