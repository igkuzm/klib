/**
 * File              : strrf.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 27.05.2022
 * Last Modified Date: 16.04.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_striio_h__
#define k_lib_striio_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>

// read file to string

/* strrf
 * return allocated string with the contents of file */
static char * 
strrf(FILE * fp)
{
	size_t len = 0;
        
	if (fseek (fp, 0, SEEK_END))
		return NULL;

  if ((len = ftell(fp)) < 0)
		return NULL;
    
	if (fseek (fp, 0, SEEK_SET))
		return NULL;

  char *buff = (char *)malloc(len+1);
	if (!buff)
		return 0;

	fread(buff, len, 1, fp);	
	buff[len] = 0;

	return buff;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_string_h__

