#include "src/dream.h"
#include "src/camera.h"
#include "src/hittable.h"
#include "src/hittable_list.h"
#include "src/material.h"
#include "src/sphere.h"


int main() {
    Hittable_List_t* world = Hittable_List();
    
    Lambertian_t* ground_material = Lambertian(Color(0.5, 0.5, 0.5));
    Sphere_t* sphere_material = Sphere(Vector(0, -1000, 0), 1000, (Material_t*)ground_material);
    hittable_list_add(world, (Hittable*)sphere_material);

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = rand_double();
            Point_t center = (Point_t)Vector(a + 0.9*rand_double(), 0.2, b + 0.9*rand_double());

            if (vector_length(vector_sub(center, Vector(4, 0.2, 0))) > 0.9) {
                Material_t* sphere_material = Material();

                if (choose_mat < 0.8) {
                    // diffuse 
                    Color_t albedo = (Color_t)vector_mult(rand_vector(), rand_vector());
                    sphere_material = (Material_t*)Lambertian(albedo);
                    hittable_list_add(world, (Hittable*)Sphere(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal 
                    Color_t albedo = (Color_t)random_vector(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = (Material_t*)Metal(albedo, fuzz);
                    hittable_list_add(world, (Hittable*)Sphere(center, 0.2, sphere_material));
                } else {
                    // glass 
                    sphere_material = (Material_t*)Dielectric(1.5);
                    hittable_list_add(world, (Hittable*)Sphere(center, 0.2, sphere_material));
                }
            }
        }
    }

    Material_t* mat1 = (Material_t*)Dielectric(1.5);
    hittable_list_add(world, (Hittable*)Sphere(Vector(0, 1, 0), 1.0, mat1));

    Material_t* mat2 = (Material_t*)Lambertian((Color_t)Vector(0.4, 0.2, 0.1));
    hittable_list_add(world, (Hittable*)Sphere(Vector(-4, 1, 0), 1.0, mat2));

    Material_t* mat3 = (Material_t*)Metal((Color_t)Vector(0.7, 0.6, 0.5), 0.0);
    hittable_list_add(world, (Hittable*)Sphere(Vector(4, 1, 0), 1.0, mat3));

    Camera_t cam = Camera(16.0/9.0,            // aspect ratio 
                          500,                // image_width
                          100,                 // samples_per_pixel 
                          50,                  // max_depth 
                          20,                  // vfov 
                          Vector(13, 2, 3),    // lookfrom 
                          Vector(0, 0, 0),     // lookat 
                          Vector(0, 1, 0),     // vup 
                          0.6,                 // defocus_angle 
                          10.0);               // focus_dist

    camera_render(&cam, (Hittable*)world);
}
