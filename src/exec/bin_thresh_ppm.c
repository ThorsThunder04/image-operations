#include <stdio.h>
#include <stdlib.h>
#include <imgio.h>
#include <imgops.h>


int main(int argc, char* argv[]) {

    if (argc != 6) {
        printf("Expected usage: %s <in_pathname> <out_pathname> <rthresh> <bthresh> <cthresh>\n", argv[0]);
        return 1;
    }

    IMAGE img = {0};
    int rthresh = 0;
    int gthresh = 0;
    int bthresh = 0;
    sscanf(argv[3], "%d", &rthresh);
    sscanf(argv[4], "%d", &gthresh);
    sscanf(argv[5], "%d", &bthresh);

    read_ppm_image(argv[1], &img);

    bin_rgbthresh_img(&img, rthresh, gthresh, bthresh, 0, 255);

    write_ppm2ppm(argv[2], &img);

    free_pxmat(img.mat, img.height);

    return 0;
}