#ifndef SPHERE_H 
#define SPHERE_H 

#include "hittable.h"
#include "vector.h" 

typedef struct {
    Point_t center;
    double radius;
} Sphere_t;

static inline Sphere_t Sphere(Point_t center, double radius) {
    Sphere_t out = {.center = center, .radius = radius};
    return out;
}

static inline bool hit_sphere(Sphere_t sphere, Ray_t ray, double ray_tmin, double ray_tmax, Hit_Record rec) {
    Vector_t oc = vector_sub(sphere.center, ray.origin);
    double a = vector_length_sq(ray.direction);
    double h = vector_dot(ray.direction, oc);
    double c = vector_length_sq(oc) - (sphere.radius * sphere.radius);

    double discriminant = h*h - a*c;
    if (discriminant < 0) 
        return false;

    double sqrtd = sqrt(discriminant);
    
    // Find the nearest root that lies in the acceptable range.
    double root = (h - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (h + sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) 
            return false;
    }
    rec.t = root;
    rec.p = ray_at(ray, rec.t);
    Vector_t outward_normal = vector_scalar_div(vector_sub(rec.p, sphere.center), sphere.radius); 
    set_face_normal(rec, ray, outward_normal);

    return true;
}

#endif 
