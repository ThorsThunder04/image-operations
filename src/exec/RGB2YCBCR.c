#include <stdio.h>
#include <stdlib.h>
#include <imgio.h>
#include <imgops.h>


int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Expected usage: %s <in_filepath> <out_filepath>\n", argv[0]);
        exit(1);
    }

    IMAGE img;
    read_ppm_image(argv[1], &img);

    convert_channel_img(&img, &img, RGB2YCBCR);

    write_ppm2ppm(argv[2], &img);

    free_img_pxmat(&img);

    return 0;
}