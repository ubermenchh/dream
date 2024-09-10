#ifndef MATERIAL_H
#define MATERIAL_H 

#include "dream.h"

typedef struct Hit_Record Hit_Record;

typedef struct Material_t {
    bool (*scatter)(Material_t* self, Ray_t* ray_in, Hit_Record* rec, 
                    Color_t* attenuation, Ray_t* scattered);
} Material_t;

static inline bool default_scatter(Material_t* self, Ray_t* ray_in, Hit_Record* rec, 
                                   Color_t* attenuation, Ray_t* scattered) {
    return false;
}

static inline Material_t* Material(void) {
    Material_t* mat = (Material_t*)malloc(sizeof(Material_t));
    mat->scatter = default_scatter;
    return mat;
}

typedef struct Lambertian_t {
    Material_t base;
    Color_t albedo; // fractional reflectance (whtieness)
} Lambertian_t;

static inline bool lambertian_scatter(Material_t* mat, Ray_t* ray_in, Hit_Record* rec, 
                                      Color_t* attenuation, Ray_t* scattered) {
    Lambertian_t* lamb = (Lambertian_t*)mat;
    Vector_t scatter_direction = vector_add(rec->normal, random_unit_vector());

    // catch degenerate scatter direction 
    if (vector_near_zero(scatter_direction)) 
        scatter_direction = rec->normal;

    *scattered = Ray(rec->p, scatter_direction);
    *attenuation = lamb->albedo;
    return true;
}

static inline Lambertian_t* Lambertian(Color_t albedo) {
    Lambertian_t* lamb = (Lambertian_t*)malloc(sizeof(Lambertian_t));
    lamb->base.scatter = lambertian_scatter;
    lamb->albedo = albedo;
    return lamb;
}

typedef struct Metal_t {
    Material_t base;
    Color_t albedo;
    double fuzz;
} Metal_t;

static inline bool metal_scatter(Material_t* mat, Ray_t* ray_in, Hit_Record* rec, 
                                 Color_t* attenuation, Ray_t* scattered) {
    Metal_t* metal = (Metal_t*)mat;
    Vector_t reflected = vector_reflect(ray_in->direction, rec->normal);
    reflected = vector_add(unit_vector(reflected), 
                           vector_scalar_mult(random_unit_vector(), metal->fuzz));
    *scattered = Ray(rec->p, reflected);
    *attenuation = metal->albedo;
    return vector_dot(scattered->direction, rec->normal) > 0;
}

static inline Metal_t* Metal(Color_t albedo, double fuzz) {
    Metal_t* metal = (Metal_t*)malloc(sizeof(Metal_t));
    metal->base.scatter = metal_scatter;
    metal->albedo = albedo;
    metal->fuzz = fuzz < 1 ? fuzz : 1;
    return metal;
}

typedef struct Dielectric_t {
    Material_t base;
    
    double refraction_index;
    double (*reflectance)(double cosine, double refraction_index);
} Dielectric_t;

static inline double dielectric_reflectance(double cosine, double refraction_index) {
    // Schlick's approximation for reflectance 
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

static inline bool dielectric_scatter(Material_t* mat, Ray_t* ray_in, Hit_Record* rec, 
                                      Color_t* attenuation, Ray_t* scattered) {
    Dielectric_t* diel = (Dielectric_t*)mat;
    *attenuation = Color(1.0, 1.0, 1.0);
    double ri = rec->front_face ? (1.0 / diel->refraction_index) : diel->refraction_index;

    Vector_t unit_direction = unit_vector(ray_in->direction);
    Vector_t neg_unit_direction = vector_negate(unit_direction);

    double cos_theta = fmin(vector_dot(neg_unit_direction, rec->normal), 1.0);
    double sin_theta = sqrt(1.0 - (cos_theta*cos_theta));

    bool cannot_refract = ri * sin_theta > 1.0;
    Vector_t direction;

    if (cannot_refract || diel->reflectance(cos_theta, ri) > rand_double()) 
        direction = vector_reflect(unit_direction, rec->normal);
    else 
        direction = vector_refract(unit_direction, rec->normal, ri);

    *scattered = Ray(rec->p, direction);
    return true;
}

static inline Dielectric_t* Dielectric(double refraction_index) {
    Dielectric_t* diel = (Dielectric_t*)malloc(sizeof(Dielectric_t));
    diel->base.scatter = dielectric_scatter;
    diel->refraction_index = refraction_index;
    diel->reflectance = dielectric_reflectance;
    return diel;
}

#endif // MATERIAL_H
