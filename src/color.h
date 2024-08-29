#ifndef COLOR_H 
#define COLOR_H 

#include "interval.h"
#include "vector.h" 

typedef Vector_t Color_t;

/* Initialize a Color */
static inline Color_t Color(double x, double y, double z) {
    Color_t out = {.x = x, .y = y, .z = z};
    return out;
}

static inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) 
        return sqrt(linear_component);
    return 0;
}

static inline void write_color(Color_t pixel_color) {
    double r = pixel_color.x;
    double g = pixel_color.y;
    double b = pixel_color.z;

    // apply a linear to gamma transform for gamma 2 
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // translate the [0, 1] component values to the byte range [0, 255]
    Interval_t intensity = Interval(0.000, 0.999);
    int rbyte = (int)(256 * interval_clamp(intensity, r));
    int gbyte = (int)(256 * interval_clamp(intensity, g));
    int bbyte = (int)(256 * interval_clamp(intensity, b));

    // write out the pixel color components 
    printf("%d %d %d\n", rbyte, gbyte, bbyte);
}

#endif // COLOR_H
