#ifndef RAY_H
#define RAY_H 

#include "vector.h"

typedef struct {
    Point* origin;
    Vector* direction;
} Ray;

static inline Ray* ray_init(Point* orig, Vector* dir) {
    Ray* ray = (Ray*)(sizeof(Ray));
    if (ray == NULL) return NULL;
    ray->origin = orig;
    ray->direction = dir;
    return ray;
}

static inline Point ray_at(Ray* ray, double t) {
    return (Point)vector_add(&ray->origin, vector_scalar_mul(ray->direction, t));
}

#endif // RAY_H
