/**
 * File              : translit.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 07.03.2022
 * Last Modified Date: 23.10.2022
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

//#define TRANSLIT_LINEAR_SEARCH

static char * translit(const char * string);


/*
 * IMP
 */

typedef struct translit_t {
	uint32_t utf_code;
	char utf[5];
	char ascii[4];
} translit_t;

//for binary search we need sorted array
static translit_t translit_table[] = 
{
	{0x0401, "Ё", "YO" },
	{0x0410, "А", "A"  },
	{0x0411, "Б", "B"  },
	{0x0412, "В", "B"  },
	{0x0412, "Г", "G"  },
	{0x0413, "Д", "D"  },
	{0x0414, "Е", "E"  },
	{0x0415, "Ж", "ZH" },
	{0x0416, "З", "Z"  },
	{0x0418, "И", "I"  },
	{0x0419, "Й", "Y"  },
	{0x041A, "К", "K"  },
	{0x041B, "Л", "L"  },
	{0x041C, "М", "M"  },
	{0x041D, "Н", "N"  },
	{0x041E, "О", "O"  },
	{0x041F, "П", "P"  },
	{0x0420, "Р", "R"  },
	{0x0420, "Р", "R"  },
	{0x0421, "С", "S"  },
	{0x0422, "Т", "T"  },
	{0x0423, "У", "U"  },
	{0x0424, "Ф", "PH" },
	{0x0425, "Х", "KH" },
	{0x0426, "Ц", "TS" },
	{0x0427, "Ч", "TCH"},
	{0x0428, "Ш", "SH" },
	{0x0429, "Щ", "SCH"},
	{0x042A, "Ъ", "'"  },
	{0x042B, "Ы", "Y"  },
	{0x042C, "Ь", "'"  },
	{0x042D, "Э", "E"  },
	{0x042E, "Ю", "YU" },
	{0x042F, "Я", "YA" },
	{0x0430, "а", "a"  },
	{0x0431, "б", "b"  },
	{0x0432, "в", "v"  },
	{0x0433, "г", "g"  },
	{0x0434, "д", "d"  },
	{0x0435, "е", "e"  },
	{0x0436, "ж", "zh" },
	{0x0437, "з", "z"  },
	{0x0438, "и", "i"  },
	{0x0439, "й", "y"  },
	{0x043A, "к", "k"  },
	{0x043B, "л", "l"  },
	{0x043C, "м", "m"  },
	{0x043D, "н", "n"  },
	{0x043E, "о", "o"  },
	{0x043F, "п", "p"  },
	{0x0440, "р", "r"  },
	{0x0441, "с", "c"  },
	{0x0442, "т", "t"  },
	{0x0443, "у", "u"  },
	{0x0444, "ф", "ph" },
	{0x0445, "х", "kh" },
	{0x0446, "ц", "ts" },
	{0x0447, "ч", "tch"},
	{0x0448, "ш", "sh" },
	{0x0449, "щ", "sch"},
	{0x044A, "ъ", "'"  },
	{0x044B, "ы", "y"  },
	{0x044C, "ь", "'"  },
	{0x044D, "э", "e"  },
	{0x044E, "ю", "yu" },
	{0x044F, "я", "ya" },
	{0x0451, "ё", "yo" },
	{0x2116, "№", "#"  },
};

#ifndef TRANSLIT_LINEAR_SEARCH
//function for binary search
static int translit_compare (const void * s1, const void * s2) {
    return ((translit_t *)s1)->utf_code - ((translit_t *)s2)->utf_code;
}
#endif

char * 
translit(
		const char * str
		)
{
	//get size of string
	int len = strlen(str);
	
	//allocate return string
	char * ret = malloc(len*3 +1);
	if (!ret)
		return NULL;

	//get size of translit table
	int translit_table_size = 
			sizeof(translit_table) / sizeof(translit_t);
	

	int i;

	char * ptr = (char *)str; //pointer to string
	while(*ptr){ //check utf8 char size
		int buflen = 0;
		char buf[5];
		if ((*ptr & 0b11110000) == 240) {     //4 byte
			for (i = 0; i < 4; i++) {
				buf[i] = *ptr++;
				buflen++;
			} 
			buf[4] = 0;
		} 
		else if ((*ptr & 0b11100000) == 224){ //3 byte
			for (i = 0; i < 3; i++) {
				buf[i] = *ptr++;
				buflen++;
			}
			buf[3] = 0;
		}		  
		else if ((*ptr & 0b11000000) == 192){ //2 byte
			for (i = 0; i < 2; i++) {
				buf[i] = *ptr++;
				buflen++;
			}			
			buf[2] = 0;
		}		  
		else {                                //1 byte - ANSY
			buf[0] = *ptr++;
			buf[1] = 0;
		}

		//find translit
#ifdef TRANSLIT_LINEAR_SEARCH
		//linear search
		char matches = 0; 
		for (i = 0; i < translit_table_size; i++){
			if (strcmp(buf, translit_table[i].utf) == 0){
				matches = 1;
				break;
			}
		}

		if (matches)
			strcat(ret, translit_table[i].ascii);
		else
			strcat(ret, buf);
#else
		//binary search
		translit_t value = {0};
		switch (buflen) {
			case 4: {
				value.utf_code = (
						((buf[0] & 0b00000111) << 18) |	   
						((buf[1] & 0b00111111) << 12) |	   
						((buf[2] & 0b00111111) << 6 ) |	   
						( buf[3] & 0b00111111)
						);		
				break;
			}
			case 3: {
				value.utf_code = (
						((buf[0] & 0b00001111) << 12) |	   
						((buf[1] & 0b00111111) << 6 ) |	   
						( buf[2] & 0b00111111)
						);		
				break;
			}					
			case 2: {
				value.utf_code = (
						((buf[0] & 0b00011111) << 6 ) |	   
						( buf[1] & 0b00111111)
						);		
				break;
			}					

			default:
				break;
		}
		if (value.utf_code) {
			translit_t *translit = 
					bsearch(&value, translit_table, 
							translit_table_size, sizeof(translit_t), 
									translit_compare);
			if (translit)
				strcat(ret, translit->ascii);
			else
				strcat(ret, buf);
		} else
			strcat(ret, buf);
#endif
	}	

	return ret;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_translit_h__
