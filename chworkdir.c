/**
 * File              : chworkdir.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 21.02.2022
 * Last Modified Date: 21.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "chworkdir.h"
#include "cp.h"
#include "ls.h"
#include "cp_recursive.h"

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>


int k_lib_chWorkDir(char *argv[]){
	char *execDir=dirname(argv[0]);
	char workDir[BUFSIZ];
#ifdef __APPLE__
		sprintf(workDir, "%s/../Resources", execDir); //workdir for Apple is BUNDLE/../Resources
#elif defined _WIN32 || defined _WIN64
		sprintf(workDir, "%s", execDir); //workdir for Windows is executable dir
#else
		//for Unix systems
		//find homedir and create new dir
		char *progname = basename(argv[0]);
		sprintf(workDir, "%s/%s", getenv("HOME"), progname); 
		
		//create directory if not exists
		struct stat st;
		if (stat(workDir, &st) == -1) {
			if (mkdir(workDir, 0700) == -1) {
				perror("Can't create directory at $(HOME)");
			}			
		}

		//find executable path
		char selfpath[128];
		memset (selfpath, 0, sizeof (selfpath));
		if (readlink ("/proc/self/exe", selfpath, sizeof (selfpath) - 1) < 0)
			{
				perror ("first readlink");
				exit (EXIT_FAILURE);
			};
		execDir=dirname(selfpath);

		char resourceDir[BUFSIZ];
		sprintf(resourceDir, "%s/../share/%s", execDir, progname); //resources dir

		//copy files from resources to workdir
		k_lib_cp_recursive(resourceDir, workDir);
		
#endif
	chdir(workDir); //change workdir
	printf("Workdir changed to:%s\n", workDir);
	return 0;
}
