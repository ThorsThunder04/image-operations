#include <stdio.h>
#include <stdlib.h>
#include "imgio.h"
#include "imgops.h"



int is_ib(int r, int c, IMAGE* img) {
    return (r < img->height && r >= 0 && c < img->width && c >= 0);
}


RGBPIXEL g2rgb(GPIXEL* px) {
    return (RGBPIXEL){px->v, px->v, px->v};
}

GPIXEL rgb2g(RGBPIXEL* px) {
    GPIXEL gpx = {0};

    // calculates floored average of px
    gpx.v += px->r/3;
    gpx.v += px->g/3;
    gpx.v += px->b/3;

    return gpx;

}

PIXEL* get_pixel(int r, int c, IMAGE* img) {
    if (!is_ib(r,c,img)) {
        return NULL;
    }

    return &img->mat[r][c];
}

PIXEL* set_pixel(int r, int c, PIXEL* px, IMAGE* img) {

    if (!is_ib(r,c,img)) {
        return NULL;
    }

    PIXEL* pixloc = &img->mat[r][c];

    *pixloc = *px;

    return pixloc;

}

void set_rgbpixel(RGBPIXEL* px, BYTE R, BYTE G, BYTE B)  {


    //? handle error here ?
    if (px != NULL) {
        px->r = R;
        px->g = G;
        px->b = B;
    }

}

void set_gpixel(GPIXEL* px, BYTE V) {

    //? handle error here ?
    if (px != NULL) {
        px->v = V;
    }
}

RGBPIXEL* get_rgbpixel(int r, int c, IMAGE* img) {
    if (!is_ib(r,c,img)) {
        return NULL;
    }

    return &get_pixel(r,c,img)->cpx;
}

GPIXEL* get_gpixel(int r, int c, IMAGE* img) {

    if (!is_ib(r,c,img)) {
        return NULL;
    }

    return &get_pixel(r,c,img)->gpx;
}

void apply_chan2img(void (*op)(PIXEL*), IMAGE* img) {
    
    for (int r = 0; r < img->height; r++) {
        for (int c = 0; c < img->width; c++) {
            op(get_pixel(r,c,&img));
        }
    }
}

void rgb2yuv(PIXEL* px);

void rgb2ycbcr(PIXEL* px);

void rgb2luma(PIXEL* px);

void rgb2yuvimg(IMAGE* img);

void rgb2ycbcrimg(IMAGE* img);

void rgb2lumaimg(IMAGE* img);