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
	{0x00AB, "«", "<<", 0xAB, 0x22,  34,  0x22,  0xC7  }, //0
	{0x00BB, "»", ">>", 0xBB, 0x22,  34,  0x22,  0xC8  }, //1
	{0x0401, "Ё", "YO", 0xA8, 0xB3, 240,  0xA1,  0xDD  }, //2
	{0x0410, "А", "A",  0xC0, 0xE1, 128,  0xB0,  0x80  }, //3
	{0x0411, "Б", "B",  0xC1, 0xE2, 129,  0xB1,  0x81  }, //4
	{0x0412, "В", "B",  0xC2, 0xF7, 130,  0xB2,  0x82  }, //5
	{0x0412, "Г", "G",  0xC3, 0xE7, 131,  0xB3,  0x83  }, //6
	{0x0413, "Д", "D",  0xC4, 0xE4, 132,  0xB4,  0x84  }, //7
	{0x0414, "Е", "E",  0xC5, 0xE5, 133,  0xB5,  0x85  }, //8
	{0x0415, "Ж", "ZH", 0xC6, 0xF6, 134,  0xB6,  0x86  }, //9
	{0x0416, "З", "Z",  0xC7, 0xFA, 135,  0xB7,  0x87  }, //10
	{0x0418, "И", "I",  0xC8, 0xE9, 136,  0xB8,  0x88  }, //11
	{0x0419, "Й", "Y",  0xC9, 0xEA, 137,  0xB9,  0x89  }, //12
	{0x041A, "К", "K",  0xCA, 0xEB, 138,  0xBA,  0x8A  }, //13
	{0x041B, "Л", "L",  0xCB, 0xEC, 139,  0xBB,  0x8B  }, //14
	{0x041C, "М", "M",  0xCC, 0xED, 140,  0xBC,  0x8C  }, //15
	{0x041D, "Н", "N",  0xCD, 0xEE, 141,  0xBD,  0x8D  }, //16
	{0x041E, "О", "O",  0xCE, 0xEF, 142,  0xBE,  0x8E  }, //17
	{0x041F, "П", "P",  0xCF, 0xF0, 143,  0xBF,  0x8F  }, //18
	{0x0420, "Р", "R",  0xD0, 0xF2, 144,  0xC0,  0x90  }, //19
	{0x0421, "С", "S",  0xD1, 0xF3, 145,  0xC1,  0x91  }, //20
	{0x0422, "Т", "T",  0xD2, 0xF4, 146,  0xC2,  0x92  }, //21
	{0x0423, "У", "U",  0xD3, 0xF5, 147,  0xC3,  0x93  }, //22
	{0x0424, "Ф", "PH", 0xD4, 0xE6, 148,  0xC4,  0x94  }, //23
	{0x0425, "Х", "KH", 0xD5, 0xE8, 149,  0xC5,  0x95  }, //24
	{0x0426, "Ц", "TS", 0xD6, 0xE3, 150,  0xC6,  0x96  }, //25
	{0x0427, "Ч", "TCH",0xD7, 0xFE, 151,  0xC7,  0x97  }, //26
	{0x0428, "Ш", "SH", 0xD8, 0xFB, 152,  0xC8,  0x98  }, //27
	{0x0429, "Щ", "SCH",0xD9, 0xFD, 153,  0xC9,  0x99  }, //28
	{0x042A, "Ъ", "'",  0xDA, 0xFF, 154,  0xCA,  0x9A  }, //29
	{0x042B, "Ы", "Y",  0xDB, 0xF9, 155,  0xCB,  0x9B  }, //30
	{0x042C, "Ь", "'",  0xDC, 0xF8, 156,  0xCC,  0x9C  }, //31
	{0x042D, "Э", "E",  0xDD, 0xFC, 157,  0xCD,  0x9D  }, //32
	{0x042E, "Ю", "YU", 0xDE, 0xE0, 158,  0xCE,  0x9E  }, //33
	{0x042F, "Я", "YA", 0xDF, 0xF1, 159,  0xCF,  0x9F  }, //34
	{0x0430, "а", "a",  0xE0, 0xC1, 160,  0xD0,  0xE0  }, //35
	{0x0431, "б", "b",  0xE1, 0xC2, 161,  0xD1,  0xE1  }, //36
	{0x0432, "в", "v",  0xE2, 0xD7, 162,  0xD2,  0xE2  }, //37
	{0x0433, "г", "g",  0xE3, 0xC7, 163,  0xD3,  0xE3  }, //38
	{0x0434, "д", "d",  0xE4, 0xC4, 164,  0xD4,  0xE4  }, //39
	{0x0435, "е", "e",  0xE5, 0xC5, 165,  0xD5,  0xE5  }, //40
	{0x0436, "ж", "zh", 0xE6, 0xD6, 166,  0xD6,  0xE6  }, //41
	{0x0437, "з", "z",  0xE7, 0xDA, 167,  0xD7,  0xE7  }, //42
	{0x0438, "и", "i",  0xE8, 0xC9, 168,  0xD8,  0xE8  }, //43
	{0x0439, "й", "y",  0xE9, 0xCA, 169,  0xD9,  0xE9  }, //44
	{0x043A, "к", "k",  0xEA, 0xCB, 170,  0xDA,  0xEA  }, //45
	{0x043B, "л", "l",  0xEB, 0xCC, 171,  0xDB,  0xEB  }, //46
	{0x043C, "м", "m",  0xEC, 0xCD, 172,  0xDC,  0xEC  }, //47
	{0x043D, "н", "n",  0xED, 0xCE, 173,  0xDD,  0xED  }, //48
	{0x043E, "о", "o",  0xEE, 0xCF, 174,  0xDE,  0xEE  }, //49
	{0x043F, "п", "p",  0xEF, 0xD0, 175,  0xDF,  0xEF  }, //50
	{0x0440, "р", "r",  0xF0, 0xD2, 224,  0xE0,  0xF0  }, //51
	{0x0441, "с", "c",  0xF1, 0xD3, 225,  0xE1,  0xF1  }, //52
	{0x0442, "т", "t",  0xF2, 0xD4, 226,  0xE2,  0xF2  }, //53
	{0x0443, "у", "u",  0xF3, 0xD5, 227,  0xE3,  0xF3  }, //54
	{0x0444, "ф", "ph", 0xF4, 0xC6, 228,  0xE4,  0xF4  }, //55
	{0x0445, "х", "kh", 0xF5, 0xC8, 229,  0xE5,  0xF5  }, //56
	{0x0446, "ц", "ts", 0xF6, 0xC3, 230,  0xE6,  0xF6  }, //57
	{0x0447, "ч", "tch",0xF7, 0xDE, 231,  0xE7,  0xF7  }, //58
	{0x0448, "ш", "sh", 0xF8, 0xDB, 232,  0xE8,  0xF8  }, //59
	{0x0449, "щ", "sch",0xF9, 0xDD, 233,  0xE9,  0xF9  }, //60
	{0x044A, "ъ", "'",  0xFA, 0xDF, 234,  0xEA,  0xFA  }, //61
	{0x044B, "ы", "y",  0xFB, 0xD9, 235,  0xEB,  0xFB  }, //62
	{0x044C, "ь", "'",  0xFC, 0xD8, 236,  0xEC,  0xFC  }, //63
	{0x044D, "э", "e",  0xFD, 0xDC, 237,  0xED,  0xFD  }, //64
	{0x044E, "ю", "yu", 0xFE, 0xC0, 238,  0xEE,  0xFE  }, //65
	{0x044F, "я", "ya", 0xFF, 0xD1, 239,  0xEF,  0xDF  }, //66
	{0x0451, "ё", "yo", 0xB8, 0xA3, 241,  0xF2,  0xDE  }, //67
	{0x2116, "№", "#",  0xB9, 0x23, 252,  0xF0,  0xDC  }, //68
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

