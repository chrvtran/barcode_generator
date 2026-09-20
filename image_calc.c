#include "barcode.h"
#include "loader.h"
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv){
    // TODO: parse the arguments in argv. 
    // You can expect argv[1] to be the digits to encode
    // You can expect argv[2] to be the integer width
    // You can expect argv[3] to be the integer height
    // You can expect argv[4] to be the output filepath.
    char* digits = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    char* filepath = argv[4];

    if(argc != 5) {
        printf("Incorrect number of arguments. Expected: ./build/image_calc <barcode_value> <width> <height> <output_image_path>\n");
        return -1;
    }

    // TODO: call barcode
    struct image* img = barcode(digits, width, height);
    if (img == NULL) {
        printf("Error: can't malloc for\n");
        return -1;
    }
    
    // TODO: save result to file
    saveimage(filepath, img);

    // unallocate
    free(img->pixels);
    free(img);
}
