#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include "vec3.h" 

typedef Vector3 Color;

static inline void write_color(FILE* out, Color* pixel_color) {
    double r = pixel_color->x;
    double g = pixel_color->y;
    double b = pixel_color->z;

    int rbyte = (int)(255.999 * r);
    int gbyte = (int)(255.999 * g);
    int bbyte = (int)(255.999 * b);

    fprintf(out, "%d %d %d\n", rbyte, gbyte, bbyte);
}


#endif // COLOR_H
