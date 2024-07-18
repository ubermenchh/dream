#ifndef COLOR_H
#define COLOR_H 

#include <stdio.h> 
#include "vector.h"

typedef Vector Color;

static inline void color_init(FILE* out, const Color* pixel_color) {
    double r = pixel_color->x;
    double g = pixel_color->y;
    double b = pixel_color->z;

    // Translate the [0, 1] component values to the byte range [0, 255]
    int rbyte = (int)(255.999 * r);
    int gbyte = (int)(255.999 * g);
    int bbyte = (int)(255.999 * b);

    // Write out the pixel color components
    fprintf(out, "%d %d %d\n", rbyte, gbyte, bbyte);
}

#endif // COLOR_H
