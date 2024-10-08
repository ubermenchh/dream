#ifndef SPHERE_H 
#define SPHERE_H 

#include "hittable.h"

typedef struct Sphere_t {
    Hittable base;

    Point_t center;
    double radius;
    Material_t* mat;
} Sphere_t;


static inline bool hit_sphere(Hittable* hittable, Ray_t ray, Interval_t ray_t, Hit_Record* rec) {
    Sphere_t* sphere = (Sphere_t*)hittable;
    Vector_t oc = vector_sub(sphere->center, ray.origin);
    double a = vector_length_sq(ray.direction);
    double h = vector_dot(ray.direction, oc);
    double c = vector_length_sq(oc) - (sphere->radius * sphere->radius);

    double discriminant = h*h - a*c;
    if (discriminant < 0) 
        return false;

    double sqrt_d = sqrt(discriminant);
    
    // Find the nearest root that lies in the acceptable range.
    double root = (h - sqrt_d) / a;
    if (!interval_surrounds(ray_t, root)) {
        root = (h + sqrt_d) / a;
        if (!interval_surrounds(ray_t, root)) 
            return false;
    }
    rec->t = root;
    rec->p = ray_at(ray, rec->t);
    Vector_t outward_normal = vector_scalar_div(vector_sub(rec->p, sphere->center), sphere->radius); 
    set_face_normal(rec, ray, outward_normal);
    rec->mat = sphere->mat;

    return true;
}

static inline Sphere_t* Sphere(Point_t center, double radius, Material_t* mat) {
    Sphere_t* sphere = (Sphere_t*)malloc(sizeof(Sphere_t));
    sphere->base.hit = hit_sphere;
    sphere->center = center;
    sphere->radius = fmax(0, radius);
    sphere->mat = mat;
    return sphere;
}

static inline void free_sphere(Sphere_t* sphere) {
    free(sphere);
}

#endif // SPHERE_H
