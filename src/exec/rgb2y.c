#include <stdio.h>
#include <stdlib.h>
#include <imgio.h>
#include <imgops.h>


int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Expected usage: %s imgin.ppm imgout.pgm\n");
        exit(EXIT_FAILURE);
    }

    IMAGE img;

    read_ppm_image(argv[1], &img);

    for (int r = 0; r < img.height; r++) {
        for (int c = 0; c < img.width; c++) {
            RGBPIXEL* px = get_rgbpixel(r,c,&img);
            BYTE yval = 0.299*(px->r) + 0.587*(px->g) + 0.114*(px->b);

            set_rgbpixel(px, yval, yval, yval);

        }
    }

    write_pgm2pgm(argv[2], &img);


    free_imgmat(img.mat, img.height);

    return 0;

}