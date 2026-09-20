#include "barcode.h"
#include <stdio.h>


struct image* barcode(char* data, int width, int height) {
    // allocate image
    struct image* img = malloc(sizeof(struct image));
    if (img == NULL) {
        return NULL;
    }

    // set img and allocate pixels
    img->width = width;
    img->height = height;
    img->pixels = malloc(sizeof(struct pixel) * width * height);
    if (img->pixels == NULL) {
        free(img);
        return NULL;
    }

    // white canvas
    for (int i = 0; i < width * height; i++) {
        img->pixels[i].r = 255;
        img->pixels[i].g = 255;
        img->pixels[i].b = 255;
    }

    return img;
}

