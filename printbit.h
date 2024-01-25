/**
 * File              : printbit.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 22.02.2022
 * Last Modified Date: 25.01.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_math_h__
#define k_lib_math_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <stdio.h>

static void print64bit(int64_t c);
static void print32bit(int32_t c);
static void print8bit (int8_t c);

/*
 * IMP
 */

void 
print64bit(int64_t c) {
	for (int i = 63; i >= 0; --i) {
		if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55)
			printf(" ");
	   printf("%c", (c & (1 << i)) ? '1' : '0');
	}
	printf("\n");
}

void 
print32bit(int32_t c) {
	for (int i = 31; i >= 0; --i) {
		if (i == 7 || i == 15 || i == 23)
			printf(" ");
	   printf("%c", (c & (1 << i)) ? '1' : '0');
	}
	printf("\n");
}

void 
print8bit(int8_t c) {
	for (int i = 7; i >= 0; --i) {
	   printf("%c", (c & (1 << i)) ? '1' : '0');
	}
	printf("\n");
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_math_h__
