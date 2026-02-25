#include <stdio.h>
#include <stdlib.h>
#include "imgops.h"
#include "imgio.h"


int main(int argc, char* argv[]) {


    if (argc != 3) {
        printf("Expected usage: %s <in_pathname> <out_pathname>\n", argv[0]);
        exit(1);
    }

    IMAGE inimg;

    read_pgm_image(argv[1], &inimg);

    IMAGE outimg = inimg;
    outimg.mat = pxalloc(outimg.width, outimg.height);
    if (outimg.mat == NULL) {
        printf("Error allocating memory for output image\n");
        free_img_pxmat(&inimg);
        exit(1);
    }

    int blur_range = 1;

    for (int r = 0; r < inimg.height; r++) {
        for (int c = 0; c < inimg.width; c++) {
            int n = 0;
            int sum = 0;

            // usedfor getting the 4 surrounding pixels in a + cross

            // get horizontal of cross
            for (int cc = c-blur_range; cc <= c+blur_range; cc++) {
                GPIXEL* px = get_gpixel(r, cc, &inimg);
                if (px != NULL) {
                    n++;
                    sum += px->v;
                }
            }
            // get vertical of cross
            for (int rr = r-blur_range; rr <= r+blur_range; rr++) {
                GPIXEL* px = get_gpixel(rr, c, &inimg);
                if (px != NULL) {
                    n++;
                    sum += px->v;
                }
            }

            // place the average of the surrounding pixels in the resulting image
            GPIXEL* destpx = get_gpixel(r, c, &outimg);
            // it's unlikely to be 0 if the image's sizes haven't been modified
            if (n != 0) {
                set_gpixel(
                    destpx,
                    sum/n
                );
            } else {
                set_gpixel(
                    destpx,
                    get_gpixel(r, c, &inimg)->v
                );
            }

        }
    }

    free_img_pxmat(&inimg);

    write_pgm2pgm(argv[2], &outimg);

    free_img_pxmat(&outimg);

    return 0;
}