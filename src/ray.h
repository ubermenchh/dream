#ifndef RAY_H 
#define RAY_H
 
#include "vector.h" 

typedef struct {
    Point_t origin;
    Vector_t direction;
} Ray_t;

/* Initialize a ray */
static inline Ray_t Ray(Point_t origin, Vector_t direction) {
    Ray_t ray = {.origin = origin, .direction = direction};
    return ray;
}

/* Calculate the 3D position along the line. */
static inline Point_t ray_at(Ray_t ray, double t) {
    // P_t = A + tb
    return (Point_t)vector_add(ray.origin, vector_scalar_mult(ray.direction, t));
}

#endif // RAY_H
