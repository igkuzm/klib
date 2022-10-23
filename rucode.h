/**
 * File              : rucode.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 07.03.2022
 * Last Modified Date: 23.10.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */


#ifndef k_lib_rucode_h__
#define k_lib_rucode_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>	
#include <stdbool.h>	

//#define TRANSLIT_LINEAR_SEARCH

typedef enum {
	RUCODE_CODEPAGE_UTF8,
	RUCODE_CODEPAGE_ASCII,
	RUCODE_CODEPAGE_CP1251,
	RUCODE_CODEPAGE_KOI8R,
	RUCODE_CODEPAGE_CP866,
	RUCODE_CODEPAGE_ISO8859_5
} RUCODE_CODEPAGE;

/*
 * Convert russian utf8 string to ASCII transliterating string
 * allocate new string
 */
static char * translit(const char * string);

/*
 * Convert string from one codepage to other
 * allocate new string
 */
static char * rucode_convert_codepage(const char * string, RUCODE_CODEPAGE from, RUCODE_CODEPAGE to);

/*
 * IMP
 */

typedef struct translit_t {
	uint32_t utf_code;
	char utf8[5];
	char ascii[4];
	char cp1251;
	char koi8r;
	char cp866;
	char iso8859_5;
} translit_t;

//for binary search we need sorted array
static translit_t translit_table[] = 
{
	{0x0401, "Ё", "YO", 168, 179, 240, 161  },
	{0x0410, "А", "A",  192, 225, 128, 176  },
	{0x0411, "Б", "B",  193, 226, 129, 177  },
	{0x0412, "В", "B",  194, 247, 130, 178  },
	{0x0412, "Г", "G",  195, 231, 131, 179  },
	{0x0413, "Д", "D",  196, 228, 132, 180  },
	{0x0414, "Е", "E",  197, 229, 133, 181  },
	{0x0415, "Ж", "ZH", 198, 246, 134, 182  },
	{0x0416, "З", "Z",  199, 250, 135, 183  },
	{0x0418, "И", "I",  200, 233, 136, 184  },
	{0x0419, "Й", "Y",  201, 234, 137, 185  },
	{0x041A, "К", "K",  202, 235, 138, 186  },
	{0x041B, "Л", "L",  203, 236, 139, 187  },
	{0x041C, "М", "M",  204, 237, 140, 188  },
	{0x041D, "Н", "N",  205, 238, 141, 189  },
	{0x041E, "О", "O",  206, 239, 142, 190  },
	{0x041F, "П", "P",  207, 240, 143, 191  },
	{0x0420, "Р", "R",  208, 242, 144, 192  },
	{0x0421, "С", "S",  209, 243, 145, 193  },
	{0x0422, "Т", "T",  210, 244, 146, 194  },
	{0x0423, "У", "U",  211, 245, 147, 195  },
	{0x0424, "Ф", "PH", 212, 230, 148, 196  },
	{0x0425, "Х", "KH", 213, 232, 149, 197  },
	{0x0426, "Ц", "TS", 214, 227, 150, 198  },
	{0x0427, "Ч", "TCH",215, 254, 151, 199  },
	{0x0428, "Ш", "SH", 216, 251, 152, 200  },
	{0x0429, "Щ", "SCH",217, 253, 153, 201  },
	{0x042A, "Ъ", "'",  218, 255, 154, 202  },
	{0x042B, "Ы", "Y",  219, 249, 155, 203  },
	{0x042C, "Ь", "'",  220, 248, 156, 204  },
	{0x042D, "Э", "E",  221, 252, 157, 205  },
	{0x042E, "Ю", "YU", 222, 224, 158, 206  },
	{0x042F, "Я", "YA", 223, 241, 159, 207  },
	{0x0430, "а", "a",  224, 193, 160, 208  },
	{0x0431, "б", "b",  225, 193, 161, 209  },
	{0x0432, "в", "v",  226, 215, 162, 210  },
	{0x0433, "г", "g",  227, 199, 163, 211  },
	{0x0434, "д", "d",  228, 196, 164, 212  },
	{0x0435, "е", "e",  229, 197, 165, 213  },
	{0x0436, "ж", "zh", 230, 214, 166, 214  },
	{0x0437, "з", "z",  231, 218, 167, 215  },
	{0x0438, "и", "i",  232, 201, 168, 216  },
	{0x0439, "й", "y",  233, 202, 169, 217  },
	{0x043A, "к", "k",  234, 203, 170, 218  },
	{0x043B, "л", "l",  235, 204, 171, 219  },
	{0x043C, "м", "m",  236, 205, 172, 220  },
	{0x043D, "н", "n",  237, 206, 173, 221  },
	{0x043E, "о", "o",  238, 207, 174, 222  },
	{0x043F, "п", "p",  239, 208, 175, 223  },
	{0x0440, "р", "r",  240, 201, 224, 224  },
	{0x0441, "с", "c",  241, 211, 225, 225  },
	{0x0442, "т", "t",  242, 212, 226, 226  },
	{0x0443, "у", "u",  243, 213, 227, 227  },
	{0x0444, "ф", "ph", 244, 198, 228, 228  },
	{0x0445, "х", "kh", 245, 200, 229, 229  },
	{0x0446, "ц", "ts", 246, 195, 230, 230  },
	{0x0447, "ч", "tch",247, 222, 231, 231  },
	{0x0448, "ш", "sh", 248, 219, 232, 232  },
	{0x0449, "щ", "sch",249, 221, 233, 233  },
	{0x044A, "ъ", "'",  250, 223, 234, 234  },
	{0x044B, "ы", "y",  251, 217, 235, 235  },
	{0x044C, "ь", "'",  252, 212, 236, 236  },
	{0x044D, "э", "e",  253, 220, 237, 237  },
	{0x044E, "ю", "yu", 254, 192, 238, 238  },
	{0x044F, "я", "ya", 255, 209, 239, 239  },
	{0x0451, "ё", "yo", 184, 163, 241, 241  },
	{0x2116, "№", "#",  185, '#', '#', '#'  },
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

void get_utf8(char *ptr, translit_t * value){
		int i, buflen = 0;
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
			buflen++;
		}	

		switch (buflen) {
			case 4: {
				value->utf_code = (
						((buf[0] & 0b00000111) << 18) |	   
						((buf[1] & 0b00111111) << 12) |	   
						((buf[2] & 0b00111111) << 6 ) |	   
						( buf[3] & 0b00111111)
						);		
				break;
			}
			case 3: {
				value->utf_code = (
						((buf[0] & 0b00001111) << 12) |	   
						((buf[1] & 0b00111111) << 6 ) |	   
						( buf[2] & 0b00111111)
						);		
				break;
			}					
			case 2: {
				value->utf_code = (
						((buf[0] & 0b00011111) << 6 ) |	   
						( buf[1] & 0b00111111)
						);		
				break;
			}					
			case 1: {
				value->utf_code = buf[0];		
				break;
			}					

			default:
				break;
		}		
}

