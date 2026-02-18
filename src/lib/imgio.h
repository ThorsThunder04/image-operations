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

typedef enum {
    PBM = 4,
    PGM,
    PPM,
    PAM

} IMGTYPE;

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
    IMGTYPE img_type;
} IMAGE;

/**
 * @brief dynamic allocation of memory for pixel matrix
 *
 * @param width: width of the pixel matrix
 * @param height: height of the pixel matrix
 *
 * @returns PIXEL** type (beginning of matrix)
 */
PIXEL** pixalloc(int width, int height);


/**
 * @brief Reads PNM image from a given filename (detects wether it's PGM or PPM)
 * @brief Only supports PGM and PPM images of max pixel val 255.
 *
 * @param filename: string representing path to PNM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void read_pnm_image(char* filename, IMAGE* img);

/**
 * @brief Reads PPM image from a given filename.
 * @brief Only supports max pixel value of 255
 *
 * @param filename: string representing path to PPM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void read_ppm_image(char* filename, IMAGE* img);

/**
 * @brief Reads PPM image from a given filename.
 * @brief Only supports max pixel value of 255
 *
 * @param filename: string representing path to PPM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void read_pgm_image(char* filename, IMAGE* img);


/**
 * @brief writes a given IMAGE to a PGM file.
 *
 * @param destname: pathname of the image file to write
 * @param img: IMAGE data to write to file
 */
void write_pgm2pgm(char* destname, IMAGE* img);

/**
 * @brief writes a given image to a PPM file.
 *
 * @param destname: pathname of the image file to write
 * @param img: IMAGE data to write to file
 */
void write_ppm2ppm(char* destname, IMAGE* img);


/**
 * @brief De-allocates the dynamically allocated parts of an IMAGE structure
 * @brief NOTE: this only de-alloactes the pixel matrix part of an image. If the rest of the structure was dynamically allocated, it must be freed separately.
 *
 * @param img: pointer towards previously created image structure
 */
void free_img(IMAGE* img);

/**
 * @brief De-allocates a 2D pixel array of a given size
 *
 * @param mat: pointer towards the 2D array
 * @param height: height of the image
 */
void free_imgmat(PIXEL** mat, int height);

/**
 * @brief reads the dimensions from a PNM file and stores them in a provided IMAGE pointer
 *
 * @param filename: path to the PNM image
 * @param img: pointer towards an IMAGE type to store the width and height
 */
void get_image_dimensions(char* filename, IMAGE* img);

///////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////


/**
 * @brief skips whitespace in a file descriptor. After execution is done, the cursor is pointing on the first next non whitespace character.
 */
void _skip_whitespace(FILE* fd);

/**
 * @brief skips whatever comments there is between the magic number and the beginning of the data
 * @brief NOTE: assums PNM type has already been read
 */
void _skip_comments(FILE* fd);

 /**
  * @brief reads an image's dimesions from an open file
  * @brief NOTE: assumes PNM type and comments have already been read
  */
void _read_image_dimensions(FILE* fd, unsigned int* width, unsigned int* height);

/**
 * @brief returns an integer `n` representing the number in a pnm's file's Pn which descripes what kind of pnm file it is.
 * @brief Must be executed starting at the beginning of the file.
 *
 * @param fd: pointer towards the open file descriptor
 */
unsigned int _read_pnm_type(FILE* fd);



#endif