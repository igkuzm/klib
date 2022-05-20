/**
 * File              : rgb2hsv.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 20.05.2022
 * Last Modified Date: 20.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#ifndef k_lib_rgb2hsv_h__
#define k_lib_rgb2hsv_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <math.h>

typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

#ifdef __cplusplus
}
#endif

#endif //k_lib_rgb2hsv_h__
