#include <stdio.h>
#include <stdlib.h>
#include <imgio.h>
#include <imgops.h>


int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Exprected usage: %s <in_filepaht>\n", argv[0]);
        return 1;
    }

    int hist[256] = {0};
    IMAGE img = {0};

    read_pgm_image(argv[1], &img);

    for (int r = 0; r < img.height; r++) {
        for (int c = 0; c < img.width; c++) {
            GPIXEL* px = get_gpixel(r,c,&img);
            hist[px->v]++;
        }
    }
    
    free_pxmat(img.mat, img.height);

    for (int i = 0; i < 256; i++) {
        printf("%d %d\n", i, hist[i]);
    }

    return 0;
}