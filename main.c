#include "src/dream.h"
#include "src/camera.h"
#include "src/hittable.h"
#include "src/hittable_list.h"
#include "src/material.h"
#include "src/sphere.h"


int main() {
    // World 
    Hittable_List_t* world = Hittable_List();
    
    Lambertian_t* material_ground = Lambertian(Color(0.8, 0.8, 0.0));
    Lambertian_t* material_center = Lambertian(Color(0.1, 0.2, 0.5));
    Dielectric_t* material_left = Dielectric(1.50);
    Dielectric_t* material_bubble = Dielectric(1.00 / 1.50);
    Metal_t* material_right = Metal(Color(0.8, 0.6, 0.2), 1.0);

    Sphere_t* sphere_ground = Sphere(Vector(0.0, -100.5, -1.0), 100.0, (Material_t*)material_ground);
    Sphere_t* sphere_center = Sphere(Vector(0.0, 0.0, -1.2), 0.5, (Material_t*)material_center);
    Sphere_t* sphere_left = Sphere(Vector(-1.0, 0.0, -1.0), 0.5, (Material_t*)material_left);
    Sphere_t* sphere_bubble = Sphere(Vector(-1.0, 0.0, -1.0), 0.4, (Material_t*)material_bubble);
    Sphere_t* sphere_right = Sphere(Vector(1.0, 0.0, -1.0), 0.5, (Material_t*)material_right);

    hittable_list_add(world, (Hittable*)sphere_ground);
    hittable_list_add(world, (Hittable*)sphere_center);
    hittable_list_add(world, (Hittable*)sphere_left);
    hittable_list_add(world, (Hittable*)sphere_bubble);
    hittable_list_add(world, (Hittable*)sphere_right);

    // aspect_ratio = 16/9, width = 500, samples_per_pixel = 100, max_depth = 50 
    Camera_t cam = Camera(16.0 / 9.0, 500, 100, 50);
    camera_render(&cam, (Hittable*)world);
}
