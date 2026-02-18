#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <imgio.h>
#include <imgops.h>

/**
 * @brief dynamic allocation of memory for pixel matrix
 *
 * @param width: width of the pixel matrix
 * @param height: height of the pixel matrix
 *
 * @returns PIXEL** type (beginning of matrix) | NULL if dynamic allocation failed (refer to malloc of stdlib.h)
 */
PIXEL** pxalloc(int width, int height) {
    // allocate all row pointers
    PIXEL** mat = (PIXEL**)malloc(sizeof(PIXEL*)*height);
    if (mat == NULL) {
        // propagate the MALLOC error, as it's essencially just that
        return NULL;
    }

    for (int r = 0; r < height; r++) {
        // allocate all columns of a row
        mat[r] = (PIXEL*)malloc(sizeof(PIXEL)*width);


        if (mat[r] == NULL) {
            printf("Error allocating memory for row %d !\n", r);
            // de-allocate all memory of the matrix allocated up until this point
            for (int rr = 0; rr < r; rr++) free(mat[rr]);
            free(mat);

            // propagate the MALLOC error
            return NULL;
        }
    }

    return mat;

}

/**
 * @brief reads PNM image from a given filename (detects wether it's PGM or PPM)
 * @brief Only supports PGM and PPM images of max pixel val 255.
 * 
 * ! this function should really get deprecated soon
 *
 * @param filename: string representing path to PNM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void read_pnm_image(char* filename, IMAGE* img) {

    FILE* imgfd = fopen(filename, "rb");

    // check that file exists
    if (imgfd == NULL) {
        printf("Error opening PNM file!\n");
        exit(EXIT_FAILURE);
    }


    img->img_type = _read_pnm_type(imgfd); // PPM (rgb) type is "P6"
    unsigned int pixel_size = (img->img_type == PPM) ? sizeof(BYTE)*3 : sizeof(BYTE); // not using size of the pixel structures due to alignment

    _skip_comments(imgfd);
    _read_image_dimensions(imgfd, &(img->width), &(img->height));


    // allocate memory for image pixel matrice
    img->mat = pxalloc(img->width, img->height);

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
void read_ppm_image(char* filename, IMAGE* img) {

    FILE* imgfd = fopen(filename, "rb");

    // check that file exists
    if (imgfd == NULL) {
        printf("Error opening PPM file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    IMGTYPE it = _read_pnm_type(imgfd);

    if (it != PPM) {
        printf("Incorrect file type! Tried opening PPM, P%d type recieved!\n", it);
        fclose(imgfd);
        exit(EXIT_FAILURE);
    }

    img->img_type = PPM;

    _skip_comments(imgfd);

    unsigned int width, height;
    _read_image_dimensions(imgfd, &width, &height);

    img->mat = pxalloc(width, height);
    if (img->mat == NULL) {
        printf("Error allocating pixel matrix memory for read PPM file %s\n", filename);
        fclose(imgfd);
    }

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {

            int res = fread(
                (void*)&img->mat[r][c],
                sizeof(BYTE)*3, // RGBPIXEL works too, but I think 3*BYTE is safer
                1,
                imgfd
            );
            if (res < 1) {
                if (feof(imgfd)) {
                    printf("Error: unexpectedly reached end of file in %s !\n", filename);
                } else {
                    printf("Error: something went wrong while reading from %s !\n", filename);
                }
            }

        }
    }
    fclose(imgfd);

    img->width = width;
    img->height = height;

}

/**
 * @brief Reads PGM image from a given filename.
 * @brief Only supports max pixel value of 255
 *
 * @param filename: string representing path to PGM file to open
 * @param img: destination pointer to write the IMAGE data to
 */
