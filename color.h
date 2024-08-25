#ifndef COLOR_H 
#define COLOR_H 

#include "vector.h" 

typedef Vector_t Color_t;

/* Initialize a Color */
static inline Color_t Color(double x, double y, double z) {
    Color_t out = {.x = x, .y = y, .z = z};
    return out;
}

static inline void write_color(Color_t pixel_color) {
    double r = pixel_color.x;
    double g = pixel_color.y;
    double b = pixel_color.z;

    // translate the [0, 1] component values to the byte range [0, 255]
    int rbyte = (int)(255.999 * r);
    int gbyte = (int)(255.999 * g);
    int bbyte = (int)(255.999 * b);

    // write out the pixel color components 
    printf("%d %d %d\n", rbyte, gbyte, bbyte);
}

#endif // COLOR_H
