#ifndef RAY_H
#define RAY_H 

#include "vec3.h"

typedef struct {
    Point3 origin; // Point3 is just an alias for Vector3
    Vector3 direction;
} Ray;

static inline Ray* ray_init(Point3* origin, Vector3* direction) {
    Ray* ray = (Ray*)malloc(sizeof(Ray));
    ray->origin = *origin;
    ray->direction = *direction;
    return ray;
} 

static inline Point3 ray_at(Ray* ray, double t) {
    // origin + direction * t
    Vector3* scaled_dir = vector_scalar_op(MULT, &ray->direction, t);
    return *vector_op(ADD, &ray->origin, scaled_dir);
}


#endif // RAY_H
