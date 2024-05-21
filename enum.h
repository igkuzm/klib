/**
 * File              : enum.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 24.04.2023
 * Last Modified Date: 21.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * enum.h
 * Copyright (c) 2023 Igor V. Sementsov <ig.kuzm@gmail.com>
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
 * macros for emun
 *
 * to use include:
 *
 * #define DAYS \
 *	DAY(monday)\
 *	DAY(tuesday)\
 *	DAY(wednesday)\
 *	...etc
 *
 * BEGIN_ENUM(Days)
 *		#define DAY(x) DECL_ENUM_ELEMENT(x), 
 *      DAYS
 *      #undef DAY
 * END_ENUM(Days)
 *
 * BEGIN_ENUM_STRING( Days )
 *		#define DAY(x) DECL_ENUM_STRING_ELEMENT(x), 
 *      DAYS
 *      #undef DAY
 * END_ENUM_STRING(Days)
 * 
 * then you can use: 
 *
 * switch(day)
 *	case monday: break;
 *	case tuesday: break;
 *	case wednesday: 
 *			printf("Today is %s\n", getStringDays(day));
 *			break;
 *
 * or you can get enum index by it's name
 * int index = getIndexDays("monday");
 */

#ifndef ENUM_H
#define ENUM_H
	#include <string.h>

    #define BEGIN_ENUM( ENUM_NAME ) typedef enum tag##ENUM_NAME {
    #define DECL_ENUM_ELEMENT( element ) element
    #define END_ENUM( ENUM_NAME ) } ENUM_NAME; 

    #define BEGIN_ENUM_STRING( ENUM_NAME ) static char* gs_##ENUM_NAME [] = {
	#define DECL_ENUM_STRING_ELEMENT( element ) #element
    #define END_ENUM_STRING( ENUM_NAME ) NULL};\
		static char* getString##ENUM_NAME(enum \
            tag##ENUM_NAME index){ return gs_##ENUM_NAME [index]; }\
		static int getIndex##ENUM_NAME(const char *name){\
			char **ptr = gs_##ENUM_NAME;\
			int i = 0;\
			while (ptr[i]){\
				if (strcmp(ptr[i++], name) == 0)\
					return --i;\
			}\
		   	return -1;\
		}
	
#endif /* ifndef ENUM_H */
