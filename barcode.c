#include "barcode.h"
#include <stdio.h>
#include <string.h>

// See Apppendix (appx.md)

// bit i (0 = leftmost) of an n-bit mask; returns 0 or 1
#define GET_BIT(mask, i, n) (((mask) >> ((n) - 1 - (i))) & 1)

static const int L_CODE['9' + 1] = {
    ['0'] = 0x72, // 1110010
    ['1'] = 0x66, // 1100110
    ['2'] = 0x6C, // 1101100
    ['3'] = 0x50, // 1010000
    ['4'] = 0x5C, // 1011100
    ['5'] = 0x4E, // 1001110
    ['6'] = 0x42, // 1000010
    ['7'] = 0x44, // 1000100
    ['8'] = 0x48, // 1001000
    ['9'] = 0x74  // 1110100
};
// inverse of L_CODE
#define R_CODE(c)    (~L_CODE[(int) (c)] & 0x7F)

#define MIDDLE_MASK  0x15 // 10101
#define START_MASK   0x2  // 010
#define END_MASK     0x2  // 010
#define WHITE_MASK   0x1  // 1

static void fill_digit(struct image* img, int start_x, int mask, int nbits) {
    for (int i = 0; i < nbits; i++) {
        int value = GET_BIT(mask, i, nbits) ? 255 : 0;
        for (int y = 0; y < img->height; y++) {
            struct pixel* pixel = &img->pixels[y * img->width + start_x + i];
            pixel->r = value;
            pixel->g = value;
            pixel->b = value;
        }
    }
}

struct image* barcode(char* data, int width, int height) {
    // VERIFYING INPUTS
    if (data == NULL || width < 113 || height <= 0) {
        return NULL;
    }

    int n = (int) strlen(data);
    if (n != 12) {
        // Barcode should have 12 digits
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        // Digits should be '0' to '9'
        if (data[i] < '0' || data[i] > '9') return NULL;
    }
 
    // ALLOCATING
    struct image* img = malloc(sizeof(struct image));
    if (img == NULL) {
        return NULL;
    }
    img->width = width;
    img->height = height;
    img->pixels = malloc(sizeof(struct pixel) * width * height);
    if (img->pixels == NULL) {
        free(img);
        return NULL;
    }

    // FILL STARTING QUIETZONE
    // 7*12 (digits) + 9*2 (quite zone) + 3 (start) + 3(end) + 5(middle) = 113 pixels wide
    int excess_width = width - 113;
    int x_pos = 0;

    for (; x_pos < excess_width/2 + 9; x_pos++) {
        fill_digit(img, x_pos, WHITE_MASK, 1);
    }

    // FILL START 3
    fill_digit(img, x_pos, START_MASK, 3);
    x_pos += 3;
 
    // FILL LEFT 6
    for (int i = 0; i < 6; i++) {
        fill_digit(img, x_pos, L_CODE[(int) data[i]], 7);
        x_pos += 7;
    }
 
    // FILL MIDDLE 5
    fill_digit(img, x_pos, MIDDLE_MASK, 5);
    x_pos += 5;

    // FILL RIGHT 6
    for (int i = 6; i < n; i++) {
        fill_digit(img, x_pos, R_CODE((int) data[i]), 7);
        x_pos += 7;
    }
 
    // FILL END 3
    fill_digit(img, x_pos, END_MASK, 3);
    x_pos += 3;

    // FILL ENDING QUIETZONE
    for (; x_pos < width; x_pos++) {
        fill_digit(img, x_pos, WHITE_MASK, 1);
    }

    return img;
}