void read_pgm_image(char* filename, IMAGE* img) {

    FILE* imgfd = fopen(filename, "rb");

    // check that file exists
    if (imgfd == NULL) {
        printf("Error opening PGM file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    IMGTYPE it = _read_pnm_type(imgfd);

    if (it != PGM) {
        printf("Incorrect file type! Tried opening PGM, P%d type recieved!\n", it);
        fclose(imgfd);
        exit(EXIT_FAILURE);
    }

    img->img_type = PGM;

    _skip_comments(imgfd);

    unsigned int width, height;
    _read_image_dimensions(imgfd, &width, &height);

    img->mat = pxalloc(width, height);
    if (img->mat == NULL) {
        printf("Error allocating pixel matrix memory for read PGM file %s\n", filename);
        fclose(imgfd);
    }

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            int res = fread(
                (void*)&img->mat[r][c],
                sizeof(GPIXEL),
                1,
                imgfd
            );

            if (res < 1) {
                if (feof(imgfd)) {
                    printf("Error: unexpectedly reached end of file in %s !\n", filename);
                } else {
                    printf("Error: something went wrong while reading from %s !\n", filename);
                }
            }

        }
    }
    fclose(imgfd);

    img->width = width;
    img->height = height;
}

/**
 * @brief writes a given IMAGE to a PGM file.
 *
 * @param destname: pathname of the image file to write
 * @param img: IMAGE data to write to file
 */
void write_pgm2pgm(char* destname, IMAGE* img) {

    FILE* imgfd = fopen(destname, "wb");

    // write the image file headers
    fprintf(imgfd, "P5\r");
    fprintf(imgfd, "%u %u\r255\r", img->width, img->height);

    for (int r = 0; r < img->height; r++) {
        for (int c = 0; c < img->width; c++) {

            int res = fwrite(
                (void*)&img->mat[r][c].gpx,
                sizeof(BYTE),
                1,
                imgfd
            );
            if (res < 1) {
                printf("Error writing pgm2pgm to %s\n", destname);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(imgfd);
}

/**
 * @brief writes a given image to a PPM file.
 *
 * @param destname: pathname of the image file to write
 * @param img: IMAGE data to write to file
 */
void write_ppm2ppm(char* destname, IMAGE* img) {

    FILE* imgfd = fopen(destname, "wb");
    if (imgfd == NULL) {
        printf("Error opening PPM image for writing\n");
        exit(EXIT_FAILURE);
    }

    fprintf(imgfd, "P6\r");
    fprintf(imgfd, "%u %u\r255\r", img->width, img->height);


    for (int r = 0; r < img->height; r++) {
        for (int c = 0; c < img->width; c++) {

            int res = fwrite(
                (void*)&img->mat[r][c],
                sizeof(BYTE)*3,
                1,
                imgfd
            );

            if (res < 1) {
                printf("Error writing ppm2ppm to %s\n", destname);
                exit(EXIT_FAILURE);
            }

        }
    }

    fclose(imgfd);
}

/**
 * @brief De-allocates the dynamically allocated parts of an IMAGE structure
 * @brief NOTE: this only de-alloactes the pixel matrix part of an image. If the rest of the structure was dynamically allocated, it must be freed separately.
 *
 * @param img: pointer towards previously allocated image structure
 */
void free_img(IMAGE* img) {

    free_imgmat(img->mat, img->height);
    free(img);
}

/**
 * @brief De-allocates a 2D pixel array of a given size
 *
 * @param mat: pointer towards the 2D array
 * @param height: height of the image
 */
void free_imgmat(PIXEL** mat, int height) {

    for (int r = 0; r < height; r++) {
        free(mat[r]);
    }

    free(mat);
    *mat = NULL;
}

/**
 * @brief reads the dimensions from a PNM file and stores them in a provided IMAGE pointer
 *
 * @param filename: path to the PNM image
 * @param img: pointer towards an IMAGE type to store the width and height
 */
void get_image_dimensions(char* filename, IMAGE* img) {

    FILE* imgfd = fopen(filename, "rb");
    _read_pnm_type(imgfd);
    _skip_comments(imgfd);
    _read_image_dimensions(imgfd, &(img->width), &(img->height));

    fclose(imgfd);
}

/**
 * @brief copies an image into another pointer (including allocation of the pixel matrix)
 * 
 * @param dest where to copy the image to
 * @param src the image to copy
 */
void img_copy(IMAGE* dest, IMAGE* src) {

    // copies the initial structure
    memcpy(dest, src, sizeof(IMAGE));

    dest->mat = pxalloc(dest->width, dest->height);

    pxmat_copy(dest->mat, src->mat, dest->width, dest->height);

}

/**
 * @brief copies a 2D pixel matrix
 * 
 * @param dest where to copy the matrix to (must already have the necessary allocated space, otherwise seg fault)
 * @param src  the matrix to copy
 * @param width width of the matrix
 * @param height height of the matrix
 */
void pxmat_copy(PIXEL** dest, PIXEL** src, int width, int height) {

    // copy each row of pixels from src to dest
    for (int r = 0; r < height; r++) {
        memcpy(dest[r], src[r], sizeof(PIXEL)*width);
    }
}


/**
 * @brief Extracts the red channel from an RGBPIXEL image and places it in a GPIXEL image
 * 
 * @param dest destination image
 * @param src  source image
 */
void extr_rchan2img(IMAGE* dest, IMAGE* src) {
    // yeah yeah it's just a wrapper, idc

    extr_rchan2pxmat(dest->mat, src->mat, src->width, src->height);

}

/**
 * @brief Extracts the red channel from an 2D (RGB)PIXEL matrix and places it in a (G)PIXEL matrix
 * 
 * @param dest destination matrix
 * @param src source matrix
 * @param width width of the matrix
 * @param height height of the matrix
 */
void extr_rchan2pxmat(PIXEL** dest, PIXEL** src, int width, int height) {
    /*
    This operation is sort of redundant in a sense, as since PIXEL is a union, the location of R
    from an RGBPIXEL is in the same position as a V from a GPIXEL. So the operation is kind of just like
    we are copying a single channel over to a different matrix.
    */

    // for each pixel, extract the red channel from src, and place it in the grey channel of dest
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            dest[r][c].gpx.v = src[r][c].cpx.r;
        }
    }
}

/**
 * @brief Extracts the green channel from an RGBPIXEL image and places it in a GPIXEL image
 * 
 * @param dest destination image
 * @param src  source image
 */
void extr_gchan2img(IMAGE* dest, IMAGE* src) {

    extr_gchan2pxmat(dest->mat, src->mat, src->width, src->height);

}

/**
 * @brief Extracts the green channel from an 2D (RGB)PIXEL matrix and places it in a (G)PIXEL matrix
 * 
 * @param dest destination matrix
 * @param src source matrix
 * @param width width of the matrix
 * @param height height of the matrix
 */
void extr_gchan2pxmat(PIXEL** dest, PIXEL** src, int width, int height) {

    // for each pixel, extract the green channel from src, and place it in the grey channel of dest
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            dest[r][c].gpx.v = src[r][c].cpx.g;
        }
    }
}

