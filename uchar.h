/**
 * File              : uchar.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 14.06.2023
 * Last Modified Date: 15.06.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef UCHAR_H
#define UCHAR_H

#include <stddef.h>
#include <stdint.h>

/* convert utf32 char to utf8 multybite char array and return number of bytes */ 
static int c32tomb(char s[6], const uint32_t c32){
	int i = 0;
	if (c32 <= 0x7F) {
		// Plain single-byte ASCII.
		s[i++] = (char) c32;
	}
	else if (c32 <= 0x7FF) {
		// Two bytes.
		s[i++] = 0xC0 |  (c32 >> 6);
		s[i++] = 0x80 | ((c32 >> 0) & 0x3F);
	}
	else if (c32 <= 0xFFFF) {
		// Three bytes.
		s[i++] = 0xE0 |  (c32 >> 12);
		s[i++] = 0x80 | ((c32 >> 6) & 0x3F);
		s[i++] = 0x80 | ((c32 >> 0) & 0x3F);
	}
	else if (c32 <= 0x1FFFFF) {
		// Four bytes.
		s[i++] = 0xF0 |  (c32 >> 18);
		s[i++] = 0x80 | ((c32 >> 12) & 0x3F);
		s[i++] = 0x80 | ((c32 >> 6)  & 0x3F);
		s[i++] = 0x80 | ((c32 >> 0)  & 0x3F);
	}
	else if (c32 <= 0x3FFFFFF) {
		// Five bytes.
		s[i++] = 0xF8 |  (c32 >> 24);
		s[i++] = 0x80 | ((c32 >> 18) & 0x3F);
		s[i++] = 0x80 | ((c32 >> 12) & 0x3F);
		s[i++] = 0x80 | ((c32 >> 6)  & 0x3F);
		s[i++] = 0x80 | ((c32 >> 0)  & 0x3F);
	}
	else if (c32 <= 0x7FFFFFFF) {
		// Six bytes.
		s[i++] = 0xFC |  (c32 >> 30);
		s[i++] = 0x80 | ((c32 >> 24) & 0x3F);
		s[i++] = 0x80 | ((c32 >> 18) & 0x3F);
		s[i++] = 0x80 | ((c32 >> 12) & 0x3F);
		s[i++] = 0x80 | ((c32 >> 6)  & 0x3F);
		s[i++] = 0x80 | ((c32 >> 0)  & 0x3F);
	}
	else{
		// Invalid char; don't encode anything.
	}	

	return i;
}

/* convert utf8 multybite null-terminated string to utf32 null-terminated string 
 * and return it's len */ 
static size_t mbtoc32(uint32_t *s32, const char *s){
	char *ptr = (char *)s;
	size_t i = 0;
	while (*ptr){
		uint8_t c = *ptr;
		if (c >= 252){/* 6-bytes */
			s32[i] |= (*ptr++ & 0x1)  << 30;  // 0b00000001
			s32[i] |= (*ptr++ & 0x3F) << 24;  // 0b00111111	
			s32[i] |= (*ptr++ & 0x3F) << 18;  // 0b00111111
			s32[i] |= (*ptr++ & 0x3F) << 12;  // 0b00111111
			s32[i] |= (*ptr++ & 0x3F) << 6;   // 0b00111111
			s32[i] |=  *ptr++ & 0x3F;         // 0b00111111
			i++;
		} 
		else if (c >= 248){/* 5-bytes */
			s32[i] |= (*ptr++ & 0x3)  << 24;  // 0b00000011
			s32[i] |= (*ptr++ & 0x3F) << 18;  // 0b00111111
			s32[i] |= (*ptr++ & 0x3F) << 12;  // 0b00111111
			s32[i] |= (*ptr++ & 0x3F) << 6;   // 0b00111111
			s32[i] |=  *ptr++ & 0x3F;         // 0b00111111
			i++;
		}
		else if (c >= 240){/* 4-bytes */
			s32[i] |= (*ptr++ & 0x7)  << 18;  // 0b00000111
			s32[i] |= (*ptr++ & 0x3F) << 12;  // 0b00111111
			s32[i] |= (*ptr++ & 0x3F) << 6;   // 0b00111111
			s32[i] |=  *ptr++ & 0x3F;         // 0b00111111
			i++;
		} 
		else if (c >= 224){/* 3-bytes */
			s32[i] |= (*ptr++ & 0xF)  << 12;  // 0b00001111
			s32[i] |= (*ptr++ & 0x3F) << 6;   // 0b00111111
			s32[i] |=  *ptr++ & 0x3F;         // 0b00111111
			i++;                
		}
		else if (c >= 192){/* 2-bytes */
			s32[i] |= (*ptr++ & 0x1F) << 6;   // 0b00011111
			s32[i] |=  *ptr++ & 0x3F;         // 0b00111111 
			i++; 
		} 
		else{/* 1-byte */
			s32[i++] = *ptr++;
		} 
	}

	// null-terminate string
	s32[i] = 0;
	return i;
}

#endif /* ifndef UCHAR_H */
