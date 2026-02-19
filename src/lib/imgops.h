#ifndef IMAGE_OPS_H
#define IMAGE_OPS_H
#include "imgio.h"


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

// PIXEL torgb(PIXEL* px);
// PIXEL togray(PIXEL* px);

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
 * @brief Applies a pixel operation to an entire image ( `op` takes in a PIXEL pointer, and does some operations on it)
 * 
 * @param op pointer towards pixel operation function
 * @param img image to do the operations on
 */
void apply_chan2img(void (*op)(PIXEL*), IMAGE* img);

/**
 * @brief Converts pixel from RGB color space to YUV
 * 
 * @param px the pixel to convert
 */
void rgb2yuv(PIXEL* px);

/**
 * @brief Converts pixel from RGB color space to YCbCr
 * 
 * @param px the pixel to convert
 */
void rgb2ycbcr(PIXEL* px);

/**
 * @brief Converts pixel from RGB colors space to Luma (read via GPIXEL type)
 * 
 * @param px the pixel to convert
 */
void rgb2luma(PIXEL* px);

/**
 * @brief Converts image from RGB color space to YUV
 * 
 * @param img Image to convert
 */
void rgb2yuvimg(IMAGE* img);

/**
 * @brief Converts image from RGB color space to YCbCr
 * 
 * @param img Image to convert
 */
void rgb2ycbcrimg(IMAGE* img);

/**
 * @brief Converts image from RGB color space to Luma ( => sets image type of `img` to PGM)
 * 
 * @param img Image to convert
 */
void rgb2lumaimg(IMAGE* img);



#endif