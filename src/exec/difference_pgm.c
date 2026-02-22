#include <stdio.h>
#include <stdlib.h>
#include <imgio.h>
#include <imgops.h>

int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Expected usage: %s <in_filepath_1> <in_filepath_2> <out_filepath>\n", argv[0]);
        return 1;
    }

    IMAGE img1 = {0};
    IMAGE img2 = {0};

    read_pgm_image(argv[1], &img1);
    read_pgm_image(argv[2], &img2);

    if (img1.height != img2.height) {
        printf("Incompatible image heights !\n");
        free_pxmat(img1.mat, img1.height);
        free_pxmat(img2.mat, img2.height);
        return 1;
    }
    if (img1.width != img2.width) {
        printf("Incompatible image widths !\n");
        free_pxmat(img1.mat, img1.height);
        free_pxmat(img2.mat, img2.height);
        return 1;
    }

    IMAGE outimg = img1;
    outimg.mat = pxalloc(outimg.width, outimg.height);
    if (outimg.mat == NULL) {
        printf("Error allocating memory for output image matrix !\n");
        free_pxmat(img1.mat, img1.height);
        free_pxmat(img2.mat, img2.height);
        return 1;
    }


    // creates contour image
    for (int r = 0; r < img1.height; r++) {
        for (int c = 0; c < img2.width; c++) {
            GPIXEL* px1 = get_gpixel(r,c,&img1);
            GPIXEL* px2 = get_gpixel(r,c,&img2);

            set_gpixel(
                get_gpixel(r, c, &outimg),
                (px1->v == px2->v) ? 255 : 0
            );
        }
    }

    write_pgm2pgm(argv[3], &outimg);

    // previous if statements assert that they all have the same height
    free_pxmat(outimg.mat, outimg.height);
    free_pxmat(img1.mat, outimg.height);
    free_pxmat(img2.mat, outimg.height);


    return 0;
}