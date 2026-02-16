#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "imgio.h"


/**
 * @brief reads PNM image from a given filename.
 * @brief Only supports PGM and PPM images of max pixel val 255.
 *
 * @param filename: string representing path to PNM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void get_image(char* filename, IMAGE* img) {

    FILE* imgfd = fopen(filename, "r");

    img->is_rgb = _read_pnm_type(imgfd) == 6; // PPM (rgb) type is "P6"
    unsigned int pixel_size = (img->is_rgb) ? sizeof(BYTE)*3 : sizeof(BYTE); // not using size of the pixel structures due to alignment

    _skip_comments(imgfd);
    _read_image_dimensions(imgfd, &(img->width), &(img->height));

    // allocate row pointers of image
    PIXEL** imgmat = (PIXEL**)malloc(img->height*sizeof(PIXEL*));
    if (imgmat == NULL) {
        printf("Error allocating image rows !\n");
        exit(EXIT_FAILURE);
    }

    // allocate and read pixel rows of image
    for (int r = 0; r < img->height; r++) {
        imgmat[r] = (PIXEL*)malloc(img->width*sizeof(PIXEL));
        if (imgmat[r] == NULL) {
            printf("Error allocating image row columns !\n");

            // close and free allocated resources
            fclose(imgfd);
            for (int rr = 0; rr < r; rr++) free(imgmat[rr]);
            free(imgmat);

            exit(EXIT_FAILURE);
        }

        // can probably be done one row at a time, but I'm doing it like this rn since I've never used unions before
        for (int c = 0; c < img->width; c++) {
            fread((void*)&(imgmat[r][c]), pixel_size, 1, imgfd);
        }
    }
    fclose(imgfd);

    img->mat = imgmat;
}

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


/**
 * @brief De-allocates the dynamically allocated parts of an IMAGE structure
 * @brief NOTE: this only de-alloactes the pixel matrix part of an image. If the rest of the structure was dynamically allocated, it must be freed separately.
 *
 * @param img: pointer towards previously created image structure
 */
void free_image(IMAGE* img);


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
        exit(1);
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