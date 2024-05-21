/**
 * File              : strsubstr.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 21.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * strsubstr.h
 * Copyright (c) 2022 Igor V. Sementsov <ig.kuzm@gmail.com>
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

/* 
 * Substring function
 */

#ifndef STRSUBSTR_H
#define STRSUBSTR_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>
#include <string.h>

/* strsubstr
 * return allocated string with contents of str 
 * from position pos and lendth len 
 */
static char * strsubstr(
		const char *str, int pos, int len)
{
	char *substr = (char*)malloc(len+1);
	if (!substr)
		return NULL;
	
	memcpy(substr, &str[pos], len);
	substr[len] = '\0';

	return substr;
}
#ifdef __cplusplus
}
#endif

#endif //STRSUBSTR_H
