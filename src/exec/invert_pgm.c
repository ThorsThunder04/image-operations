#include <stdio.h>
#include <stdlib.h>
#include "imgio.h"
#include "imgops.h"


int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Expected usage: %s <in_pathname> <out_pathname>\n", argv[0]);
        return 1;
    }

    IMAGE img = {0};

    read_pgm_image(argv[1], &img);

    for (int r = 0; r < img.height; r++) {
        for (int c = 0; c < img.width; c++) {
            GPIXEL* px = get_gpixel(r, c, &img);
            set_gpixel(px, 255 - px->v);
        }
    }
    
    write_pgm2pgm(argv[2], &img);

    free_pxmat(img.mat, img.height);

    return 0;
}