char * 
rucode_convert_codepage(const char * str, RUCODE_CODEPAGE from, RUCODE_CODEPAGE to)
{
	if (from == to)
		return (char *)str;

	if (from == RUCODE_CODEPAGE_ASCII){
		printf("rucode: we can't use ASCII as source codepage yet\n");
		return NULL;
	}

	//get size of string
	int len = strlen(str);	

	//allocate buffer
	char * ret;
	if (to == RUCODE_CODEPAGE_UTF8 || to == RUCODE_CODEPAGE_ASCII){
		//allocate 3*len should be enough	
		ret = malloc(len*3 +1);
		if (!ret)
			return NULL;
	} else {
		//allocate len should be enough	
		ret = malloc(len +1);
		if (!ret)
			return NULL;
	}

	bool linear_search = false;
	//use linear search for koi8-r (do not sort array)
	if (to == RUCODE_CODEPAGE_KOI8R)
		linear_search = true;
	

	//pointer to string
	char *ptr = (char *)str; 

	//iterate string
	while(*ptr){
		translit_t value = {0};
		if (from == RUCODE_CODEPAGE_UTF8)
			get_utf8(ptr, &value);
		else{
			switch (from) {
				case RUCODE_CODEPAGE_CP1251:
					value.cp1251 = *ptr++;
					break;
				case RUCODE_CODEPAGE_KOI8R:
					value.koi8r = *ptr++;
					break;
				case RUCODE_CODEPAGE_CP866:
					value.cp866 = *ptr++;
					break;
				case RUCODE_CODEPAGE_ISO8859_5:
					value.iso8859_5 = *ptr++;
					break;
				
				default:
					break;
			}
			
		}

	
	}

}

#ifdef __cplusplus
}
#endif

#endif //k_lib_rucode_h__
