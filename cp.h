/**
 * File              : cp.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.09.2021
 * Last Modified Date: 29.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#ifndef k_lib_cp_h__
#define k_lib_cp_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

//copy file FROM path to destination TO path
int k_lib_cp(const char *from, const char *to);

#ifndef cp
#define cp(from, to)	\
({	\
	int ___c = k_lib_cp(from, to);\
	___c;	\
})
#endif

int k_lib_cp(const char *from, const char *to)
{
	FILE * fp_from = fopen(from, "r");
	if (!fp_from) 
		return -1;

	FILE * fp_to = fopen(to, "w");
	if (!fp_to){
		fclose(fp_from);
		return -1;
	} 

	int ch;
	while ((ch = fgetc(fp_from)) != EOF)
		fputc(ch, fp_to);
	
	fclose(fp_from);
	fclose(fp_to);

	return 0;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_cp_h__
