/**
 * File              : strfilter.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 22.10.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_strfilter_h__
#define k_lib_strfilter_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//allocate new string and fill with str filtred by date like DD.MM.YYYY
static char * strfilter_date(const char * str);

//allocate new string and fill with str filtred by date and time like DD.MM.YYYY HH:MM
static char * strfilter_datetime(const char * str);

//allocate new string and fill with str filtred by allowd strings in null-terminated array
static char * strfilter_str(const char * str, const char * chars[]);

/*
 * IMP
 */

char * 
strfilter_date(const char * str)
{
	//pointer to string
	char * ptr = (char *)str;

	//allocate return buffer
	char * buf = malloc(BUFSIZ);
	if(!buf)
		return NULL;

	//iterate string
	int i = 0, dotcnt = 0;
	while(ptr[i]){
		//stop if overbuf
		if (i == BUFSIZ)
			break;
		
		if (ptr[i] >= '0' && ptr[i] <= '9') //check numbers
			*buf++ = ptr[i];

		if(ptr[i] == '.'){               //stop filter if there is more then 2 dots
			if (dotcnt < 2){
				dotcnt++;
				*buf++ = ptr[i];
			} else
				break;
		}

		i++;
	}

	//null-terminate string
	*buf = 0;

	return buf;
}

char * 
strfilter_datetime(const char * str)
{
	//pointer to string
	char * ptr = (char *)str;

	//allocate return buffer
	char * buf = malloc(BUFSIZ);
	if(!buf)
		return NULL;

	//iterate string
	int i = 0, dotcnt = 0, spaces = 0;
	while(ptr[i]){
		//stop if overbuf
		if (i == BUFSIZ)
			break;
		
		if (ptr[i] >= '0' && ptr[i] <= '9') //check numbers
			*buf++ = ptr[i];

		if(ptr[i] == '.'){               //stop filter if there is more then 2 dots
			if (dotcnt < 2){
				dotcnt++;
				*buf++ = ptr[i];
			} else
				break;
		}

		if(ptr[i] == ' ' && dotcnt == 2 && spaces == 0){ //allow space if dotcnt == 2
			spaces++;
			*buf++ = ptr[i];
		}

		if(ptr[i] == ':' && spaces == 1) //allow ':'
			*buf++ = ptr[i];

		i++;
	}
	
	//null-terminate string
	*buf = 0;

	return buf;
}

int 
_strfilter_strinc(
		const char * haystack, 
		const char * needle
		)
{
	char * hp = (char *)haystack; //haystack pointer
	char * np = (char *)needle;   //needle pointer

	//for each char in neddle check if matches haystack
	int len = 0;
	while(*np){
		len++;
		if (*hp++ != *np++)
			return 0;
	}
	
	return len;
}

char * 
strfilter_str(
		const char * str, 
		const char * chars[]
		)
{
	//pointer to string
	char * ptr = (char *)str;

	//allocate return buffer
	char * buf = malloc(BUFSIZ);
	if(!buf)
		return NULL;

	//iterate string
	int i, c = 0;
	while(ptr[c]){
		//stop if overbuf
		if (c == BUFSIZ)
			break;

		//pointer to array
		char ** pa = (char **)chars;		
		while(*pa){
			int len = _strfilter_strinc(&ptr[c], *pa);
			if (len)
				for (i = 0; i < len; i++) {
					*buf++ = ptr[c++];
				}
		}
		
		c++;
	}

	//null-terminate string
	*buf = 0;

	return buf;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_strfilter_h__
