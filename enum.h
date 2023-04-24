/**
 * File              : enum.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 24.04.2023
 * Last Modified Date: 24.04.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
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
 * BEGIN_ENUM(Days){
 *		#define DAY(x) DECL_ENUM_ELEMENT(x), 
 *      DAYS
 *      #undef DAY
 * }
 * END_ENUM(Days)
 *
 * BEGIN_ENUM_STRING( Days ){
 *		#define DAY(x) DECL_ENUM_STRING_ELEMENT(x), 
 *      DAYS
 *      #undef DAY
 * }
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
 */

#ifndef ENUM_H
#define ENUM_H

    #define BEGIN_ENUM( ENUM_NAME ) typedef enum tag##ENUM_NAME
    #define DECL_ENUM_ELEMENT( element ) element
    #define END_ENUM( ENUM_NAME ) ENUM_NAME;
    
    #define BEGIN_ENUM_STRING( ENUM_NAME ) static char* gs_##ENUM_NAME [] =
	#define DECL_ENUM_STRING_ELEMENT( element ) #element
    #define END_ENUM_STRING( ENUM_NAME ) ; static char* getString##ENUM_NAME(enum \
            tag##ENUM_NAME index){ return gs_##ENUM_NAME [index]; }

#endif /* ifndef ENUM_H */
