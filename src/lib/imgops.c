#include <stdio.h>
#include <stdlib.h>
#include "imgio.h"
#include "imgops.h"



int is_ib(int r, int c, IMAGE* img) {
    return (r < img->height && r >= 0 && c < img->width && c >= 0);
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



int convert_channel_img(IMAGE* destimg, IMAGE* srcimg, CONVTYPE conv) {

    // we do the `-1`s here because this function's range parameters are inclusive
    // so it WILL try and access the value at index `img.mat[r2][c2]`. Though this function has bound checks
    // so if we just pass in the the actual sizes of srcimg instead of indexes, we will repeatedly get
    // `-2` or `-3` return values
    return convert_channel_img_range(destimg, srcimg, conv, 0, 0, srcimg->height-1, srcimg->width-1);

}



int convert_channel_img_range(IMAGE* destimg, IMAGE* srcimg, CONVTYPE conv, int r1, int c1, int r2, int c2) {

    // catch when an untreaded `conv` value is used (the value isn't in the enum)
    if (conv < 0 || conv >= _CONVEND) {
        return -1;
    }

    // this means that we would be writing to pixels that don't exist in `destimg` (=> seg fault)
    if (!(is_ib(r1, c1, destimg) && is_ib(r2, c2, destimg))) {
        return -2;
    }

    // this means that we would be reading from pixels that don't exist in `srcimg` (=> seg fault)
    if (!(is_ib(r1, c1, srcimg) && is_ib(r2, c2, srcimg))) {
            return -3;
    }

    for (int r = r1; r <= r2; r++) {
        for (int c = c1; c <= c2; c++) {
            
            // we have already checked the bounds, so we don't need to worry about
            // the return values of the get_pixel functions here
            convert_channel_px(
                get_pixel(r, c, destimg),
                get_pixel(r, c, srcimg),
                conv
            );

        }
    }

    return 0;

}

void bin_gthresh_img(IMAGE* img, int thresh, BYTE underv, BYTE abovev) {

    for (int r = 0; r < img->height; r++) {
        for (int c = 0; c < img->width; c++) {

            GPIXEL* px = get_gpixel(r,c,img);

            BYTE thresh_res = (thresh < px->v) ? abovev : underv;

            set_gpixel(px, thresh_res);
        }
    }
}

void bin_rgbthresh_img(IMAGE* img, int rthresh, int gthresh, int bthresh, BYTE underv, BYTE abovev) {
    
    for (int r = 0; r < img->height; r++) {
        for (int c = 0; c < img->width; c++) {
            RGBPIXEL* px = get_rgbpixel(r,c,img);


            px->r = (px->r < rthresh) ? underv : abovev;
            px->g = (px->g < gthresh) ? underv : abovev;
            px->b = (px->b < bthresh) ? underv : abovev;
        }
    }

}

int erode_px(int r, int c, IMAGE* destimg, IMAGE* srcimg, unsigned int range) {

    int effective_errosions = 0;

    if (get_gpixel(r,c,srcimg)->v == 0) {
        for (int rr = r-range; rr <= r+range; rr++) {
            for (int cc = c-range; cc <= c+range; cc++) {
                GPIXEL* px = get_gpixel(rr,cc,destimg);

                if (px != NULL) {
                    px->v = 0;
                    effective_errosions++;
                }
            } 
        }
    }

    return effective_errosions;
}


int dilate_px(int r, int c, IMAGE* destimg, IMAGE* srcimg, unsigned int range){

    int effective_dilations = 0;

    if (get_gpixel(r,c,srcimg)->v == 255) {
        for (int rr = r-range; rr <= r+range; rr++) {
            for (int cc = c-range; cc <= c+range; cc++) {
                GPIXEL* px = get_gpixel(rr,cc,destimg);

                if (px != NULL) {
                    px->v = 255;
                    effective_dilations++;
                }
            } 
        }
    }

    return effective_dilations;
}

// This function will remain at the end as I think it will be the longest one
int convert_channel_px(PIXEL* destpx, PIXEL* srcpx, CONVTYPE conv) {
    // doing all of this in a single function with an ENUM is a lot more practical then
    // writing a LOAD of different functions for each type of color channel conversion

    // the provided conversion type hasn't been defined in the ENUM of conversion types
    if (conv < 0 || conv >= _CONVEND) {
        return -1;
    }
    // this could have been done with a default case in the switch, but I prefer this


    // srcpx is copied in case `destpx` == `srcpx`
    // allowing them to be equal means that we don't need to also make a conversion function for inplace pixel conversion
    RGBPIXEL pxcpy = srcpx->cpx; // directly taking RGBPIXEL as it contains all values we could want


    switch (conv) {

        // MISC types

        case GRAY2RGB:
        case RED2RGB: // they're equivalent
            set_rgbpixel(
                &srcpx->cpx,
                pxcpy.r,
                pxcpy.r,
                pxcpy.r
            );
            break;
        case GREEN2RGB:
            set_rgbpixel(
                &srcpx->cpx,
                pxcpy.g,
                pxcpy.g,
                pxcpy.g
            );
            break;
        case BLUE2RGB:
            set_rgbpixel(
                &srcpx->cpx,
                pxcpy.b,
                pxcpy.b,
                pxcpy.b
            );
            break;
        
        // from RGB
        case RGB2GRAY:
            BYTE luma = 0.299*pxcpy.r + 0.587*pxcpy.g + 0.114*pxcpy.b;
            set_rgbpixel(
                &srcpx->cpx,
                luma,luma,luma
            );
            break;

        // from YUV

        // from YCbCr

        // from HSV

    }

    return 0;
}