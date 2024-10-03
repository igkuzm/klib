/**
 * File              : getstr.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.10.2024
 * Last Modified Date: 04.10.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * strrep.h
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

#ifndef GETSTR_H
#define GETSTR_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

static char *getstr(char *buffer, int len)
{
	int i = 0;
	for (i = 0; i < len-1; ++i) {
		scanf("%c", &buffer[i]);
		if (buffer[i] == '\n') {
			break;
		}
	}
	buffer[i] = '\0';
	return buffer;
}

#ifdef __cplusplus
}
#endif
#endif /* ifndef FGETSTR_H */