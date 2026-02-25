#include <stdio.h>
#include <stdlib.h>
#include "imgio.h"
#include "imgops.h"
#include <string.h>


int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Exprected usage: %s <in_filepath>\n", argv[0]);
        return 1;
    }

    int hist[256][3];
    // set all values to 0
    for (int i = 0; i < 256; i++) memset(hist[i], 0, sizeof(int)*3);

    IMAGE img = {0};

    read_ppm_image(argv[1], &img);

    for (int r = 0; r < img.height; r++) {
        for (int c = 0; c < img.width; c++) {
            RGBPIXEL* px = get_rgbpixel(r,c,&img);
            hist[px->r][0]++;
            hist[px->g][1]++;
            hist[px->b][2]++;
        }
    }
    
    free_pxmat(img.mat, img.height);

    for (int i = 0; i < 256; i++) {
        printf("%d\t%d\t%d\t%d\n", i, hist[i][0], hist[i][1], hist[i][2]);
    }

    return 0;
}