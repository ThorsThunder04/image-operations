#ifndef IMAGE_OPS_H
#define IMAGE_OPS_H
#include "imgio.h"

// no conversions from gray to anything else (as there's only gray)
typedef enum {
    // RGB source conversions
    RGB2GRAY = 0,
    RGB2YUV,
    RGB2YCBCR,
    RGB2HSV,

    // YUV source conversions
    YUV2RGB,
    YUV2GRAY,
    YUV2YCBCR,
    YUV2HSV,

    // YCBCR source conversions
    YCBCR2RGB,
    YCBCR2YUV,
    YCBCR2GRAY,
    YCBCR2HSV,

    // HSV source conversions
    HSV2RGB,
    HSV2GRAY,
    HSV2YCBCR,
    HSV2YUV,

    _CONVEND
} CONVTYPE;

/**
 * @brief returns wether (r,c) is within the bounds of img
 *
 * @param r row in image
 * @param c column in image
 * @param img image we're verifying the bounds of
 * @return int 1 if (r,c) is within bounds, else 0
 */
int is_ib(int r, int c, IMAGE* img);


/**
 * @brief converts grayscale pixel to rgb pixel by copying the grayscale value to R, G, and B channels
 *
 * @param px: pointer to a GPIXEL object
 * @returns an RGBPIXEL type
 */
RGBPIXEL g2rgb(GPIXEL* px);

/**
 * @brief converts rgb pixel to grayscale by storing floor(avg(R,G,B)) into a grayscale pixel
 *
 * @param px: pointer to an RGB pixel
 *
 * @returns an GPIXEL type
 */
GPIXEL rgb2g(RGBPIXEL* px);

/**
 * @brief gets a PIXEL* type at position (r,c) in an IMAGE
 * @brief NOTE: position (0,0) represent the top left corner of the image
 *
 * @param r row in the image
 * @param c column in the image
 * @param img pointer towards image matrix
 *
 * @returns PIXEL* type | NULL if (r,c) is out of bounds
 */
PIXEL* get_pixel(int r, int c, IMAGE* img);


/**
 * @brief gits the RGBPIXEL* type at position (r,c) from a given PIXEL in an IMAGE
 * @brief NOTE: position (0,0) represent the top left corner of the image
 *
 * @param r row in image
 * @param c column in image
 * @param img pointer to an IMAGE type
 * @return RGBPIXEL* type | NULL if (r,c) is out of bounds
 */
RGBPIXEL* get_rgbpixel(int r, int c, IMAGE* img);

/**
 * @brief gits the GPIXEL* type at position (r,c) from a given PIXEL in an IMAGE
 * @brief NOTE: position (0,0) represent the top left corner of the image
 *
 * @param r row in image
 * @param c column in image
 * @param img pointer to an IMAGE type
 * @return GPIXEL* type | NULL if (r,c) is out of bounds
 */
GPIXEL* get_gpixel(int r, int c, IMAGE* img);


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
PIXEL* set_pixel(int r, int c, PIXEL* px, IMAGE* img);

/**
 * @brief Sets the RGB value of an RGBPIXEL
 *
 * @param px pointer towards an RGBPIXEL type
 * @param R red value
 * @param G green value
 * @param B blue value
 * @return RGBPIXEL* towards pixel that was written to | NULL if (r,c) out of bounds
 */
void set_rgbpixel(RGBPIXEL* px, BYTE R, BYTE G, BYTE B);

/**
 * @brief Sets the gray value of a GPIXEL
 *
 * @param px pointer towards a GPIXEL
 * @param V gray value
 */
void set_gpixel(GPIXEL* px, BYTE V);


/**
 * @brief Converts the pixel from `srcpx` to a different color space specified by `conv` and writes the converted pixel values in `destpx` (can be same as `srxpx`)
 * 
 * @param destpx pixel to write converted values to
 * @param srcpx pixel to convert the values from
 * @param conv colorspace conversion type (ex: RGB2HSV)
 */
void convert_pxchan(PIXEL* destpx, PIXEL* srcpx, CONVTYPE conv);


/**
 * @brief Converts the pixels from `srcimg` to a different color space specified by `conv` and writes the convereted pixels to `destimg` (can be same as `srcimg`)
 * 
 * @param destimg image to write all the converted pixels to
 * @param srcimg image to convert the pixels from
 * @param conv colorspace conversion type (ex: `RGB2HSV`)
 * 
 * @returns `int` `0` if success. `-1` if `conv` is a non existant conversion type. `-2` if out of bounds error on `destimg` (at least one of `srcimg`'s dimensions are bigger than those of `destimg`'s).
 */
int convert_imgchan(IMAGE* destimg, IMAGE* srcimg, CONVTYPE conv);

/**
 * @brief Converts the pixels from `srcimg` to a different color space specified by `conv` within a given rectangular range, then writes the converted pixels to `destimg`
 * 
 * @param destimg image to write the converted pixels to
 * @param srcimg  image to convert the pixels from
 * @param conv colorspace conversion type (ex: RGB2HSV)
 * @param r1 start row
 * @param c1 start column
 * @param r2 end row (inclusive)
 * @param c2 end column (inclusive)
 * 
 * @returns `int` `0` if success. `-1` if `conv` is a non existant conversion type. `-2` if out of bounds error on `destimg`. `-3` if out of bounds error on `srcimg`
 */
int convert_imgchanrange(IMAGE* destimg, IMAGE* srcimg, CONVTYPE conv, int r1, int c1, int r2, int c2);



#endif
