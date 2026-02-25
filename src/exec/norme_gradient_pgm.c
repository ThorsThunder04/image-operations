#include <stdio.h>
#include <stdlib.h>
#include "imgio.h"
#include "imgops.h"



int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Expected usage: %s <in_image.pgm> <out_image.pgm>\n", argv[0]);
        exit(1);
    }

    IMAGE srcimg;
    IMAGE destimg;

    read_pgm_image(argv[1], &srcimg);
    // to copy over all data and allocate pixel matrix
    copy_img(&destimg, &srcimg);

    grad_gimg(&destimg, &srcimg);

    write_pgm2pgm(argv[2], &destimg);

    free_img_pxmat(&destimg);
    free_img_pxmat(&srcimg);
    
    return 0;
}