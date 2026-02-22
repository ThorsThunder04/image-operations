#ifndef IMAGE_OPS_H
#define IMAGE_OPS_H
#include "imgio.h"

// no conversions from gray to anything else (as there's only gray)
enum _channel_conversion_type_enum {
    
    // MISC conversions
    GRAY2RGB = 0, // this just places the R channel across all 3 channels
    RED2RGB, // same as GRAY2RGB
    GREEN2RGB, // places G across all 3 channels
    BLUE2RGB, // same as prev but for B

    // RGB source conversions
    RGB2GRAY, // the gray is placed across all 3 channels (this applies to all X2GRAY conversions)
    RGB2YUV,
    RGB2YCBCR,
    RGB2HSV,

    // YUV source conversions
    YUV2RGB,
    YUV2GRAY,
    YUV2YCBCR,
    YUV2HSV,

    // YCbCr source conversions
    YCBCR2RGB,
    YCBCR2YUV,
    YCBCR2GRAY,
    YCBCR2HSV,

    // HSV source conversions
    HSV2RGB,
    HSV2GRAY,
    HSV2YCBCR,
    HSV2YUV,

    //! Is just used for enum type checking
    _CONVEND
};
typedef enum _channel_conversion_type_enum CONVTYPE;

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
 * @brief gets the GPIXEL* type at position (r,c) from a given PIXEL in an IMAGE
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
 * 
 * @returns int `0` if success. `-1` if `conv` is a non existant conversion type.
 */
int convert_channel_px(PIXEL* destpx, PIXEL* srcpx, CONVTYPE conv);


/**
 * @brief Converts the pixels from `srcimg` to a different color space specified by `conv` and writes the convereted pixels to `destimg` (can be same as `srcimg`)
 * 
 * @param destimg image to write all the converted pixels to
 * @param srcimg image to convert the pixels from
 * @param conv colorspace conversion type (ex: `RGB2HSV`)
 * 
 * @returns `int` `0` if success. `-1` if `conv` is a non existant conversion type. `-2` if out of bounds error on `destimg` (at least one of `srcimg`'s dimensions are bigger than those of `destimg`'s).
 */
int convert_channel_img(IMAGE* destimg, IMAGE* srcimg, CONVTYPE conv);

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
int convert_channel_img_range(IMAGE* destimg, IMAGE* srcimg, CONVTYPE conv, int r1, int c1, int r2, int c2);

/**
 * @brief Applies a binary threhold to a grayscale image (sets to 0 if strictly under threshold, else 255)
 * 
 * @param img grayscale image to apply the threhold to
 * @param thresh the threshold bound
 * @param underv the value if the pixel is under the threshold
 * @param abovev the value if the pixel is above the threshold
 */
void bin_gthresh_img(IMAGE* img, int thresh, BYTE underv, BYTE abovev);

/**
 * @brief individually applies a binary threhold to each rgb channel of an image (sets to 0 if strictly under threshold, else 255)
 * 
 * @param img grayscale image to apply the threhold to
 * @param rthresh the red threshold bound
 * @param gthresh the green threshold bound
 * @param bthresh the blue threshold bound
 * @param underv the value if the pixel is under the threshold
 * @param abovev the value if the pixel is above the threshold
 */
void bin_rgbthresh_img(IMAGE* img, int rthresh, int gthresh, int bthresh, BYTE underv, BYTE abovev);


/**
 * @brief Erodes pixels in a `range` "radius" square around (r,c)
 * 
 * @param r row in image
 * @param c column in image
 * @param destimg image to apply the erosion to
 * @param srcimg image to use to calculate erosion values
 * @param range radius of erosion
 * @return int number of pixels effectively erroded (less than range*range if some are out of bounds)
 */
int erode_px(int r, int c, IMAGE* destimg, IMAGE* srcimg, unsigned int range);

/**
 * @brief Dialtes pixels in a `range` "radius" square around (r,c)
 * 
 * @param r row in image
 * @param c column in image
 * @param destimg image to apply the dilation to
 * @param srcimg image to use to calculate dilation values
 * @param range radius of dilations
 * @return int number of pixels effectively dilated (less than range*range if some are out of bounds)
 */
int dilate_px(int r, int c, IMAGE* destimg, IMAGE* srcimg, unsigned int range);


int erode_img(IMAGE* destimg, IMAGE* srcimg);
int dilate_img(IMAGE* destimg, IMAGE* srcimg);

int fermeture_img(IMAGE* destimg, IMAGE* srcimg);
int ouverture_img(IMAGE* destimg, IMAGE* srcimg);


#endif
