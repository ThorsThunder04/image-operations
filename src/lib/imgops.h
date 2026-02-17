#ifndef IMAGE_OPS_H
#define IMAGE_OPS_H
#include "imgio.h"

/**
 * @brief creates greyscale RGB pixel
 *
 * @param v: value between 0 and 255 that will be written to the 3 color channels
 * @returns an RGBPIXEL type
 */
RGBPIXEL rgb_from_one(BYTE v);

/**
 * @brief converts grayscale pixel to rgb pixel by copying the grayscale value to R, G, and B channels
 *
 * @param px: pointer to a GPIXEL object
 * @returns RGBPIXEL object
 */
RGBPIXEL g2rgb(GPIXEL* px);

/**
 * @brief converts rgb pixel to grayscale by storing floor(avg(R,G,B)) into a grayscale pixel
 *
 * @param px: pointer to an RGB pixel
 * @returns a GPIXEL object
 */
GPIXEL rgb2g(RGBPIXEL* px);

PIXEL torgb(PIXEL* px);
PIXEL togray(PIXEL* px);

#endif