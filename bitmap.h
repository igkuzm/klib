/**
 * File              : bitmap.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 20.05.2022
 * Last Modified Date: 25.02.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * library to manage bitmap images
 */

#ifndef k_lib_pixel_h__
#define k_lib_pixel_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

// pixel is int32 type with (AA BB GG RR) content, each with value from 0 to 255
// (FF)
typedef uint32_t pixel_t;

struct rgb {
  unsigned char r; // red
  unsigned char g; // green
  unsigned char b; // blue
  unsigned char a; // alfa
};

struct hsv {
  unsigned char h; // hue
  unsigned char s; // saturation
  unsigned char v; // value
  unsigned char a; // alfa
};

static pixel_t rgb2pixel(struct rgb rgb); // convert pixel (int32) to rgb struct
static struct rgb pixel2rgb(pixel_t pixel); // convert rgb struct to pixel
static struct rgb hsv2rgb(struct hsv hsv); // convert hsv to rbg
static struct hsv rgb2hsv(struct rgb rgb); // convert rgb to hsv

static pixel_t *filter_median( // make median filter to image
    pixel_t *image, // pointer to image bitmap
    int w, int h, // image width, height
    int filterW, int filterH // filter width, height
    );

static pixel_t *image_convolution( // make filter with matrix
    pixel_t *image, // pointer to image bitmap
    int w, int h, // image width, height
    int matrixW, int matrixH, // matrix width, height
    double matrix[matrixW][matrixH], // filter matrix
    double factor, // multiplier to result
    double bias // add to result
    );

static pixel_t *filter_hsv( // make filter for hsv image represantation
    pixel_t *image, // pointer to image bitmap
    int w, int h, // image width, height
    void *user_data, // image data to path through filter
    int (*filter)( // filter function
        struct hsv *hsv, void *user_data));

// blur filter
#define filter_blur(image, w, h) \
({\
double ___matrix[3][3] = { 0.0, 0.2, 0.0, 0.2, 0.2, 0.2, 0.0, 0.2, 0.0, };\
image_convolution(image, w, h, 3, 3, ___matrix, 1.0, 0);\
})

#define filter_blur5(image, w, h) \
({\
	double ___matrix[5][5] = \
	{\
		0, 0, 1, 0, 0,\
		0, 1, 1, 1, 0,\
		1, 1, 1, 1, 1,\
		0, 1, 1, 1, 0,\
		0, 0, 1, 0, 0,\
	};\
	image_convolution(image, w, h, 3, 3, ___matrix, 1.0 / 13.0, 0);\
})

#define filter_gaussian_blur(image, w, h) \
({\
	double ___matrix[3][3] = \
	{\
		1, 2, 1,\
		2, 4, 2,\
		1, 2, 1,\
	};\
	image_convolution(image, w, h, 3, 3, ___matrix, 1.0 / 16.0, 0);\
})

#define filter_gaussian_blur5(image, w, h) \
({\
	double ___matrix[5][5] = \
	{\
		1,  4,  6,  4,  1,\
		4, 16, 24, 16,  4,\
		6, 24, 36, 24,  6,\
		4, 16, 24, 16,  4,\
		1,  4,  6,  4,  1,\
	};\
	image_convolution(image, w, h, 5, 5, ___matrix, 1.0 / 256.0, 0);\
})

#define filter_gaussian(image, w, h) \
({\
	double ___matrix[3][3] = \
	{\
		0.077847, 0.123317, 0.077847,\
		0.123317, 0.195346, 0.123317,\
		0.077847, 0.123317, 0.077847,\
	};\
	image_convolution(image, w, h, 3, 3, ___matrix, 1.0, 0);\
})

#define filter_motion_blur(image, w, h) \
({\
	double ___matrix[9][9] = \
	{\
		1, 0, 0, 0, 0, 0, 0, 0, 0,\
		0, 1, 0, 0, 0, 0, 0, 0, 0,\
		0, 0, 1, 0, 0, 0, 0, 0, 0,\
		0, 0, 0, 1, 0, 0, 0, 0, 0,\
		0, 0, 0, 0, 1, 0, 0, 0, 0,\
		0, 0, 0, 0, 0, 1, 0, 0, 0,\
		0, 0, 0, 0, 0, 0, 1, 0, 0,\
		0, 0, 0, 0, 0, 0, 0, 1, 0,\
		0, 0, 0, 0, 0, 0, 0, 0, 1,\
	};\
	image_convolution(image, w, h, 9, 9, ___matrix, 1.0 / 9.0, 0);\
})

// sharpen filter
#define filter_sharpen(image, w, h) \
({\
	double ___matrix[3][3] = \
	{\
		-1, -1, -1,\
		-1,  9, -1,\
		-1, -1, -1,\
	};\
	image_convolution(image, w, h, 3, 3, ___matrix, 1.0, 0);\
})

