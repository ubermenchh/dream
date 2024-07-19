#ifndef RAY_H
#define RAY_H 

#include "vector.h"

typedef struct {
    Point* origin;
    Vector* direction;
} Ray;

static inline Ray* ray_init(Point* orig, Vector* dir) {
    Ray* ray = (Ray*)malloc(sizeof(Ray));
    if (ray == NULL) return NULL;
    ray->origin = orig;
    ray->direction = dir;
    return ray;
}

static inline void free_ray(Ray* ray) {
    if (ray == NULL) return;
    if (ray->origin != NULL) free_vector(ray->origin);
    if (ray->direction != NULL) free_vector(ray->direction);
    free(ray);
}

static inline Point ray_at(Ray* ray, double t) {
    return *vector_add(ray->origin, vector_scalar_mul(ray->direction, t));
}

#endif // RAY_H
