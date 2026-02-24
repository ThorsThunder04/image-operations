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

int erode_px(int r, int c, IMAGE* destimg, IMAGE* srcimg, unsigned int radius) {

    int effective_errosions = 0;

    if (get_gpixel(r,c,srcimg)->v == 0) {
        for (int rr = r-radius; rr <= r+radius; rr++) {
            for (int cc = c-radius; cc <= c+radius; cc++) {
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


int dilate_px(int r, int c, IMAGE* destimg, IMAGE* srcimg, unsigned int radius){

    int effective_dilations = 0;

    if (get_gpixel(r,c,srcimg)->v == 255) {
        for (int rr = r-radius; rr <= r+radius; rr++) {
            for (int cc = c-radius; cc <= c+radius; cc++) {
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

/**
 * @brief Errodes an entire image with the given radius
 * 
 * @param destimg image to write the erosion to
 * @param srcimg the image to erode
 * @param radius how far to erode around each pixel
 */
void erode_img(IMAGE* destimg, IMAGE* srcimg, unsigned int radius) {

    // copies the current contents of srcimg into the destination
    copy_pxmat(
        destimg->mat,
        srcimg->mat,
        srcimg->width,
        srcimg->height
    );

    // apply the erosion to each individual pixel
    for (int r = 0; r < srcimg->height; r++) {
        for (int c = 0; c < srcimg->width; c++) {
            erode_px(r, c, destimg, srcimg, radius);
        }
    }
}

/**
 * @brief Dilates an entire image with the given radius
 * 
 * @param destimg image to write the dilation to
 * @param srcimg the image to dilate
 * @param radius how far to dilate around each pixel
 */
void dilate_img(IMAGE* destimg, IMAGE* srcimg, unsigned int radius) {
    // copies the current contents of srcimg into the destination
    copy_pxmat(
        destimg->mat,
        srcimg->mat,
        srcimg->width,
        srcimg->height
    );

    // apply the erosion to each individual pixel
    for (int r = 0; r < srcimg->height; r++) {
        for (int c = 0; c < srcimg->width; c++) {
            dilate_px(r, c, destimg, srcimg, radius);
        }
    }
}


void fermeture_img(IMAGE* destimg, IMAGE* srcimg, unsigned int radius) {

    // temporary image for holding the first dilation
    IMAGE tempimg;
    copy_img(&tempimg, srcimg);


    dilate_img(&tempimg, srcimg, radius);

    erode_img(destimg, &tempimg, radius);

    free_pxmat(tempimg.mat, tempimg.height);
}

void ouverture_img(IMAGE* destimg, IMAGE* srcimg, unsigned int radius) {

    // temporary image for holding the first erosion
    IMAGE tempimg;
    copy_img(&tempimg, srcimg);


    erode_img(&tempimg, srcimg, radius);

    dilate_img(destimg, &tempimg, radius);

    free_pxmat(tempimg.mat, tempimg.height);
}

int blur_gpx_cross(IMAGE* destimg, IMAGE* srcimg, int r, int c, unsigned int range) {

    if (!is_ib(r,c,destimg) || !is_ib(r,c,srcimg)) {
        return 0;
    }

    int n = 0;
    int sum = 0;

    // sum the rows of the c column
    for (int rr = r-range; rr <= r+range; rr++) {
        GPIXEL* px = get_gpixel(rr, c, srcimg);

        if (px != NULL) {
            sum += px->v;
            n++;
        }
    }

    // sum the columns of the r row
    for (int cc = c-range; cc <= c+range; cc++) {
        GPIXEL* px = get_gpixel(r, cc, srcimg);

        if (px != NULL) {
            sum += px->v;
            n++;
        }
    }

    // (r,c) already assured to be in bounds of destimg
    GPIXEL* destpx = get_gpixel(r,c,destimg);
    BYTE res = 0;
    // n is unlikely to be 0 anyway
    if (n > 0) {
        res = sum/n;
    } else {
        res = get_gpixel(r,c,srcimg)->v;
    }

    set_gpixel(destpx, res);

    return n;
}

int blur_rgbpx_cross(IMAGE* destimg, IMAGE* srcimg, int r, int c, unsigned int range) {

    if (!is_ib(r,c,destimg) || !is_ib(r,c,srcimg)) {
        return 0;
    }

    int n = 0;
    int sum[3] = {0};

    // sum the rows of the c column
    for (int rr = r-range; rr <= r+range; rr++) {
        RGBPIXEL* px = get_rgbpixel(rr, c, srcimg);

        if (px != NULL) {
            sum[0] += px->r;
            sum[1] += px->g;
            sum[2] += px->b;
            n++;
        }
    }

    // sum the columns of the r row
    for (int cc = c-range; cc <= c+range; cc++) {
        RGBPIXEL* px = get_rgbpixel(r, cc, srcimg);

        if (px != NULL) {
            sum[0] += px->r;
            sum[1] += px->g;
            sum[2] += px->b;
            n++;
        }
    }

    // (r,c) already assured to be in bounds of destimg
    RGBPIXEL* destpx = get_rgbpixel(r,c,destimg);
    // n is unlikely to be 0 anyway
    if (n > 0) {
        destpx->r = sum[0]/n;
        destpx->g = sum[1]/n;
        destpx->b = sum[2]/n;
        
    } else {
        *destpx = *get_rgbpixel(r,c,srcimg);
    }

    return n;
}


int blur_gpx_square(IMAGE* destimg, IMAGE* srcimg, int r, int c, unsigned int range) {

    if (!is_ib(r,c,destimg) || !is_ib(r,c,srcimg)) {
        return 0;
    }

    int n = 0;
    int sum = 0;

    for (int rr = r-range; rr <= r+range; rr++) {
        for (int cc = c-range; cc <= c+range; cc++) {
            GPIXEL* px = get_gpixel(rr,cc,srcimg);

            if (px != NULL) {
                sum += px->v;
                n++;
            }

        }
    }

    GPIXEL* destpx = get_gpixel(r,c,destimg);
    BYTE res = 0;

    if (n > 0) {
        res = sum/n;
    } else {
        res = get_gpixel(r,c,srcimg)->v;
    }

    set_gpixel(destpx, res);

    return n;
}

int blur_rgbpx_square(IMAGE* destimg, IMAGE* srcimg, int r, int c, unsigned int range) {

    if (!is_ib(r,c,destimg) || !is_ib(r,c,srcimg)) {
        return 0;
    }

    int n = 0;
    int sum[3] = {0};

    for (int rr = r-range; rr <= r+range; rr++) {
        for (int cc = c-range; cc <= c+range; cc++) {
            RGBPIXEL* px = get_rgbpixel(rr,cc,srcimg);

            if (px != NULL) {
                sum[0] += px->r;
                sum[1] += px->g;
                sum[2] += px->b;
                n++;
            }

        }
    }

    RGBPIXEL* destpx = get_rgbpixel(r,c,destimg);

    if (n > 0) {
        destpx->r = sum[0]/n;
        destpx->g = sum[1]/n;
        destpx->b = sum[2]/n;

    } else {
        *destpx = *get_rgbpixel(r,c,srcimg);
    }

    return n;
}

int apply_blur2img(
    int (*blur_func)(IMAGE*, IMAGE*, int, int, unsigned int),
    IMAGE* destimg,
    IMAGE* srcimg,
    unsigned int range
) {

    if (destimg->height != srcimg->height || destimg->width != srcimg->width) {
        return -1;
    }
    
    for (int r = 0; r < srcimg->height; r++) {
        for (int c = 0; c < srcimg->width; c++) {
            // no worry about return values because `apply_blur2img` already assures we're in bounds
            // out of bounds caused by the range are disregarded
            blur_func(destimg, srcimg,r,c,range);
        }
    }

    return 0;

}

int blur_img_rep(int (*blur_img_func)(IMAGE* , IMAGE*, unsigned int), IMAGE* destimg, IMAGE* srcimg, unsigned int range, int n_reps) {

    // create a temporary image for intermediate blurs
    IMAGE tempimg1 = {0};
    IMAGE tempimg2 = {0};

    copy_img(&tempimg1, srcimg);
    tempimg2 = tempimg1;
    PIXEL** newalloc = pxalloc(tempimg2.width, tempimg2.height);
    if (newalloc == NULL) {
        return -1;
    }
    tempimg2.mat = newalloc;


    for (int i = 0; i < n_reps; i++) {
        blur_img_func(&tempimg2, &tempimg1, range);

        // switch both structures around and their pointers for the next iteration
        IMAGE s = tempimg2;
        tempimg2 = tempimg1;
        tempimg1 = s;
        
    }

    // now that we have done all of the repetitive bluring, we copy resulting pixel matrix
    // values into the pixel matrix of the destination image
    // the reason we don't just move the pointer is because the destimg->mat pointer could be used elsewhere, and we don't want to overwrite it
    copy_pxmat(destimg->mat, tempimg1.mat, tempimg1.width, tempimg1.height);

    // free the allocated pixel matrixes for this function
    free_img_pxmat(&tempimg1);
    free_img_pxmat(&tempimg2);

    return 0;

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