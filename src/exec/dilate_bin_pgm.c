#include <stdio.h>
#include <stdlib.h>
#include <imgio.h>
#include <imgops.h>



int main(int argc, char* argv[]) {

    if (argc != 3 ) {
        printf("Expected usage: %s <in_pathname> <out_pathname>\n");
        return 1;
    }
    
    
    IMAGE srcimg;
    read_pgm_image(argv[1], &srcimg);

    IMAGE destimg;
    copy_img(&destimg, &srcimg);

    for (int r = 0; r < srcimg.height; r++) {
        for (int c = 0; c < srcimg.width; c++) {
            dilate_px(r, c, &destimg, &srcimg, 1);
        }
    }

    free_pxmat(srcimg.mat, srcimg.height);

    write_pgm2pgm(argv[2], &destimg);

    free_pxmat(destimg.mat, destimg.height);

    return 0;
}