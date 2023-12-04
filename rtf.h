/**
 * File              : rtf.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.12.2023
 * Last Modified Date: 04.12.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/* functions to handle RTF files */

#ifndef RTF_H_
#define RTF_H_

#include "str.h"

/* rtf_from_utf8
 * return string with rtf code from utf8 multibite 
 * sting or NULL on error
 */
static char *
rtf_from_utf8(const char *s);

/* rtf_table_header
 * return string with rtf code of table header
 * or NULL on error
 * %coln   - number of columns
 * %titles - array of columns titles
 * %width  - array of columns width
 */
static char *
rtf_table_header(int coln, const char *titles[], int *width);

/* rtf_table_header
 * return string with rtf code of table row
 * or NULL on error
 * %coln  - number of columns
 * %colv  - columns values
 */
static char *
rtf_table_row(int coln, char *colv[]);


/* IMPLIMATION */
#include <stdint.h>

char *
rtf_from_utf8(const char *s)
{
	if (!s)
		return NULL;
	
	size_t len = strlen(s);
	char *out = (char *)malloc(len * 6 + 1);
	if (!out)
		return NULL;
	strcpy(out, "");

	char *ptr = (char *)s;
	uint32_t c32;
	while(*ptr){
		// get char
		uint8_t c = *ptr;
		if (c >= 252){/* 6-bytes */
			c32  = (*ptr++ & 0x1)  << 30;  // 0b00000001
			c32 |= (*ptr++ & 0x3F) << 24;  // 0b00111111
			c32 |= (*ptr++ & 0x3F) << 18;  // 0b00111111
			c32 |= (*ptr++ & 0x3F) << 12;  // 0b00111111
			c32 |= (*ptr++ & 0x3F) << 6;   // 0b00111111
			c32 |=  *ptr++ & 0x3F;         // 0b00111111
			sprintf(out, "%s\\u%d ", out, c32);
		}	
		else if (c >= 248){/* 5-bytes */
			c32  = (*ptr++ & 0x3)  << 24;  // 0b00000011
			c32 |= (*ptr++ & 0x3F) << 18;  // 0b00111111
			c32 |= (*ptr++ & 0x3F) << 12;  // 0b00111111
			c32 |= (*ptr++ & 0x3F) << 6;   // 0b00111111
			c32 |=  *ptr++ & 0x3F;         // 0b00111111
			sprintf(out, "%s\\u%d ", out, c32);
		}
		else if (c >= 240){/* 4-bytes */
			c32  = (*ptr++ & 0x7)  << 18;  // 0b00000111
			c32 |= (*ptr++ & 0x3F) << 12;  // 0b00111111
			c32 |= (*ptr++ & 0x3F) << 6;   // 0b00111111
			c32 |=  *ptr++ & 0x3F ;        // 0b00111111
			sprintf(out, "%s\\u%d ", out, c32);
		}
		else if (c >= 224){/* 3-bytes */
			c32  = (*ptr++ & 0xF)  << 12;  // 0b00001111
			c32 |= (*ptr++ & 0x3F) << 6;   // 0b00111111
			c32 |=  *ptr++ & 0x3F;         // 0b00111111
			sprintf(out, "%s\\u%d ", out, c32);
		}
		else if (c >= 192){/* 2-bytes */
			c32  = (*ptr++ & 0x1F) << 6;   // 0b00011111
			c32 |=  *ptr++ & 0x3F;         // 0b00111111
			sprintf(out, "%s\\u%d ", out, c32);
		}
		else{/* 1-byte */
			sprintf(out, "%s%c", out, *ptr++);
		}
	}
	strcat(out, " ");
	return out;
}

char *
rtf_table_header(
		int coln, const char *titles[], int *width)
{
	int i;
	struct str s;
	if (str_init(&s))
		return NULL;
	
	str_cat(&s, 
			  "\\pard\\par\\trowd\n");
	for (i = 0; i < coln; ++i) {
		str_catf(&s, 
				"\\clbrdrt\\brdrs"
				"\\clbrdrl\\brdrs"
				"\\clbrdrb\\brdrs"
				"\\clbrdrr\\brdrs\n"
				"\\cellx%d\n", 
				width[i]);
	}
	for (i = 0; i < coln; ++i) {
		char *title = rtf_from_utf8(titles[i]);
		str_catf(&s, 
				"\\intbl %s \\cell\n", 
				title);
		free(title);
	}

	return s.str;
}

char *
rtf_table_row(
		int coln, char *colv[])
{
	int i;
	struct str s;
	if (str_init(&s))
		return NULL;

	str_cat(&s, 
				"\\trowd\n");
	
	for (i = 0; i < coln; ++i)
		str_catf(&s, 
				"\\intbl %s \\cell\n",
				rtf_from_utf8(colv[i]));
	
	str_cat(&s, 
				"\\row\n");
	
	return s.str;
}
#endif /* ifndef RTF_H_ */
