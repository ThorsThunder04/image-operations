#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <imgio.h>
#include <imgops.h>



int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        printf("Expected usage: %s inimg.ppm outname\n", argv[0]);
        printf("Output: {outname}_Y.pgm {outname}_Cb.pgm {outname}_Cr.pgm\n");
        exit(EXIT_FAILURE);
    }


    IMAGE inImg = {0};
    read_ppm_image(argv[1], &inImg);

    for (int r = 0; r < inImg.height; r++) {
        for (int c = 0; c < inImg.width; c++) {
            // get pixel at this pos
            RGBPIXEL* pxpt = get_rgbpixel(r,c,&inImg);

            // creates copy
            RGBPIXEL px = *pxpt;

            // calculates Y
            pxpt->r = 0.299*px.r + 0.587*px.g + 0.114*px.b;
            // calculates Cb
            pxpt->g = -0.1687*px.r - 0.3313*px.g + 0.5*px.b + 128;
            // calculates Cr
            pxpt->b = 0.5*px.r - 0.4187*px.g - 0.0813*px.b + 128;
                        
        }
    }

    // + 7 for "_XX.pgm"
    char* filepath = (char*)malloc(sizeof(char)*(strlen(argv[2]) + 1 + 7));

    // write Y.pgm file
    printf("Writing Y file...");
    filepath[0] = '\0';
    strcat(filepath, argv[2]);
    strcat(filepath, "_Y.pgm");

    write_pgm2pgm(filepath, &inImg);
    printf(" DONE\n");


    // write Cb.pgm file
    printf("Writing Cb file...");
    // moves green channel to the position of red, which is same position as grayscale value
    extr_gchan2img(&inImg, &inImg);

    filepath[0] = '\0';
    strcat(filepath, argv[2]);
    strcat(filepath, "_Cb.pgm");

    write_pgm2pgm(filepath, &inImg);
    printf(" DONE\n");


    // write Cr.pgm file
    printf("Writing Cr file...");
    extr_bchan2img(&inImg, &inImg);

    filepath[0] = '\0';
    strcat(filepath, argv[2]);
    strcat(filepath, "_Cr.pgm");

    write_pgm2pgm(filepath, &inImg);
    printf(" DONE\n");

    free_pxmat(inImg.mat, inImg.height);
    free(filepath);

    return 0;
}