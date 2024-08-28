#include "src/dream.h"
#include "src/camera.h"
#include "src/hittable.h"
#include "src/hittable_list.h"
#include "src/sphere.h"



int main() {
        // World 
    Hittable_List_t* world = Hittable_List();
    Sphere_t* sphere1 = Sphere(Vector(0, 0, -1), 0.5);
    Sphere_t* sphere2 = Sphere(Vector(0, -100.5, -1), 100);
    hittable_list_add(world, (Hittable*)sphere1);
    hittable_list_add(world, (Hittable*)sphere2);
    
    // aspect_ratio = 16/9, width = 400, samples_per_pixel = 100 
    Camera_t cam = Camera(16.0 / 9.0, 400, 100);
    camera_render(&cam, (Hittable*)world);
}
