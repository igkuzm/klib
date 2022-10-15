/**
 * File              : translit.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 07.03.2022
 * Last Modified Date: 15.10.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */


#ifndef k_lib_translit_h__
#define k_lib_translit_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>	

static char * translit(const char * string);

static char * cyrilic[] = 
{
	"А", "Б", "В", "Г", "Д", "Е", "Ё", "Ж", "З", "И", "К", 
	"Л", "М", "Н", "О", "П", "Р", "С", "Т", "У", "Ф", "Х", 
	"Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я",
	"а", "б", "в", "г", "д", "е", "ё", "ж", "з", "и", "к", 
	"л", "м", "н", "о", "п", "р", "с", "т", "у", "ф", "х", 
	"ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я",
	"№", 
	NULL						
};

static char * latin[] = 
{
	"A", "B", "V", "G", "D", "E", "YO", "ZH", "Z", "I", "K", 
	"L", "M", "N", "O", "P", "R", "S", "T", "U", "PH", "KH", 
	"TS", "TCH", "SH", "SCH", "'", "Y", "'", "E", "YU", "YA",
	"a", "b", "v", "g", "d", "e", "yo", "zh", "z", "i", "k", 
	"l", "m", "n", "o", "p", "r", "s", "t", "u", "ph", "kh", 
	"ts", "tch", "sh", "sch", "'", "y", "'", "e", "yu", "ya",
	"#", 
	NULL						
};

char * 
translit(
		const char * str
		)
{
	int len = 0;
	char * ret = malloc(1);
	if (!ret)
		return NULL;

	int i;

	char * ptr = (char *)str; //pointer to string
	while(*ptr){ //check utf8 char size
		char buf[5];
		if ((*ptr & 0b11110000) == 240) {     //4 byte
			for (i = 0; i < 4; i++) {
				buf[i] = *ptr++;
			} 
			buf[4] = 0;
		} 
		else if ((*ptr & 0b11100000) == 224){ //3 byte
			for (i = 0; i < 3; i++) {
				buf[i] = *ptr++;
			}
			buf[3] = 0;
		}		  
		else if ((*ptr & 0b11000000) == 192){ //2 byte
			for (i = 0; i < 2; i++) {
				buf[i] = *ptr++;
			}			
			buf[2] = 0;
		}		  
		else {                                //1 byte - ANSY
			buf[0] = *ptr++;
			buf[1] = 0;
		}

		//realoc return string
		len += strlen(buf);
		ret = realloc(ret, 1 + len);
		if (!ret)
			return NULL;

		//find translit
		char matches = 0; 
		for (i = 0; cyrilic[i]; i++){
			if (strcmp(buf, cyrilic[i]) == 0){
				matches = 1;
				break;
			}
		}

		if (matches)
			strcat(ret, latin[i]);
		else
			strcat(ret, buf);
	}	

	return ret;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_translit_h__
