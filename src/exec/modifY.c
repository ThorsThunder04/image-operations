
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imgio.h"
#include "imgops.h"



int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Expected usage: %s <Y_img.pgm> <outname.pgm> <k:-127 to 127>\n", argv[0]);
        exit(1);
    }

    int k = 0;
    sscanf(argv[3], "%d", &k);
    if (k <= -128 ||128 <= k ) {
        printf("Exptected K value to within -127 and 127, got %d", k);
        exit(1);
    }

    IMAGE img;
    read_pgm_image(argv[1], &img);

    for (int r = 0; r < img.height; r++) {
        for (int c = 0; c < img.width; c++) {
            GPIXEL* px = get_gpixel(r, c, &img);
            int v = px->v + k;
            // we clamp the updated exposure so that there are no overflows before putting it back on the image
            clampg(&v);
            px->v = v;
        }
    }

    write_pgm2pgm(argv[2], &img);

    free_img_pxmat(&img);

    return 0;
}