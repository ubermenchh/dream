#include "dream.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

// Equation for the gradient
// blended_value = (1 - a) * start_value + a * end_value
Color_t ray_color(Ray_t ray, Hittable* world) {
    Hit_Record rec;
    if (world->hit(world, ray, 0, INFINITY, rec)) {
        return vector_scalar_mult(vector_add(rec.normal, Color(1, 1, 1)), 0.5);
    } 

    Vector_t unit_direction = unit_vector(ray.direction);
    double a = 0.5 * (unit_direction.y + 1.0);
    return vector_add(
                vector_scalar_mult(Color(1.0, 1.0, 1.0), (1.0 - a)),
                vector_scalar_mult(Color(0.5, 0.7, 1.0), a)
            );
}

int main() {
    // Image 
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height and ensure that it's at least 1 
    int image_height = (int)(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // World 
    Hittable_List_t* world = Hittable_List();
    Sphere_t* sphere1 = Sphere((Point_t)Vector(0, 0, -1), 0.5);
    Sphere_t* sphere2 = Sphere((Point_t)Vector(0, -100.5, -1), 100);
    hittable_list_add(world, (Hittable*)sphere1);
    hittable_list_add(world, (Hittable*)sphere2);

    // Camera 
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double)(image_width) / image_height; 
    Point_t camera_center = (Point_t)Vector(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges 
    Vector_t viewport_u = Vector(viewport_width, 0, 0);
    Vector_t viewport_v = Vector(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel 
    Vector_t pixel_delta_u = vector_scalar_div(viewport_u, (double)image_width);
    Vector_t pixel_delta_v = vector_scalar_div(viewport_v, (double)image_height);

    // Calculate the location of the upper left pixel 
    // viewport = (viewport_u / 2) + (viewport_v / 2) 
    Vector_t viewport = vector_add(vector_scalar_div(viewport_u, 2), vector_scalar_div(viewport_v, 2));
    // viewport_upper_left = camera_center - vector(0, 0, focal_length) - viewport
    Vector_t viewport_upper_left = vector_sub(vector_sub(camera_center, Vector(0, 0, focal_length)), viewport);
    // pixel_delta = pixel_delta_u + pixel_delta_v
    Vector_t pixel_delta = vector_add(pixel_delta_u, pixel_delta_v);
    // pixel00_loc = viewport_upper_left + 0.5 * pixel_delta
    Vector_t pixel00_loc = vector_add(viewport_upper_left, vector_scalar_mult(pixel_delta, 0.5));

    // Render 
    printf("P3\n %d %d\n255\n", image_width, image_height);

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            Vector_t pixel_center = vector_add(pixel00_loc,
                                               vector_add(vector_scalar_mult(pixel_delta_u, i), 
                                                          vector_scalar_mult(pixel_delta_v, j)));
            Vector_t ray_direction = vector_sub(pixel_center, camera_center);
            Ray_t ray = Ray(camera_center, ray_direction);
            
            Color_t pixel_color = ray_color(ray, (Hittable*)world);
            write_color(pixel_color);
        }
    }
}
