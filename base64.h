/**
 * File              : base64.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 05.04.2022
 * Last Modified Date: 05.04.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

#ifndef k_base64_h__
#define k_base64_h__

#ifdef __cplusplus
extern "C"{
#endif

char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);

#ifdef __cplusplus
}
#endif

#endif //k_base64_h__



