#include <stdio.h>
#include <stdlib.h>
#include <imgio.h>
#include <imgops.h>


int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Expected usage: %s <in_pathname> <out_pathname> <threshvalue>\n", argv[0]);
        return 1;
    }

    IMAGE img = {0};
    int thresh = 0;
    sscanf(argv[3], "%d", &thresh);

    read_pgm_image(argv[1], &img);

    bin_gthreshimg(&img, thresh, 0, 255);

    write_pgm2pgm(argv[2], &img);

    free_pxmat(img.mat, img.height);

    return 0;
}