/**
 * @brief Extracts the blue channel from an RGBPIXEL image and places it in a GPIXEL image
 * 
 * @param dest destination image
 * @param src  source image
 */
void extr_bchan2img(IMAGE* dest, IMAGE* src) {

    extr_bchan2pxmat(dest->mat, src->mat, src->width, src->height);

}

/**
 * @brief Extracts the blue channel from an 2D (RGB)PIXEL matrix and places it in a (G)PIXEL matrix
 * 
 * @param dest destination matrix
 * @param src source matrix
 * @param width width of the matrix
 * @param height height of the matrix
 */
void extr_bchan2pxmat(PIXEL** dest, PIXEL** src, int width, int height) {

    // for each pixel, extract the blue channel from src, and place it in the grey channel of dest
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            dest[r][c].gpx.v = src[r][c].cpx.b;
        }
    }
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

    _skip_whitespace(fd);
    fscanf(fd, "%u %u %u%*c", width, height, &maxval);
}

/**
 * @brief returns an integer `n` representing the number in a pnm's file's Pn which descripes what kind of pnm file it is.
 * @brief Must be executed starting at the beginning of the file.
 *
 * @param fd: pointer towards the open file descriptor
 */
IMGTYPE _read_pnm_type(FILE* fd) {
    IMGTYPE t;

    fscanf(fd, "P%u", &t);

    return t;
}