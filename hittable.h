#ifndef HITTABLE_H 
#define HITTABLE_H 

#include "ray.h"

typedef struct {
    Point_t p;
    Vector_t normal;
    double t;
    bool front_face;
} Hit_Record;

typedef struct Hittable {
    bool (*hit)(Ray_t ray, double ray_tmin, double ray_tmax, Hit_Record rec);
} Hittable;

static inline void set_face_normal(Hit_Record hr, Ray_t ray, Vector_t outward_normal) {
    // sets the hit record normal vector.
    // NOTE: `outward_normal` is assumed to have unit length.
    bool front_face = vector_dot(ray.direction, outward_normal) < 0;
    hr.normal = front_face ? outward_normal : vector_negate(outward_normal);
}

#endif
