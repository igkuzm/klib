/**
 * File              : enum.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 24.04.2023
 * Last Modified Date: 24.04.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * macros for emun
 */
#ifdef DECL_ENUM_ELEMENT
#undef DECL_ENUM_ELEMENT 
#endif

#ifdef BEGIN_ENUM
#undef BEGIN_ENUM 
#endif

#ifdef END_ENUM
#undef END_ENUM 
#endif

#ifndef GENERATE_ENUM_STRINGS
    #define DECL_ENUM_ELEMENT( element ) element
    #define BEGIN_ENUM( ENUM_NAME ) typedef enum tag##ENUM_NAME
    #define END_ENUM( ENUM_NAME ) ENUM_NAME; \
            char* getString##ENUM_NAME(enum tag##ENUM_NAME index);
#else
    #define DECL_ENUM_ELEMENT( element ) #element
    #define BEGIN_ENUM( ENUM_NAME ) char* gs_##ENUM_NAME [] =
    #define END_ENUM( ENUM_NAME ) ; char* getString##ENUM_NAME(enum \
            tag##ENUM_NAME index){ return gs_##ENUM_NAME [index]; }
#endif
