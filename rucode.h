/**
 * File              : rucode.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 07.03.2022
 * Last Modified Date: 24.10.2022
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
	RUCODE_CODEPAGE_ISO8859_5,
	RUCODE_CODEPAGE_MACCYRILLIC,
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

typedef struct rucode_t {
	uint32_t utf_code;
	char utf8[5];
	char ascii[4];
	char cp1251;
	char koi8r;
	char cp866;
	char iso8859_5;
	char maccyrillic;
} rucode_t;

//for binary search we need sorted array
static rucode_t codepage_table[] = 
{
	{0x00AB, "«", "<<", 0xAB, '"', '"',  '"',  0xC7  }, //0
	{0x00BB, "»", ">>", 0xBB, '"', '"',  '"',  0xC8  }, //1
	{0x0401, "Ё", "YO", 168,  179, 240,  161,  0xDD  }, //2
	{0x0410, "А", "A",  192,  225, 128,  176,  0x80  }, //3
	{0x0411, "Б", "B",  193,  226, 129,  177,  0x81  }, //4
	{0x0412, "В", "B",  194,  247, 130,  178,  0x82  }, //5
	{0x0412, "Г", "G",  195,  231, 131,  179,  0x83  }, //6
	{0x0413, "Д", "D",  196,  228, 132,  180,  0x84  }, //7
	{0x0414, "Е", "E",  197,  229, 133,  181,  0x85  }, //8
	{0x0415, "Ж", "ZH", 198,  246, 134,  182,  0x86  }, //9
	{0x0416, "З", "Z",  199,  250, 135,  183,  0x87  }, //10
	{0x0418, "И", "I",  200,  233, 136,  184,  0x88  }, //11
	{0x0419, "Й", "Y",  201,  234, 137,  185,  0x89  }, //12
	{0x041A, "К", "K",  202,  235, 138,  186,  0x8A  }, //13
	{0x041B, "Л", "L",  203,  236, 139,  187,  0x8B  }, //14
	{0x041C, "М", "M",  204,  237, 140,  188,  0x8C  }, //15
	{0x041D, "Н", "N",  205,  238, 141,  189,  0x8D  }, //16
	{0x041E, "О", "O",  206,  239, 142,  190,  0x8E  }, //17
	{0x041F, "П", "P",  207,  240, 143,  191,  0x8F  }, //18
	{0x0420, "Р", "R",  208,  242, 144,  192,  0x90  }, //19
	{0x0421, "С", "S",  209,  243, 145,  193,  0x91  }, //20
	{0x0422, "Т", "T",  210,  244, 146,  194,  0x92  }, //21
	{0x0423, "У", "U",  211,  245, 147,  195,  0x93  }, //22
	{0x0424, "Ф", "PH", 212,  230, 148,  196,  0x94  }, //23
	{0x0425, "Х", "KH", 213,  232, 149,  197,  0x95  }, //24
	{0x0426, "Ц", "TS", 214,  227, 150,  198,  0x96  }, //25
	{0x0427, "Ч", "TCH",215,  254, 151,  199,  0x97  }, //26
	{0x0428, "Ш", "SH", 216,  251, 152,  200,  0x98  }, //27
	{0x0429, "Щ", "SCH",217,  253, 153,  201,  0x99  }, //28
	{0x042A, "Ъ", "'",  218,  255, 154,  202,  0x9A  }, //29
	{0x042B, "Ы", "Y",  219,  249, 155,  203,  0x9B  }, //30
	{0x042C, "Ь", "'",  220,  248, 156,  204,  0x9C  }, //31
	{0x042D, "Э", "E",  221,  252, 157,  205,  0x9D  }, //32
	{0x042E, "Ю", "YU", 222,  224, 158,  206,  0x9E  }, //33
	{0x042F, "Я", "YA", 223,  241, 159,  207,  0x9F  }, //34
	{0x0430, "а", "a",  224,  193, 160,  208,  0xE0  }, //35
	{0x0431, "б", "b",  225,  193, 161,  209,  0xE1  }, //36
	{0x0432, "в", "v",  226,  215, 162,  210,  0xE2  }, //37
	{0x0433, "г", "g",  227,  199, 163,  211,  0xE3  }, //38
	{0x0434, "д", "d",  228,  196, 164,  212,  0xE4  }, //39
	{0x0435, "е", "e",  229,  197, 165,  213,  0xE5  }, //40
	{0x0436, "ж", "zh", 230,  214, 166,  214,  0xE6  }, //41
	{0x0437, "з", "z",  231,  218, 167,  215,  0xE7  }, //42
	{0x0438, "и", "i",  232,  201, 168,  216,  0xE8  }, //43
	{0x0439, "й", "y",  233,  202, 169,  217,  0xE9  }, //44
	{0x043A, "к", "k",  234,  203, 170,  218,  0xEA  }, //45
	{0x043B, "л", "l",  235,  204, 171,  219,  0xEB  }, //46
	{0x043C, "м", "m",  236,  205, 172,  220,  0xEC  }, //47
	{0x043D, "н", "n",  237,  206, 173,  221,  0xED  }, //48
	{0x043E, "о", "o",  238,  207, 174,  222,  0xEE  }, //49
	{0x043F, "п", "p",  239,  208, 175,  223,  0xEF  }, //50
	{0x0440, "р", "r",  240,  201, 224,  224,  0xF0  }, //51
	{0x0441, "с", "c",  241,  211, 225,  225,  0xF1  }, //52
	{0x0442, "т", "t",  242,  212, 226,  226,  0xF2  }, //53
	{0x0443, "у", "u",  243,  213, 227,  227,  0xF3  }, //54
	{0x0444, "ф", "ph", 244,  198, 228,  228,  0xF4  }, //55
	{0x0445, "х", "kh", 245,  200, 229,  229,  0xF5  }, //56
	{0x0446, "ц", "ts", 246,  195, 230,  230,  0xF6  }, //57
	{0x0447, "ч", "tch",247,  222, 231,  231,  0xF7  }, //58
	{0x0448, "ш", "sh", 248,  219, 232,  232,  0xF8  }, //59
	{0x0449, "щ", "sch",249,  221, 233,  233,  0xF9  }, //60
	{0x044A, "ъ", "'",  250,  223, 234,  234,  0xFA  }, //61
	{0x044B, "ы", "y",  251,  217, 235,  235,  0xFB  }, //62
	{0x044C, "ь", "'",  252,  212, 236,  236,  0xFC  }, //63
	{0x044D, "э", "e",  253,  220, 237,  237,  0xFD  }, //64
	{0x044E, "ю", "yu", 254,  192, 238,  238,  0xFE  }, //65
	{0x044F, "я", "ya", 255,  209, 239,  239,  0xDF  }, //66
	{0x0451, "ё", "yo", 184,  163, 241,  241,  0xDE  }, //67
	{0x2116, "№", "#",  185,  '#', 0xFC, 0x0F, 0xDC  }, //68
};

#ifndef TRANSLIT_LINEAR_SEARCH
//function for binary search
static int translit_compare (const void * s1, const void * s2) {
    return ((rucode_t *)s1)->utf_code - ((rucode_t *)s2)->utf_code;
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
			sizeof(codepage_table) / sizeof(rucode_t);
	

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
		rucode_t key = {0};
		switch (buflen) {
			case 4: {
				key.utf_code = (
						((buf[0] & 0b00000111) << 18) |	   
						((buf[1] & 0b00111111) << 12) |	   
						((buf[2] & 0b00111111) << 6 ) |	   
						( buf[3] & 0b00111111)
						);		
				break;
			}
			case 3: {
				key.utf_code = (
						((buf[0] & 0b00001111) << 12) |	   
						((buf[1] & 0b00111111) << 6 ) |	   
						( buf[2] & 0b00111111)
						);		
				break;
			}					
			case 2: {
				key.utf_code = (
						((buf[0] & 0b00011111) << 6 ) |	   
						( buf[1] & 0b00111111)
						);		
				break;
			}					

			default:
				break;
		}
		if (key.utf_code) {
			rucode_t *translit = 
					bsearch(&key, codepage_table, 
							translit_table_size, sizeof(rucode_t), 
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

int get_utf8_code(char *ptr, rucode_t * key, char * buf){
	int i, buflen = 0;
	if ((*ptr & 0b11110000) == 240) {     //4 byte
		for (i = 0; i < 4; i++) {
			buf[i] = *ptr++;
			buflen++;
		} 
	} 
	else if ((*ptr & 0b11100000) == 224){ //3 byte
		for (i = 0; i < 3; i++) {
			buf[i] = *ptr++;
			buflen++;
		}
	}		  
	else if ((*ptr & 0b11000000) == 192){ //2 byte
		for (i = 0; i < 2; i++) {
			buf[i] = *ptr++;
			buflen++;
		}			
	}		  
	else {                                //1 byte - ANSY
		buf[0] = *ptr++;
		buflen++;
	}	

	switch (buflen) {
		case 4: {
			key->utf_code = (
					((buf[0] & 0b00000111) << 18) |	   
					((buf[1] & 0b00111111) << 12) |	   
					((buf[2] & 0b00111111) << 6 ) |	   
					( buf[3] & 0b00111111)
					);		
			break;
		}
		case 3: {
			key->utf_code = (
					((buf[0] & 0b00001111) << 12) |	   
					((buf[1] & 0b00111111) << 6 ) |	   
					( buf[2] & 0b00111111)
					);		
			break;
		}					
		case 2: {
			key->utf_code = (
					((buf[0] & 0b00011111) << 6 ) |	   
					( buf[1] & 0b00111111)
					);		
			break;
		}					
		case 1: {
			key->utf_code = buf[0];		
			break;
		}					

		default:
			break;
	}		
	return buflen;
}

//function for binary search
static int rucode_compare (const void * s1, const void * s2) {
	rucode_t * key = (rucode_t * )s2;
	if (key->utf_code)
		return ((rucode_t *)s1)->utf_code - key->utf_code;
	if (key->cp1251)
		return ((rucode_t *)s1)->cp1251 - key->cp1251;
	if (key->iso8859_5)
		return ((rucode_t *)s1)->iso8859_5 - key->iso8859_5;
	if (key->koi8r)
		return ((rucode_t *)s1)->koi8r - key->koi8r;	
	if (key->maccyrillic)
		return ((rucode_t *)s1)->maccyrillic - key->maccyrillic;	
	return 0;
}

void
sort_koi8r_table(const rucode_t * table, rucode_t * sorted_table, int table_size)
{
	rucode_t item;
	int i, j;
	//fill table
	for (i = 0; i < table_size; ++i) {
		sorted_table[i] = table[i];
	}
	//sort table
	for (i = 0; i < table_size; ++i) {
		for (j = 0; j < table_size; ++j) {
			if (sorted_table[i].koi8r > sorted_table[j].koi8r) {
				item = sorted_table[i];	
				sorted_table[i] = sorted_table[j];
				sorted_table[j] = item;
			}	
		}	
	}

}

char * 
rucode_convert_codepage(const char * str, RUCODE_CODEPAGE from, RUCODE_CODEPAGE to)
{
	int i;

	if (from == to)
		return (char *)str;

	if (from == RUCODE_CODEPAGE_ASCII){
		printf("rucode: we can't use ASCII as source codepage yet\n");
		return NULL;
	}

	//get size of string
	int len = strlen(str);	
	
	//get size of translit table
	int table_size = 
			sizeof(codepage_table) / sizeof(rucode_t);	

	//sort array
	rucode_t * sorted_table;
	rucode_t   sorted_table_content[sizeof(codepage_table)];
	switch (from) {
		case RUCODE_CODEPAGE_UTF8:
			sorted_table = codepage_table;
			break;		
		case RUCODE_CODEPAGE_CP1251: {
			sorted_table_content[0] = codepage_table[0];
			sorted_table_content[1] = codepage_table[65];
			sorted_table_content[2] = codepage_table[66];
			for (i = 3; i < table_size - 2; ++i) {
				sorted_table_content[i] = codepage_table[i-2];
			}
			sorted_table = sorted_table_content;
			break;
		}
		case RUCODE_CODEPAGE_KOI8R:
			sort_koi8r_table(codepage_table, sorted_table, table_size);
			sorted_table = sorted_table_content;
			break;
		case RUCODE_CODEPAGE_CP866:
			sorted_table_content[0] = codepage_table[66];
			for (i = 1; i < table_size - 3; ++i) {
				sorted_table_content[i] = codepage_table[i];
			}
			sorted_table_content[65] = codepage_table[0];
			sorted_table_content[66] = codepage_table[65];
			sorted_table = sorted_table_content;
			break;
		case RUCODE_CODEPAGE_ISO8859_5:
			sorted_table_content[0] = codepage_table[66];
			for (i = 1; i < table_size - 1; ++i) {
				sorted_table_content[i] = codepage_table[i];
			}
			sorted_table = sorted_table_content;
			break;
		
		default:
			break;
	}

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
	//pointer to return string
	char *rp  = (char *)ret; 

	//pointer to string
	char *ptr = (char *)str; 

	//iterate string
	while(*ptr){
		rucode_t key = {0};
		char buf[5];
		int buflen = 0;
		if (from == RUCODE_CODEPAGE_UTF8)
			buflen = get_utf8_code(ptr, &key, buf);
		else{
			buflen = 1;
			switch (from) {
				case RUCODE_CODEPAGE_CP1251:
					buf[0] = key.cp1251 = *ptr++;
					break;
				case RUCODE_CODEPAGE_KOI8R:
					buf[0] = key.koi8r = *ptr++;
					break;
				case RUCODE_CODEPAGE_CP866:
					buf[0] = key.cp866 = *ptr++;
					break;
				case RUCODE_CODEPAGE_ISO8859_5:
					buf[0] = key.iso8859_5 = *ptr++;
					break;
				case RUCODE_CODEPAGE_MACCYRILLIC:
					buf[0] = key.maccyrillic = *ptr++;
					break;					
				
				default:
					break;
			}
		}
		//make bsearch
		rucode_t *value = 
				bsearch(&key, sorted_table, 
						table_size, sizeof(rucode_t), 
								rucode_compare);
		if (value){
			switch (to) {
				case RUCODE_CODEPAGE_UTF8: {
					char * p = value->utf8;
					while (*p) {
						*rp++ = *p++;
					}
					break;						
				}
				case RUCODE_CODEPAGE_CP1251:
					*rp++ = value->cp1251;
					break;
				case RUCODE_CODEPAGE_CP866:
					*rp++ = value->cp866;
					break;
				case RUCODE_CODEPAGE_ISO8859_5:
					*rp++ = value->iso8859_5;
					break;
				case RUCODE_CODEPAGE_MACCYRILLIC:
					*rp++ = value->maccyrillic;
					break;					
				
				default:
					break;
			}
		} else {
			for (i = 0; i < buflen; ++i) {
				*rp++ = buf[i];	
			}
		}
	}
	return ret;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_rucode_h__
