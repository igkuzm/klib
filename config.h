/* Read config file */

#ifndef k_lib_config_h__
#define k_lib_config_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define CONFIG_ARG_MAX_BYTES 128

void read_config_file(char* path, void *user_data, int (*callback)(void *user_data, char *key, char *value)) {
    FILE* fp;
    
    if ((fp = fopen(path, "r+")) == NULL) {
        perror("fopen()");
        return NULL;
    }
    
    while(1) {
        char key[CONFIG_ARG_MAX_BYTES];
        char value[CONFIG_ARG_MAX_BYTES];
        
        if (fscanf(fp, "%s = %s", key, value) != 2) {
            //perror("fscanf()");
            continue;
        }
        
        if (key[0] == '#') {
            //while (fgetc(fp) != '\n') {
            // Do nothing (to move the cursor to the end of the line).
            //}
           continue;
        }

        //printf("Key: %s\nValue: %s\n", key, value);
        if (callback)
            if (!callback(user_data, key, value)) //stop exec if callback != 0
                break;
        
        if (feof(fp))
            break;
    }
}

#ifdef __cplusplus
}
#endif

#endif // k_lib_config_h__
