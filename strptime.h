/**
 * File              : strptime.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.10.2022
 * Last Modified Date: 04.10.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef STRPTIME_H 
#define STRPTIME_H
#include <time.h>
#ifdef _WIN32
#ifdef __cplusplus
extern "C"{
#endif
	char * strptime(const char* s, const char* f, struct tm* tm);
#ifdef __cplusplus
}
#endif	
#endif
#endif /* ifndef  */
