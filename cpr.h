/**
 * File              : cpr.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 16.10.2022
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
#include <sys/stat.h>
#include "cp.h"
#include "ls.h"
#include "fexists.h"
	
static int cpr(const char *from, const char *to, bool overwrite);

struct k_lib_cp_recursive_args {
	const char *source;
	const char *dest;
	bool overwrite;
	int depth;
};

int k_lib_cp_recursive_callback(char *filename, FILETYPE filetype, void *user_data){
	struct k_lib_cp_recursive_args *args = (struct k_lib_cp_recursive_args *)user_data;
	
	char source[BUFSIZ];
	char dest[BUFSIZ];

#if defined _WIN32
	sprintf(source, "%s\\%s", args->source, filename);
	sprintf(dest, "%s\\%s", args->dest, filename);
#else
	sprintf(source, "%s/%s", args->source, filename);
	sprintf(dest, "%s/%s", args->dest, filename);
#endif
	
	if (filetype == FILETYPE_FILE)
		if (args->overwrite || !fexists(dest))
			cp(source, dest);

	if (filetype == FILETYPE_DIR){
		if (!fexists(dest) && mkdir(dest, 0700) == -1) {
			char error[BUFSIZ];
			sprintf(error, "Can't create directory: %s", dest);
			perror(error);
		} else {
			if (args->depth < 10 ) {
				struct k_lib_cp_recursive_args new_ags;
				new_ags.source = source;
				new_ags.dest = dest;
				new_ags.depth = args->depth + 1;			
			
				ls(source, &new_ags, k_lib_cp_recursive_callback);
			}
		}
	}

	return 0;
}

int cpr(const char *from, const char *to, bool overwrite) {
	struct k_lib_cp_recursive_args args = {
		args.source = from,
		args.dest = to,
		args.depth = 0,
		args.overwrite = overwrite
	};
	return ls(from, &args, k_lib_cp_recursive_callback);
}


#ifdef __cplusplus
}
#endif

#endif //k_lib_cp_recursive_h__
