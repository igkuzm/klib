/**
 * File              : nargs.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 23.01.2026
 * Last Modified Date: 23.01.2026
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
/**
 * nargs.h
 * Copyright (c) 2026 Igor V. Sementsov <ig.kuzm@gmail.com>
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

#ifndef NARGS
#include <stdio.h>
#define NARGS(...) NARGS_(,##__VA_ARGS__, 2, 1, 0)
#define NARGS_(a, b, c, cnt, ...) cnt
#endif /* ifndef NARGS */
