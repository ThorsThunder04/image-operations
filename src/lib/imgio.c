#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <imgio.h>
#include <imgops.h>


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


void read_ppm_image(char* filename, IMAGE* img) {

    FILE* imgfd = fopen(filename, "rb");

    // check that file exists
    if (imgfd == NULL) {
        printf("Error opening PPM file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // make sure the file being read is in PPM format
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

    // go over every pixel in the image file and add it to the matrix
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

void read_pgm_image(char* filename, IMAGE* img) {

    FILE* imgfd = fopen(filename, "rb");

    // check that file exists
    if (imgfd == NULL) {
        printf("Error opening PGM file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // make sure the file being read is a PGM type
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

    // go over every pixel in the image file and add it to the matrix
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

void free_img(IMAGE* img) {

    free_pxmat(img->mat, img->height);
    free(img);
}

void free_pxmat(PIXEL** mat, int height) {

    for (int r = 0; r < height; r++) {
        free(mat[r]);
    }

    free(mat);
    *mat = NULL;
}

void get_image_dimensions(char* filename, IMAGE* img) {

    FILE* imgfd = fopen(filename, "rb");
    _read_pnm_type(imgfd);
    _skip_comments(imgfd);
    _read_image_dimensions(imgfd, &(img->width), &(img->height));

    fclose(imgfd);
}

void img_copy(IMAGE* dest, IMAGE* src) {

    // copies the initial structure
    memcpy(dest, src, sizeof(IMAGE));

    dest->mat = pxalloc(dest->width, dest->height);

    pxmat_copy(dest->mat, src->mat, dest->width, dest->height);

}

void pxmat_copy(PIXEL** dest, PIXEL** src, int width, int height) {

    // copy each row of pixels from src to dest
    for (int r = 0; r < height; r++) {
        memcpy(dest[r], src[r], sizeof(PIXEL)*width);
    }
}


void extr_rchan2img(IMAGE* dest, IMAGE* src) {
    // yeah yeah it's just a wrapper, idc

    extr_rchan2pxmat(dest->mat, src->mat, src->width, src->height);

}

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

void extr_gchan2img(IMAGE* dest, IMAGE* src) {

    extr_gchan2pxmat(dest->mat, src->mat, src->width, src->height);

}

void extr_gchan2pxmat(PIXEL** dest, PIXEL** src, int width, int height) {

    // for each pixel, extract the green channel from src, and place it in the grey channel of dest
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            dest[r][c].gpx.v = src[r][c].cpx.g;
        }
    }
}

void extr_bchan2img(IMAGE* dest, IMAGE* src) {

    extr_bchan2pxmat(dest->mat, src->mat, src->width, src->height);

}

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

void _read_image_dimensions(FILE* fd, unsigned int* width, unsigned int* height) {

    int maxval;

    _skip_whitespace(fd);
    fscanf(fd, "%u %u %u%*c", width, height, &maxval);
}

IMGTYPE _read_pnm_type(FILE* fd) {
    IMGTYPE t;

    fscanf(fd, "P%u", &t);

    return t;
}