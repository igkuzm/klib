/**
 * File              : errcodes.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 27.04.2025
 * Last Modified Date: 27.04.2025
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
/**
 * errcodes.h
 * Copyright (c) 2025 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ERRCODES_H
#define ERRCODES_H

#include <stdio.h>

#ifndef ERRCODES
/* define error codes and it's description like 
 * ERRCODE(int, "description") under ERRCODES macro 
 * use function errcode(int) to get description */
#define ERRCODES \
	ERRCODE(0, "No error.") \
	ERRCODE(-1, "Can't allocate memory.") \

#endif /* ifndef ERRCODES */

/* IMPLIMATION */
#ifdef __cplusplus
extern "C" {
#endif

	struct errcode_s {
		const char *description;
		int code;
	};

	static const struct errcode_s errcodes_[] = {
		#define ERRCODE(c, d) d, c,
		ERRCODES
		#undef ERRCODE
		"Unknown error."
	};

	static const char *errcode(int code) {
		int i, count = sizeof(errcodes_)/sizeof(struct errcode_s);
		for (i = 0; i < count; ++i) {
			if (code == errcodes_[i].code)
				return errcodes_[i].description;
		}	
		return errcodes_[i].description;
	};


#ifdef __cplusplus
}
#endif

#endif /* ifndef ERRCODES_H */
