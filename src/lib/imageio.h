#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <stdio.h>

// the size of a color value within a pixel
typedef unsigned char BYTE;


// the size of a grayscale pixel
typedef struct {
    BYTE v;
} GPIXEL;

/** the size of a Red,Green,Blue pixel */
typedef struct {
    BYTE r;
    BYTE g;
    BYTE b;
} RGBPIXEL;

// used for pseudo-inheritance
typedef union {
    GPIXEL gpx;
    RGBPIXEL cpx;
} PIXEL;

/**
 * @brief structual definition of a multi channel pixel
 *
 * @member width: width of a given image
 * @member height: height of a given image
 * @member mat: 2D array of pixels. type PIXEL is either of type GPIXEL or RGBPIXEL. Can be checked from is_rgb
 * @member is_rgb: boolean value to check what kind of pixel structures are in `mat`
 */
typedef struct {
    unsigned int width;
    unsigned int height;
    PIXEL** mat;
    BYTE is_rgb;
} IMAGE;


/**
 * @brief reads PNM image from a given filename.
 * @brief Only supports PGM and PPM images of max pixel val 255.
 *
 * @param filename: string representing path to PNM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void get_image(char* filename, IMAGE* img);


/**
 * @brief De-allocates the dynamically allocated parts of an IMAGE structure
 * @brief NOTE: this only de-alloactes the pixel matrix part of an image. If the rest of the structure was dynamically allocated, it must be freed separately.
 *
 * @param img: pointer towards previously created image structure
 */
void free_image(IMAGE* img);

/**
 * @brief reads an image's dimesions from an open file
 */
void _read_image_dimensions(FILE* fd);

/**
 * @brief reads the dimensions from a PNM file and stores them in a provided IMAGE pointer
 *
 * @param filename: path to the PNM image
 * @param img: pointer towards an IMAGE type to store the width and height
 */
void get_image_dimensions(char* filename, IMAGE* img);

/**
 * @brief returns an integer `n` representing the number in a pnm's file's Pn which descripes what kind of pnm file it is. Must be executed starting the beginning of the file.
 *
 * @param fd: pointer towards the open file descriptor
 */
unsigned int _read_pnm_type(FILE* fd);



/**
 * @brief skips whatever comments there is between the magic number and the beginning of the data
 */
void _skip_comments(FILE* fd);

/**
 * @brief writes a given IMAGE to a PGM file. If the image data is rgb, the grayscale pixel values will be the average of red green and blue
 *
 * @param destname: pathname of the image file to write
 * @param img: IMAGE data to write to file
 */
void write_to_pgm(char* destname, IMAGE* img);

/**
 * @brief writes a given image to a PPM file. If the image data is grayscale, the grayscale pixel value will be trippled and writting to the 3 rgb values of a pixel.
 *
 * @param destname: pathname of the image file to write
 * @param img: IMAGE data to write to file
 */
void write_to_ppm(char* destname, IMAGE* img);

#endif