/**
 * File              : config.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 02.02.2023
 * Last Modified Date: 02.02.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
/* Read config file */

#ifndef k_lib_config_h__
#define k_lib_config_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define CONFIG_ARG_MAX_BYTES 128

/*
 * read file and run callback for each key/value pair
 */
void read_config_file(const char* path, void *user_data, 
		int (*callback)(void *user_data, char *key, char *value)) 
{
  FILE* fp;
	char buf[BUFSIZ], key[CONFIG_ARG_MAX_BYTES], value[CONFIG_ARG_MAX_BYTES];
	int i, ch, blen=0, klen=0, vlen=0, first=0, match=0, comment=0;

  if ((fp = fopen(path, "r+")) == NULL) {
      perror("fopen()");
      return;
  }

	while((ch=fgetc(fp))!=EOF){
		if (ch=='#' && first)
			comment = 1;	
		if (ch=='=') {
			key[klen+1] = 0;
			match = 1;
		}
		if (ch=='\n'){
			if (match & !comment) {
				value[vlen+1] = 0;
				if (callback)
					if (callback(user_data, key, value))
						break;
			}
			for (i = 0; i < klen; ++i)
				key[i] = 0;
			klen = 0;
			for (i = 0; i < vlen; ++i)
				value[i] = 0;
			vlen = 0;
			match = 0;
			comment = 0;
			first = 1;
			continue;
		}
		if (match)
			value[vlen++] = ch;
		else
			key[klen++] = ch;
		first = 0;
	}

	fclose(fp);
}

#ifdef __cplusplus
}
#endif

#endif // k_lib_config_h__
