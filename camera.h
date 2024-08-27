#ifndef CAMERA_H
#define CAMERA_H 

#include "dream.h"
#include "hittable.h"
#include "hittable_list.h"

typedef struct Camera {
    double aspect_ratio; // ratio of image width over height 
    int image_width;     // rendered image width in pixel count 
    int image_height;          // rendered image height 
    Point_t center;            // camera center 
    Point_t pixel00_loc;       // location of pixel (0, 0) 
    Vector_t pixel_delta_u;    // offset to pixel to the height 
    Vector_t pixel_delta_v;    // offset to pixel below
} Camera_t;

static inline Camera_t Camera(double aspect_ratio, int image_width) {
    Camera_t cam;
    cam.aspect_ratio = aspect_ratio;
    cam.image_width = image_width;
    return cam;
}

// Equation for the gradient
// blended_value = (1 - a) * start_value + a * end_value
static inline Color_t ray_color(Ray_t ray, Hittable* world) {
    Hit_Record rec;
    if (world->hit(world, ray, Interval(0, INFINITY), &rec)) {
        return vector_scalar_mult(vector_add(rec.normal, Color(1, 1, 1)), 0.5);
    } 

    Vector_t unit_direction = unit_vector(ray.direction);
    double a = 0.5 * (unit_direction.y + 1.0);
    return vector_add(
                vector_scalar_mult(Color(1.0, 1.0, 1.0), (1.0 - a)),
                vector_scalar_mult(Color(0.5, 0.7, 1.0), a)
            );
}

static inline void camera_init(Camera_t* cam) {
    cam->image_height = (int)(cam->image_width / cam->aspect_ratio);
    cam->image_height = (cam->image_height < 1) ? 1 : cam->image_height;

    cam->center = (Point_t)Vector(0, 0, 0);

    // determine viewport dimensions 
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)(cam->image_width) / cam->image_height); 

    // calculate the vectors across the horizontal and down the vertical viewport edges 
    Vector_t viewport_u = Vector(viewport_width, 0, 0);
    Vector_t viewport_v = Vector(0, -viewport_height, 0);

    // calculate the horizontal and vertical delta vectors from pixel to pixel 
    cam->pixel_delta_u = vector_scalar_div(viewport_u, (double)cam->image_width);  // viewport_u / image_width
    cam->pixel_delta_v = vector_scalar_div(viewport_v, (double)cam->image_height); // viewport_v / image_height

    // calculate the location of the upper left pixel
    // (viewport_u / 2) + (viewport_v / 2)
    Vector_t viewport = vector_add(vector_scalar_div(viewport_u, 2), vector_scalar_div(viewport_v, 2));
    // viewport_upper_left = camera_center - vector(0, 0, focal_length) - viewport
    Vector_t viewport_upper_left = vector_sub(vector_sub(cam->center, Vector(0, 0, focal_length)),viewport);
    // pixel_delta = pixel_delta_u + pixel_delta_v
    Vector_t pixel_delta = vector_add(cam->pixel_delta_u, cam->pixel_delta_v);
    // pixel00_loc = viewport_upper_left + 0.5 * pixel_delta
    cam->pixel00_loc = vector_add(viewport_upper_left, vector_scalar_mult(pixel_delta, 0.5));
}

static inline void camera_render(Camera_t* cam, Hittable* world) {
    camera_init(cam);

    printf("P3\n %d %d\n255\n", cam->image_width, cam->image_height);
    for (int j = 0; j < cam->image_height; j++) {
        for (int i = 0; i < cam->image_width; i++) {
            Vector_t pixel_center = vector_add(cam->pixel00_loc, 
                                               vector_add(vector_scalar_mult(cam->pixel_delta_u, i), 
                                                          vector_scalar_mult(cam->pixel_delta_v, j)));
            Vector_t ray_direction = vector_sub(pixel_center, cam->center);
            Ray_t ray = Ray(cam->center, ray_direction);

            Color_t pixel_color = ray_color(ray, world);
            write_color(pixel_color);
        }
    }
}

#endif // CAMERA_H
