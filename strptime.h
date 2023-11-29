/**
 * File              : strptime.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 29.11.2023
 * Last Modified Date: 29.11.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef K_LIB_STRPTIME
#define K_LIB_STRPTIME
#include <stdio.h>
#include <time.h>

static time_t 
strpdate(const char *text, struct tm *tp)
{
	int d, m, y;
	sscanf(text, "%d.%d.%d", &d, &m, &y);
	struct tm tm;
	tm.tm_mday=d; tm.tm_mon=m-1; tm.tm_year=y-1900;
	tm.tm_hour = tm.tm_min = tm.tm_sec = 0;
	if (tp)
		*tp = tm;
	return mktime(&tm);
}

#endif /* ifndef strptime */
