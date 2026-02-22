#include <stdio.h>
#include <stdlib.h>
#include <imgio.h>
#include <imgops.h>


int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Exprected usage: %s <in_filepath> <c|r> <index>\n", argv[0]);
        return 1;
    }

    IMAGE img = {0};
    int index;
    sscanf(argv[3], "%d", &index);
    int is_row = argv[2][0] == 'r';

    read_pgm_image(argv[1], &img);

    int minr,minc,maxr,maxc;

    if (is_row) {
        minr = index;
        maxr = minr+1;
        minc = 0;
        maxc = img.width;
    } else {
        minr = 0;
        maxr = img.height;
        minc = index;
        maxc = minc+1;
    }


    int profile[256] = {0};


    for (int r = minr; r < maxr; r++) {
        for (int c = minc; c < maxc; c++) {
            GPIXEL* px = get_gpixel(r, c, &img);
            profile[px->v]++;
        }
    }
    
    free_pxmat(img.mat, img.height);

    for (int i = 0; i < 256; i++) {
        printf("%d\t%d\n", i, profile[i]);
    }


    return 0;
}