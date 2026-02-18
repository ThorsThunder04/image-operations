#include <stdio.h>
#include <stdlib.h>
#include "imgio.h"
#include "imgops.h"



/**
 * @brief creates greyscale RGB pixel
 *
 * @param v: value between 0 and 255 that will be written to the 3 color channels
 * @returns an RGBPIXEL type
 */
RGBPIXEL rgb_from_one(BYTE v) {
    return (RGBPIXEL){v, v, v};
}

/**
 * @brief converts grayscale pixel to rgb pixel by copying the grayscale value to R, G, and B channels
 *
 * @param px: pointer to a GPIXEL object
 * @returns RGBPIXEL object
 */
RGBPIXEL g2rgb(GPIXEL* px) {
    return rgb_from_one(px->v);
}

/**
 * @brief converts rgb pixel to grayscale by storing floor(avg(R,G,B)) into a grayscale pixel
 *
 * @param px: pointer to an RGB pixel
 * @returns a GPIXEL object
 */
GPIXEL rgb2g(RGBPIXEL* px) {
    GPIXEL gpx = {0};

    // calculates floored average of px
    gpx.v += px->r/3;
    gpx.v += px->g/3;
    gpx.v += px->b/3;

    return gpx;

}

/**
 * @brief gets a PIXEL type at position (r,c) in the image
 * @brief NOTE: position (0,0) represent the top left corner of the image
 *
 * @param r: row in the image
 * @param c: column in the image
 * @param img: pointer towards image matrix
 *
 * @returns PIXEL* type pointer | NULL if (r,c) is out of bounds
 */
PIXEL* get_pixel(int r, int c, IMAGE* img); // TODO Implement

/**
 * @brief Sets a PIXEL type at position (r,c) in the image
 * @brief NOTE: position (0,0) represents the top left corner of the image
 *
 * @param r: row in the image
 * @param c: column in the image
 * @param px: pointer to a PIXEL type
 * @param img: pointer to an IMAGE type
 *
 * @returns pointer to placed pixel | NULL if (r,c) out of bounds
 */
PIXEL* set_pixel(int r, int c, PIXEL* px, IMAGE* img); // TODO Implement