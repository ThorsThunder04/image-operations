#include <stdio.h>
#include <stdlib.h>
#include <imgio.h>
#include <imgops.h>


int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Expected usage: %s <in_pathname> <out_pathname>\n", argv[0]);
        return 0;
    }

    IMAGE inimg;
    read_ppm_image(argv[1], &inimg);

    IMAGE outimg;
    copy_img(&outimg, &inimg);
    
    printf("Bluring PPM image...");

    // not checking errors as there's no reason for the shape of inimg and outimg to not match
    apply_blur2img(
        &blur_rgbpx_square,
        &outimg,
        &inimg,
        1
    );



    write_ppm2ppm(argv[2], &outimg);

    free_img_pxmat(&outimg);
    free_img_pxmat(&inimg);

    printf(" DONE!");

    return 0;
}