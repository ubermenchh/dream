#include <stdio.h> 

#include "vector.h"
#include "color.h"

int main(void) {
    // Image 
    int image_width = 256;
    int image_height = 256;

    // Render 
    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (int j = 0; j < image_height; j++) {
        fprintf(stderr, "\rScanlines remaining: %d\n", (image_height - j));
        fflush(stderr);
        for (int i = 0; i < image_width; i++) {
            Color* pixel_color = (Color*)vector_init();
            pixel_color->x = (double)(i) / (image_width - 1);
            pixel_color->y = (double)(j) / (image_height - 1);
            pixel_color->z = 0;

            color_init(stdout, pixel_color); 
        }
    }
    fprintf(stderr, "\rDone.\n");
}
