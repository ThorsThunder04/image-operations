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
    RGB2XYZ,

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
 * @brief Erodes pixels in a `radius` square around (r,c)
 * 
 * @param r row in image
 * @param c column in image
 * @param destimg image to apply the erosion to
 * @param srcimg image to use to calculate erosion values
 * @param radius how far to erode around `(r,c)` (in a square)
 * @return int number of pixels effectively erroded (less than range*range if some are out of bounds)
 */
int erode_px(int r, int c, IMAGE* destimg, IMAGE* srcimg, unsigned int radius);

/**
 * @brief Dialtes pixels in a `range` "radius" square around (r,c)
 * 
 * @param r row in image
 * @param c column in image
 * @param destimg image to apply the dilation to
 * @param srcimg image to use to calculate dilation values
 * @param radius how far to dilate around `(r,c)` (in a square)
 * @return int number of pixels effectively dilated (less than range*range if some are out of bounds)
 */
int dilate_px(int r, int c, IMAGE* destimg, IMAGE* srcimg, unsigned int radius);


/**
 * @brief Errodes an entire image with the given range
 * 
 * @param destimg image to write the erosion to
 * @param srcimg the image to erode
 * @param radius how far to erode around each pixel
 */
void erode_img(IMAGE* destimg, IMAGE* srcimg, unsigned int radius);

/**
 * @brief Dilates an entire image with the given range
 * 
 * @param destimg image to write the dilation to
 * @param srcimg the image to dilate
 * @param radius how far to dilate around each pixel
 */
void dilate_img(IMAGE* destimg, IMAGE* srcimg, unsigned int radius);

/**
 * @brief Applies a dilation then an erosion to an image
 * 
 * @param destimg resulting image
 * @param srcimg source image to start the process on
 * @param radius how far to dialte and erode around each pixel
 */
void fermeture_img(IMAGE* destimg, IMAGE* srcimg, unsigned int radius);

/**
 * @brief Applies an erosion then a dilation to an image
 * 
 * @param destimg resulting image
 * @param srcimg source image to start the process on
 * @param radius how far to dialte and erode around each pixel
 */
void ouverture_img(IMAGE* destimg, IMAGE* srcimg, unsigned int radius);

/**
 * @brief Calculates the blur value of a grayscale image using a cross around (r,c) and places it on a destination image
 * 
 * @param destimg image to place the blured pixel on
 * @param srcimg source image to calculate the blur from
 * @param r row of pixel
 * @param c column of pixel
 * @param range the size of the cross
 * @return int number of effective pixels used for the blur | 0 if out of bounds error with (r,c) on `destimg` or `srcimg`, 
 */
int blur_gpx_cross(IMAGE* destimg, IMAGE* srcimg, int r, int c, unsigned int range);

/**
 * @brief Calculates the blur value of a rgb image using a cross around (r,c) and places it on a destination image
 * 
 * @param destimg image to place the blured pixel on
 * @param srcimg source image to calculate the blur from
 * @param r row of pixel
 * @param c column of pixel
 * @param range the size of the cross
 * @return int number of effective pixels used for the blur | 0 if out of bounds error with (r,c) on `destimg` or `srcimg`, 
 */
int blur_rgbpx_cross(IMAGE* destimg, IMAGE* srcimg, int r, int c, unsigned int range);

/**
 * @brief Calculates the blur value of a grayscale image using a square around (r,c) and places it on a destination image
 * 
 * @param destimg image to place the blured pixel on
 * @param srcimg source image ot calculate the blur from
 * @param r row of pixel
 * @param c column of pixel
 * @param range the size of the square (one side would be 1 + range*2)
 * @return int number of effective pixels used for the blur | 0 if out of bounds error with (r,c) on `destimg` or `srcimg`, 
 */
int blur_gpx_square(IMAGE* destimg, IMAGE* srcimg, int r, int c, unsigned int range);

/**
 * @brief Calculates the blur value of a rgb image using a square around (r,c) and places it on a destination image
 * 
 * @param destimg image to place the blured pixel on
 * @param srcimg source image ot calculate the blur from
 * @param r row of pixel
 * @param c column of pixel
 * @param range the size of the square (one side would be 1 + range*2)
 * @return int number of effective pixels used for the blur | 0 if out of bounds error with (r,c) on `destimg` or `srcimg`, 
 */
int blur_rgbpx_square(IMAGE* destimg, IMAGE* srcimg, int r, int c, unsigned int range);

/**
 * @brief Applies any blur function with the corresponding signature to an image
 * 
 * @param blur_func the blur function to apply to each pixel of an image
 * @param destimg the image to place the blured pixels to
 * @param srcimg  the image to use to calculate the blur values
 * @param range the range of the blur
 * @return int 0 if success. -1 if shape of `destimg` and `srcimg` don't match
 */
int apply_blur2img(
    int (*blur_func)(IMAGE*, IMAGE*, int, int, unsigned int),
    IMAGE* destimg,
    IMAGE* srcimg,
    unsigned int range
);

/**
 * @brief apply a given image bluring function to an image `n_reps` times
 * 
 * @param blur_img_func the image blur function to apply
 * @param destimg the destination image to write the resulting blur to
 * @param srcimg the source image to calcualte the blur from
 * @param range the range of the blur
 * @param n_reps the amount of times to apply `blur_img_func` to an image before writing it to `destimg`
 * @returns 0 if success. -1 if error allocating intermediate images
 */
int blur_img_rep(int (*blur_img_func)(IMAGE*, IMAGE*, unsigned int), IMAGE* destimg, IMAGE* srcimg, unsigned int range, int n_reps);

/**
 * @brief clamps a single value between 0 and 255
 * 
 * @param g grayscale pixel value
 */
void clampg(int* g);

/**
 * @brief Clamps the values of r,g,b to be between 0 and 255. if above 255, set to 255, if under 0, set to 0
 * 
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 */
void clamprgb(int* r, int* g, int* b);

/**
 * @brief Calculates the gradient of a given grayscale pixel
 * 
 * @param r row of pixel
 * @param c column of pixel
 * @param img image to calculate gradients from
 * @return int return the calculated gradient value
 */
int grad_gpx(int r, int c, IMAGE* img);

/**
 * @brief calculate norme gradient image of a given grayscale image
 * 
 * @param destimg image to write the resulting gradient to
 * @param srcimg image to calculate the gradient from
 * @param return int `0` if success. `-1` if the shape of `destimg` and `srcimg` don't match. `-2` if error getting pixel from `destimg`
 */
int grad_gimg(IMAGE* destimg, IMAGE* srcimg);


#endif
