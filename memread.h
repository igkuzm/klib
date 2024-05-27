/**
 * File              : memread.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 28.05.2024
 * Last Modified Date: 27.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * memread.h
 * Copyright (c) 2024 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* memread - FILE-like functions to read from memory buffer */

#ifndef MEMREAD_H
#define MEMREAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct MEM {
	unsigned char *buffer; // memory buffeR 
	int size;              // size of buffer
	int p;                 // position of pointer buffer 
} MEM;

/* create memory stream */
static MEM *memopen(void *buffer, int size){
	MEM *mem = (MEM *)malloc(sizeof(MEM));
	if (!mem)
		return NULL;
	mem->size = size;
	mem->p = 0;
	mem->buffer = (unsigned char *)buffer;
	return mem;
};

static void memclose(MEM *mem){
	free(mem);
}

/* read n elements of size from buffer pointer to ptr 
 * and return readed bytes */
static int memread(
		void *__restrict ptr, size_t size, size_t n, 
		MEM *__restrict mem)
{
	if (!mem || !mem->buffer)
		return 0;
	int len = n * size;
	if (len + mem->p > mem->size)
		len = mem->size - mem->p;
	unsigned char *p = &(mem->buffer[mem->p]);
	memcpy(ptr, mem->buffer, len);
	mem->p += len;
	return len;
}

#ifdef __cplusplus
}
#endif

#endif // MEMREAD_H
