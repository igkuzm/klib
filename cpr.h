/**
 * File              : cpr.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 06.09.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * copy directory recursive from path to destination
 */

#ifndef k_lib_cp_recursive_h__
#define k_lib_cp_recursive_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include "cp.h"
#include "ls.h"

#include <sys/stat.h>
	

int cpr(const char *from, const char *to);

struct k_lib_cp_recursive_args {
	const char *source;
	const char *dest;
	int depth;
};

int k_lib_cp_recursive_callback(KLibFile *file, void *user_data){
	struct k_lib_cp_recursive_args *args = (struct k_lib_cp_recursive_args *)user_data;
	
	char source[BUFSIZ];
	sprintf(source, "%s/%s", args->source, file->filename);
	
	char dest[BUFSIZ];
	sprintf(dest, "%s/%s", args->dest, file->filename);

#if defined _WIN32 || defined _WIN64
	sprintf(source, "%s\\%s", dir->source, file->filename);
	sprintf(dest, "%s\\%s", dir->dest, file->filename);
#endif
	
	if (file->filetype == FILETYPE_FILE)
		k_lib_cp(source, dest);

	if (file->filetype == FILETYPE_DIR){
		if (mkdir(dest, 0700) == -1) {
			perror("Can't create directory");
		} else {
			if (args->depth < 10 ) {
				struct k_lib_cp_recursive_args new_ags;
				new_ags.source = source;
				new_ags.dest = dest;
				new_ags.depth = args->depth + 1;			
			
				k_lib_ls(source, &new_ags, k_lib_cp_recursive_callback);
			}
		}
	}

	return 0;
}

int k_lib_cp_recursive(const char *from, const char *to) {
	struct k_lib_cp_recursive_args args;
	args.source = from;
	args.dest = to;
	args.depth = 0;
	return k_lib_ls(from, &args, k_lib_cp_recursive_callback);
}


#ifdef __cplusplus
}
#endif

#endif //k_lib_cp_recursive_h__
