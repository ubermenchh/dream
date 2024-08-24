#include <stdio.h> 
#include <stdbool.h>

#include "vector.h"
#include "ray.h" 
#include "color.h"

/* Checks if a Point hits the Sphere or not */
double hit_sphere(Point_t center, double radius, Ray_t ray) {
    Vector_t oc = vector_sub(center, ray.origin);
    double a = vector_length_sq(ray.direction);
    double h = vector_dot(ray.direction, oc);
    double c = vector_length_sq(oc) - radius*radius;
    double discriminant = h*h - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (h - sqrt(discriminant)) / a;
    } 
} 

// Equation for the gradient
// blended_value = (1 - a) * start_value + a * end_value
Color_t ray_color(Ray_t ray) {
    double t = hit_sphere((Point_t)Vector(0, 0, -1), 0.5, ray);
    if (t > 0.0) {
        Vector_t N = unit_vector(vector_sub(ray_at(ray, t), Vector(0, 0, -1)));
        return vector_scalar_mult(Vector(N.x + 1, N.y + 1, N.z + 1), 0.5);
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
            
            Color_t pixel_color = ray_color(ray);
            write_color(pixel_color);
        }
    }
}
