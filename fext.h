/**
 * File              : fext.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 29.05.2022
 * Last Modified Date: 20.03.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * Get file extension
 */

#ifndef k_lib_fext_h_
#define k_lib_fext_h_

#ifdef __cplusplus
extern "C" {
#endif

	#include <string.h>

	static const char * fext(const char *filename) {
		const char *dot = strrchr(filename, '.');
		if (!dot || dot == filename)
			return "";
		return dot + 1;
	}

#ifdef __cplusplus
}
#endif

#endif // k_lib_fext_h_