#define filter_sharpen5(image, w, h) \
({\
	double ___matrix[5][5] = \
	{\
		-1, -1, -1, -1, -1,\
		-1,  2,  2,  2, -1,\
		-1,  2,  8,  2, -1,\
		-1,  2,  2,  2, -1,\
		-1, -1, -1, -1, -1,\
	};\
	image_convolution(image, w, h, 5, 5, ___matrix, 1.0 / 8.0, 0);\
})

#define filter_sharpen_ex(image, w, h) \
({\
	double ___matrix[3][3] = \
	{\
		1,  1, 1,\
		1, -7, 1,\
		1,  1, 1,\
	};\
	image_convolution(image, w, h, 3, 3, ___matrix, 1.0, 0);\
})

#define filter_edges(image, w, h) \
({\
	double ___matrix[3][3] = \
	{\
		-1, -1, -1,\
		-1,  8, -1,\
		-1, -1, -1,\
	};\
	image_convolution(image, w, h, 3, 3, ___matrix, 1.0, 0);\
})

#define filter_edges5(image, w, h) \
({\
	double ___matrix[5][5] = \
	{\
		0,  0, -1,  0,  0,\
		0,  0, -1,  0,  0,\
		0,  0,  2,  0,  0,\
		0,  0,  0,  0,  0,\
		0,  0,  0,  0,  0,\
	};\
	image_convolution(image, w, h, 5, 5, ___matrix, 1.0, 0);\
})

// embos filter
#define filter_embos(image, w, h) \
({\
	double ___matrix[3][3] = \
	{\
		-1, -1,  0,\
		-1,  0,  1,\
		 0,  1,  1,\
	};\
	image_convolution(image, w, h, 3, 3, ___matrix, 1.0, 128.0);\
})

#define filter_embos5(image, w, h) \
({\
	double ___matrix[5][5] = \
	{\
		-1, -1, -1, -1,  0,\
		-1, -1, -1,  0,  1,\
		-1, -1,  0,  1,  1,\
		-1,  0,  1,  1,  1,\
		 0,  1,  1,  1,  1,\
	};\
	image_convolution(image, w, h, 5, 5, ___matrix, 1.0, 128.0);\
})

// mean filter
#define filter_mean(image, w, h) \
({\
	double ___matrix[3][3] = \
	{\
		 1,  1,  1,\
		 1,  1,  1,\
		 1,  1,  1,\
	};\
	image_convolution(image, w, h, 3, 3, ___matrix, 1.0 / 9.0, 0);\
})

pixel_t *
filter_hsv(
		pixel_t * image,
		int w, int h,
		void * user_data,
		int (*filter)(struct hsv * hsv, void * user_data)
		)
{
  pixel_t *out = (pixel_t *)malloc(w * h * sizeof(pixel_t));
  if (!out) {
    perror("output image malloc");
    exit(EXIT_FAILURE);
  }

  for (int x = 0; x < w; x++)
    for (int y = 0; y < h; y++) {
      pixel_t pixel = image[y * w + x];
      struct hsv hsv = rgb2hsv(pixel2rgb(pixel));

      // make magic
      if (filter) {
        int c = filter(&hsv, user_data);
        if (c) {
          free(out);
          return NULL;
        }
      }

      // convert back to rgb
      struct rgb rgb = hsv2rgb(hsv);

      // convert rgb to pixel
      pixel_t newpixel = rgb2pixel(rgb);

      out[y * w + x] = newpixel;
    }
  return out;
}

struct rgb pixel2rgb(pixel_t pixel) {
  struct rgb rgb = { .r = pixel & 0xff,
                     .g = (pixel >> 8) & 0xff,
                     .b = (pixel >> 16) & 0xff,
                     .a = (pixel >> 24) & 0xff };

  return rgb;
}

pixel_t rgb2pixel(struct rgb rgb) {
  pixel_t pixel = 0;

  pixel += rgb.r;
  pixel += rgb.g << 8;
  pixel += rgb.b << 16;
  pixel += rgb.a << 24;

  return pixel;
}

struct rgb hsv2rgb(struct hsv hsv) {
  struct rgb rgb;
  rgb.a = hsv.a;
  unsigned char region, remainder, p, q, t;

  if (hsv.s == 0) {
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

  switch (region) {
  case 0:
    rgb.r = hsv.v;
    rgb.g = t;
    rgb.b = p;
    break;
  case 1:
    rgb.r = q;
    rgb.g = hsv.v;
    rgb.b = p;
    break;
  case 2:
    rgb.r = p;
    rgb.g = hsv.v;
    rgb.b = t;
    break;
  case 3:
    rgb.r = p;
    rgb.g = q;
    rgb.b = hsv.v;
    break;
  case 4:
    rgb.r = t;
    rgb.g = p;
    rgb.b = hsv.v;
    break;
  default:
    rgb.r = hsv.v;
    rgb.g = p;
    rgb.b = q;
    break;
  }

  return rgb;
}

struct hsv rgb2hsv(struct rgb rgb) {
  struct hsv hsv;
  hsv.a = rgb.a;
  unsigned char rgbMin, rgbMax;

  rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b)
                         : (rgb.g < rgb.b ? rgb.g : rgb.b);
  rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b)
                         : (rgb.g > rgb.b ? rgb.g : rgb.b);

  hsv.v = rgbMax;
  if (hsv.v == 0) {
    hsv.h = 0;
    hsv.s = 0;
    return hsv;
  }

  hsv.s = 255 * (rgbMax - rgbMin) / hsv.v;
  if (hsv.s == 0) {
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

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

pixel_t *image_convolution(pixel_t *image, int w, int h, int matrixW,
                           int matrixH, double matrix[matrixW][matrixH],
                           double factor, double bias) {
  pixel_t *out = (pixel_t *)malloc(w * h * sizeof(pixel_t));
  if (!out) {
    perror("output image malloc");
    exit(EXIT_FAILURE);
  }

  // apply the filter
  for (int x = 0; x < w; x++)
    for (int y = 0; y < h; y++) {
      double red = 0.0, green = 0.0, blue = 0.0;
      unsigned char alfa = pixel2rgb(image[y * w + x]).a;

      // multiply every value of the filter with corresponding image pixel
      for (int filterY = 0; filterY < matrixH; filterY++)
        for (int filterX = 0; filterX < matrixW; filterX++) {
          int imageX = (x - matrixW / 2 + filterX + w) % w;
          int imageY = (y - matrixH / 2 + filterY + h) % h;

          struct rgb rgb = pixel2rgb(image[imageY * w + imageX]);

          red += rgb.r * matrix[filterY][filterX];
          green += rgb.g * matrix[filterY][filterX];
          blue += rgb.b * matrix[filterY][filterX];
        }

      // truncate values smaller than zero and larger than 255
      struct rgb rgb;

      rgb.r = MIN(MAX((int)(factor * red + bias), 0), 255);
      rgb.g = MIN(MAX((int)(factor * green + bias), 0), 255);
      rgb.b = MIN(MAX((int)(factor * blue + bias), 0), 255);

      rgb.a = alfa;

      out[y * w + x] = rgb2pixel(rgb);
    }
  return out;
}

#define SWAP(a, b) (a ^= b ^= a ^= b)

// selects the k-th largest element from the data between start and end (end
// exclusive)
int selectKth(int *data, int s, int e, int k) // in practice, use C++'s
                                              // nth_element, this is for
                                              // demonstration only
{
  // 5 or less elements: do a small insertion sort
  if (e - s <= 5) {
    for (int i = s + 1; i < e; i++)
      for (int j = i; j > 0 && data[j - 1] > data[j]; j--)
        SWAP(data[j], data[j - 1]);
    return s + k;
  }

  int p = (s + e) / 2; // choose simply center element as pivot

  // partition around pivot into smaller and larger elements
  SWAP(data[p], data[e - 1]); // temporarily move pivot to the end
  int j = s;                  // new pivot location to be calculated
  for (int i = s; i + 1 < e; i++)
    if (data[i] < data[e - 1]) {
      j++;
      SWAP(data[i], data[j]);
    }
  SWAP(data[j], data[e - 1]);

  // recurse into the applicable partition
  if (k == j - s)
    return s + k;
  else if (k < j - s)
    return selectKth(data, s, j, k);
  else
    return selectKth(data, j + 1, e, k - j + s - 1); // subtract amount of
                                                     // smaller elements from k
}

pixel_t *filter_median(pixel_t *image, int w, int h, int filterW, int filterH) {
  pixel_t *out = (pixel_t *)malloc(w * h * sizeof(pixel_t));
  if (!out) {
    perror("output image malloc");
    exit(EXIT_FAILURE);
  }

  int red[filterW * filterH];
  int green[filterW * filterH];
  int blue[filterW * filterH];

  // apply the filter
  for (int x = 0; x < w; x++)
    for (int y = 0; y < h; y++) {
      int n = 0;
      unsigned char alfa = pixel2rgb(image[y * w + x]).a;

      // multiply every value of the filter with corresponding image pixel
      for (int filterY = 0; filterY < filterH; filterY++)
        for (int filterX = 0; filterX < filterW; filterX++) {
          int imageX = (x - filterW / 2 + filterX + w) % w;
          int imageY = (y - filterH / 2 + filterY + h) % h;

          struct rgb rgb = pixel2rgb(image[imageY * w + imageX]);

          red[n] = rgb.r;
          green[n] = rgb.g;
          blue[n] = rgb.b;
        }

      struct rgb rgb;

      int filterSize = filterW * filterH;

      rgb.r = red[selectKth(red, 0, filterSize, filterSize / 2)];
      rgb.g = green[selectKth(green, 0, filterSize, filterSize / 2)];
      rgb.b = blue[selectKth(blue, 0, filterSize, filterSize / 2)];

      rgb.a = alfa;

      out[y * w + x] = rgb2pixel(rgb);
    }
  return out;
}

#ifdef __cplusplus
}
#endif

#endif // k_lib_pixel_h__
