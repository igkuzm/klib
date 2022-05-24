/**
 * File              : pixel.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 20.05.2022
 * Last Modified Date: 24.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef k_lib_pixel_h__
#define k_lib_pixel_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdint.h>

typedef uint32_t pixel_t;

struct rgb
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct hsv
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
    unsigned char a;
};

static struct rgb	pixel2rgb(pixel_t pixel);
static pixel_t		rgb2pixel(struct rgb rgb);
static struct hsv	rgb2hsv(struct rgb rgb);
static struct rgb	hsv2rgb(struct hsv hsv);

struct rgb
pixel2rgb(
		pixel_t pixel
		)
{
	struct rgb rgb;
	
	rgb.r =  pixel        & 0xff;
	rgb.g = (pixel >> 8)  & 0xff;
	rgb.b = (pixel >> 16) & 0xff;
	rgb.a = (pixel >> 24) & 0xff;	

	return rgb;
}

pixel_t
rgb2pixel(
		struct rgb rgb
		)
{
	pixel_t pixel = 0;
	pixel += rgb.r;
	pixel += rgb.g << 8;
	pixel += rgb.b << 16;
	pixel += rgb.a << 24;

	return pixel;
}

struct rgb hsv2rgb(struct hsv hsv)
{
    struct rgb rgb;
	rgb.a = hsv.a;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

	return rgb;
}

struct hsv rgb2hsv(struct rgb rgb)
{
    struct hsv hsv;
	hsv.a = rgb.a;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * (rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

#ifdef __cplusplus
}
#endif

#endif //k_lib_pixel_h__
