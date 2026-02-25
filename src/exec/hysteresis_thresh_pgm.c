#include <stdio.h>
#include <stdlib.h>
#include "imgio.h"
#include "imgops.h"



int main(int argc, char* argv[]) {

    if (argc != 5) {
        printf("Expected usage: %s <in_image.pgm> <out_image.pgm> <minthresh> <maxthresh>\n", argv[0]);
        exit(1);
    }

    int minthresh, maxthresh;

    sscanf(argv[3], "%d", &minthresh);
    sscanf(argv[4], "%d", &maxthresh);

    IMAGE srcimg;
    IMAGE destimg;

    read_pgm_image(argv[1], &srcimg);
    // to copy over all data and allocate pixel matrix
    copy_img(&destimg, &srcimg);

    int res = grad_gimg(&destimg, &srcimg);
    if (res == -1) {
        printf("Error: error related to the shapes of the given images\n");
        exit(1);
    } else if (res == -2) {
        printf("There was an allocation error somewhere with `destimg`\n");
        exit(1);
    }

    free_img_pxmat(&srcimg);

    // first threshold go over
    for (int r = 0; r < destimg.height; r++) {
        for (int c = 0; c < destimg.width; c++) {
            GPIXEL* gradpx = get_gpixel(r, c, &destimg);
            if (gradpx->v <= minthresh) {
                gradpx->v = 0;
            } else if (maxthresh <= gradpx->v) {
                gradpx->v = 255;
            }
        }
    }
    // write_pgm2pgm(argv[2], &destimg);

    // second threshold go over
    for (int r = 0; r < destimg.height; r++) {
        for (int c = 0; c < destimg.width; c++) {
            GPIXEL* gradpx = get_gpixel(r, c, &destimg);

            int cond1 = (minthresh < gradpx->v && gradpx->v < maxthresh);
            int cond2 = 0;

            // look for at least one 255 in 8 neighbors
            for (int rr = r-1; rr <= r+1; rr++) {
                for (int cc = c-1; cc <= c+1; cc++) {
                    GPIXEL* npx = get_gpixel(r,c, &destimg);
                    if (npx != NULL && r != rr && c != cc && npx->v == 255) {
                        cond2++;
                    }
                }
            }

            if (cond1) {
                if (cond2) {
                    gradpx->v = 255;
                } else {
                    gradpx->v = 0;
                }
            }
            // gradpx->v = (cond1 && cond2 > 0) ? 255 : 0;

        }
    }

    write_pgm2pgm(argv[2], &destimg);

    free_img_pxmat(&destimg);
    
    return 0;
}