
#include <stdio.h>
#include <stdlib.h>
#include "imgio.h"
#include "imgops.h"


int main(int argc, char* argv[]) {

    if (argc != 5) {
        printf("Expected usage: %s <Y_image.pgm> <Cb_image.pgm> <Cr_image.pgm> <out_filepath>\n", argv[0]);
        exit(1);
    }

    IMAGE outimg;

    IMAGE Yimg;
    IMAGE Cbimg;
    IMAGE Crimg;
    printf("Reading images... \n");
    read_pgm_image(argv[1], &Yimg);
    read_pgm_image(argv[2], &Cbimg);
    read_pgm_image(argv[3], &Crimg);

    outimg = Yimg;
    outimg.img_type = PPM;
    outimg.mat = pxalloc(outimg.width, outimg.height);
    if (outimg.mat == NULL) {
        printf("Error allocating memory for output image\n");
        exit(1);
    }

    printf("Merging all 3 components... ");
    for (int r = 0; r < outimg.height; r++) {
        for (int c = 0; c < outimg.width; c++) {
            RGBPIXEL* px = get_rgbpixel(r, c, &outimg);
            px->r = get_gpixel(r, c, &Yimg)->v;
            px->g = get_gpixel(r, c, &Cbimg)->v;
            px->b = get_gpixel(r, c, &Crimg)->v;
        }
    }
    convert_channel_img(&outimg, &outimg, YCBCR2RGB);
    printf("DONE\n");


    printf("Writing image... ");
    write_ppm2ppm(argv[4], &outimg);
    printf("DONE\n");

    free_img_pxmat(&outimg);
    free_img_pxmat(&Yimg);
    free_img_pxmat(&Cbimg);
    free_img_pxmat(&Crimg);


    return 0;
}