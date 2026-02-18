#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "imgio.h"
#include "imgops.h"

/**
 * @brief dynamic allocation of memory for pixel matrix
 *
 * @param width: width of the pixel matrix
 * @param height: height of the pixel matrix
 */
PIXEL** pixalloc(int width, int height) {
    // allocate all row pointers
    PIXEL** mat = (PIXEL**)malloc(sizeof(PIXEL*)*height);
    if (mat == NULL) {
        printf("Error allocating memory for pixel matrix rows !\n");
        exit(EXIT_FAILURE);
    }

    for (int r = 0; r < height; r++) {
        // allocate all columns of a row
        mat[r] = (PIXEL*)malloc(sizeof(PIXEL)*width);


        if (mat[r] == NULL) {
            printf("Error allocating memory for row %d !\n", r);
            // de-allocate all memory of the matrix allocated up until this point
            for (int rr = 0; rr < r; rr++) free(mat[rr]);
            free(mat);
            exit(EXIT_FAILURE);
        }
    }

    return mat;

}

/**
 * @brief reads PNM image from a given filename (detects wether it's PGM or PPM)
 * @brief Only supports PGM and PPM images of max pixel val 255.
 *
 * @param filename: string representing path to PNM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void read_pnm_image(char* filename, IMAGE* img) {

    FILE* imgfd = fopen(filename, "r");

    if (imgfd == NULL) {
        printf("Error opening PNM file!\n");
        exit(EXIT_FAILURE);
    }


    img->is_rgb = _read_pnm_type(imgfd) == 6; // PPM (rgb) type is "P6"
    unsigned int pixel_size = (img->is_rgb) ? sizeof(BYTE)*3 : sizeof(BYTE); // not using size of the pixel structures due to alignment

    _skip_comments(imgfd);
    _read_image_dimensions(imgfd, &(img->width), &(img->height));

    // allocate memory for image pixel matrice
    img->mat = pixalloc(img->width, img->height);

    for (int r = 0; r < img->height; r++) {
        for (int c = 0; c < img->width; c++) {
            fread((void*)&(img->mat[r][c]), pixel_size, 1, imgfd);
        }
    }

    fclose(imgfd);
}

/**
 * @brief Reads PPM image from a given filename.
 * @brief Only supports max pixel value of 255
 *
 * @param filename: string representing path to PPM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void read_ppm_image(char* filename, IMAGE* img); // TODO Implement

/**
 * @brief Reads PPM image from a given filename.
 * @brief Only supports max pixel value of 255
 *
 * @param filename: string representing path to PPM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void read_pgm_image(char* filename, IMAGE* img); // TODO Implement

/**
 * @brief writes a given IMAGE to a PGM file. If the image data is rgb, the grayscale pixel values will be the average of red green and blue
 *
 * @param destname: pathname of the image file to write
 * @param img: IMAGE data to write to file
 */
void write_to_pgm(char* destname, IMAGE* img) {

    FILE* imgfd = fopen(destname, "w");

    // write the image file headers
    fprintf(imgfd, "P5\n");
    fprintf(imgfd, "%u %u 255\n", img->width, img->height);

    for (int r = 0; r < img->height; r++) {
        for (int c = 0; c < img->width; c++) {


            PIXEL* px = &img->mat[r][c];

            // if the IMAGE object being written is in RGB
            if (img->is_rgb) *px = (PIXEL)rgb2g(&px->cpx);

            fwrite(
                (void*)px,
                sizeof(BYTE),
                1,
                imgfd
            );
        }
    }
    fclose(imgfd);
}

/**
 * @brief writes a given image to a PPM file. If the image data is grayscale, the grayscale pixel value will be trippled and writting to the 3 rgb values of a pixel.
 *
 * @param destname: pathname of the image file to write
 * @param img: IMAGE data to write to file
 */
void write_to_ppm(char* destname, IMAGE* img) {

    FILE* imgfd = fopen(destname, "w");
    if (imgfd == NULL) {
        printf("Error opening PPM image for writing\n");
        exit(EXIT_FAILURE);
    }

    fprintf(imgfd, "P6\n");
    fprintf(imgfd, "%u %u 255\n", img->width, img->height);

    int objSize = sizeof(BYTE)*3; // size of rgb pixels
    int writeSize = 1;

    if (!img->is_rgb) {
        objSize = sizeof(BYTE); // size of grayscale pixel
        writeSize = 3;
    }

    for (int r = 0; r < img->height; r++) {
        for (int c = 0; c < img->height; r++) {

            /*
             * Since the size of a PIXEL union is 3 BYTEs,
             * Here we iterate over each pixel object, and then only write
             * whichever section of it that we need
             *
             * If it's grayscale, we only need the first byte of the union structure (read the GPIXEL size) to get the color
             * Otherwise we need the entire union structure
             */
            fwrite(
                (void*)&(img->mat[r][c]),
                objSize,
                writeSize,
                imgfd
            );
        }
    }
    fclose(imgfd);
}

/**
 * @brief De-allocates the dynamically allocated parts of an IMAGE structure
 * @brief NOTE: this only de-alloactes the pixel matrix part of an image. If the rest of the structure was dynamically allocated, it must be freed separately.
 *
 * @param img: pointer towards previously created image structure
 */
void free_image(IMAGE* img) {

    // free each row
    for (int r = 0; r < img->height; r++) {
        free(img->mat[r]);
    }

    // free main image pointer
    free(img->mat);
    // set pointer to NULL to avoid seg. fault
    img->mat = NULL;
}


/**
 * @brief reads the dimensions from a PNM file and stores them in a provided IMAGE pointer
 *
 * @param filename: path to the PNM image
 * @param img: pointer towards an IMAGE type to store the width and height
 */
void get_image_dimensions(char* filename, IMAGE* img) {

    FILE* imgfd = fopen(filename, "r");
    _read_pnm_type(imgfd);
    _skip_comments(imgfd);
    _read_image_dimensions(imgfd, &(img->width), &(img->height));

    fclose(imgfd);
}



///////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////

/**
 * @brief skips whitespace in a file descriptor. After execution is done, the cursor is pointing on the first next non whitespace character.
 */
void _skip_whitespace(FILE* fd) {
    char c;

    // consumes whitespace
    while ((c = fgetc(fd)) != EOF && isspace(c)) ;

    // if we haven't reached the end of the file,
    // put the previously read character back on the filestream
    if (c != EOF) {
        ungetc(c, fd);
    }

}

/**
 * @brief skips whatever comments there is between the magic number and the beginning of the data
 * @brief NOTE: assums PNM type has already been read
 */
void _skip_comments(FILE* fd) {
    char c;
    _skip_whitespace(fd);
    if (feof(fd)) {
        printf("End of file reached while skipping comments!\n");
        fclose(fd);
        exit(EXIT_FAILURE);
    }
    while ((c = fgetc(fd)) == '#') {
        while ((c = fgetc(fd)) != '\n');
    }
    ungetc(c, fd);
}

/**
 * @brief reads an image's dimesions from an open file
 * @brief NOTE: assumes PNM type and comments have already been read
 */
void _read_image_dimensions(FILE* fd, unsigned int* width, unsigned int* height) {

    int maxval;

    fscanf(fd, "%u %u %u", width, height, &maxval);
}

/**
 * @brief returns an integer `n` representing the number in a pnm's file's Pn which descripes what kind of pnm file it is.
 * @brief Must be executed starting at the beginning of the file.
 *
 * @param fd: pointer towards the open file descriptor
 */
unsigned int _read_pnm_type(FILE* fd) {
    unsigned int t;

    fscanf(fd, "P%u", &t);

    return t;
}