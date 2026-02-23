#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <stdio.h>

// the size of a color value within a pixel
typedef unsigned char BYTE;


// the size of a grayscale pixel

struct _gray_pixel_type_struct {
    BYTE v;
};
typedef struct _gray_pixel_type_struct GPIXEL;

/** the size of a Red,Green,Blue pixel */
struct _rgb_pixel_type_struct {
    BYTE r;
    BYTE g;
    BYTE b;
};
typedef struct _rgb_pixel_type_struct RGBPIXEL;

// used for pseudo-inheritance
union _pixel_type_enum {
    GPIXEL gpx;
    RGBPIXEL cpx;
};
typedef union _pixel_type_enum PIXEL;

enum _pnm_image_type_enum {
    PBM = 4,
    PGM,
    PPM,
    PAM

};
typedef enum _pnm_image_type_enum IMGTYPE;

/**
 * @brief structual definition of a multi channel pixel
 *
 * @member width: width of a given image
 * @member height: height of a given image
 * @member mat: 2D array of pixels. type PIXEL is either of type GPIXEL or RGBPIXEL. Can be checked from is_rgb
 * @member is_rgb: boolean value to check what kind of pixel structures are in `mat`
 */
struct _image_type_struct {
    unsigned int width;
    unsigned int height;
    PIXEL** mat;
    IMGTYPE img_type;
};
typedef struct _image_type_struct IMAGE;

/**
 * @brief dynamic allocation of memory for pixel matrix
 *
 * @param width: width of the pixel matrix
 * @param height: height of the pixel matrix
 *
 * @returns PIXEL** type (beginning of matrix) | NULL if dynamic allocation failed (refer to malloc of stdlib.h)
 */
PIXEL** pxalloc(int width, int height);


/**
 * @brief Reads PPM image from a given filename.
 * @brief Only supports max pixel value of 255
 *
 * @param filename: string representing path to PPM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void read_ppm_image(char* filename, IMAGE* img);

/**
 * @brief Reads PGM image from a given filename.
 * @brief Only supports max pixel value of 255
 *
 * @param filename string representing path to PGM file to open
 * @param img destination pointer to write the IMAGE data to
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
 * @brief De-allocates a dynamically allocated image and it's pixel matrix.
 * @brief NOTE: This attempts to free both `img` AND it's pixel matrix. If you only need to free the pixel matrix, use `free_pxmat()` to free it directly or `free_img_pxmat()` to free it by passing the image type.
 *
 * @param img: pointer towards previously created image structure
 */
void free_img(IMAGE* img);

/**
 * @brief De-allocates a dynamically allocated 2D pixel matrix of a given height
 *
 * @param mat: pointer towards the 2D array
 * @param height: height of the image
 */
void free_pxmat(PIXEL** mat, int height);

/**
 * @brief De-allocates the pixel matrix that is inside an image type
 * 
 * @param img the image type containing the matrix
 */
void free_img_pxmat(IMAGE* img);

/**
 * @brief reads the dimensions from a PNM file and stores them in a provided IMAGE pointer
 *
 * @param filename: path to the PNM image
 * @param img: pointer towards an IMAGE type to store the width and height
 */
void get_image_dimensions(char* filename, IMAGE* img);


/**
 * @brief copies an image into another pointer (including the pixel matrix)
 *
 * @param dest where to copy the image to
 * @param src the image to copy
 */
void copy_img(IMAGE* dest, IMAGE* src);

/**
 * @brief copies a 2D pixel matrix
 *
 * @param dest where to copy the matrix to
 * @param src  the matrix to copy
 * @param width width of the matrix
 * @param height height of the matrix
 */
void copy_pxmat(PIXEL** dest, PIXEL** src, int width, int height);


/**
 * @brief Extracts the red channel from an RGBPIXEL image and places it in a GPIXEL image
 *
 * @param dest destination image
 * @param src  source image
 */
void extr_rchan_img(IMAGE* dest, IMAGE* src);

/**
 * @brief Extracts the red channel from an 2D (RGB)PIXEL matrix and places it in a (G)PIXEL matrix
 *
 * @param dest destination matrix
 * @param src source matrix
 * @param width width of the matrix
 * @param height height of the matrix
 */
void extr_rchan_pxmat(PIXEL** dest, PIXEL** src, int width, int height);

/**
 * @brief Extracts the green channel from an RGBPIXEL image and places it in a GPIXEL image
 *
 * @param dest destination image
 * @param src  source image
 */
void extr_gchan_img(IMAGE* dest, IMAGE* src);

/**
 * @brief Extracts the green channel from an 2D (RGB)PIXEL matrix and places it in a (G)PIXEL matrix
 *
 * @param dest destination matrix
 * @param src source matrix
 * @param width width of the matrix
 * @param height height of the matrix
 */
void extr_gchan_pxmat(PIXEL** dest, PIXEL** src, int width, int height);

/**
 * @brief Extracts the blue channel from an RGBPIXEL image and places it in a GPIXEL image
 *
 * @param dest destination image
 * @param src  source image
 */
void extr_bchan_img(IMAGE* dest, IMAGE* src);

/**
 * @brief Extracts the blue channel from an 2D (RGB)PIXEL matrix and places it in a (G)PIXEL matrix
 *
 * @param dest destination matrix
 * @param src source matrix
 * @param width width of the matrix
 * @param height height of the matrix
 */
void extr_bchan_pxmat(PIXEL** dest, PIXEL** src, int width, int height);



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