void
sort_maccyrillic_table(const rucode_t * table, rucode_t * sorted_table, int table_size)
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
			if (sorted_table[i].maccyrillic > sorted_table[j].maccyrillic) {
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
			sorted_table_content[0] = codepage_table[2];
			sorted_table_content[1] = codepage_table[0];
			sorted_table_content[2] = codepage_table[1];
			sorted_table_content[3] = codepage_table[67];
			sorted_table_content[4] = codepage_table[68];
			for (i = 5; i < table_size - 2; ++i) {
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
			sorted_table_content[0] = codepage_table[0];
			sorted_table_content[1] = codepage_table[1];
			for (i = 3; i < table_size - 2; ++i) {
				sorted_table_content[i-1] = codepage_table[i];
			}
			sorted_table_content[66] = codepage_table[2];
			sorted_table_content[67] = codepage_table[67];
			sorted_table_content[68] = codepage_table[68];
			sorted_table = sorted_table_content;
			break;

		case RUCODE_CODEPAGE_ISO8859_5:
			sorted_table_content[0] = codepage_table[0];
			sorted_table_content[1] = codepage_table[1];
			for (i = 2; i < table_size - 2; ++i) {
				sorted_table_content[i] = codepage_table[i];
			}
			sorted_table_content[67] = codepage_table[68];
			sorted_table_content[68] = codepage_table[67];
			sorted_table = sorted_table_content;
			break;

		case RUCODE_CODEPAGE_MACCYRILLIC:
			sort_maccyrillic_table(codepage_table, sorted_table, table_size);
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
