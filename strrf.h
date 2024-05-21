/**
 * File              : strrf.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 27.05.2022
 * Last Modified Date: 21.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * strrf.h
 * Copyright (c) 2024 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STRRF_H
#define STRRF_H

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

#endif // STRRF_H

