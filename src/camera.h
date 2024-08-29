#ifndef CAMERA_H
#define CAMERA_H 

#include "dream.h"
#include "hittable.h"
#include "material.h"

typedef struct Camera {
    double aspect_ratio;        // ratio of image width over height 
    int image_width;            // rendered image width in pixel count 
    int samples_per_pixel;      // count of random samples for each pixel
    int max_depth;              // max number of ray bounces into scene
        
    int image_height;           // rendered image height 
    double pixel_samples_scale; // color scale factor for a sum of pixel samples
    Point_t center;             // camera center 
    Point_t pixel00_loc;        // location of pixel (0, 0) 
    Vector_t pixel_delta_u;     // offset to pixel to the height 
    Vector_t pixel_delta_v;     // offset to pixel below
} Camera_t;

static inline Camera_t Camera(double aspect_ratio, int image_width,
                              int samples_per_pixel, int max_depth) {
    return (Camera_t){
        .aspect_ratio = aspect_ratio,
        .image_width = image_width,
        .samples_per_pixel = samples_per_pixel,
        .max_depth = max_depth
    };
}

static inline Color_t ray_color(Ray_t ray, int depth, Hittable* world) {
    /* Equation for the gradient 
       blended_value = (1 - a) * start_value + a * end_value */

    if (depth <= 0)  // if we've exceeded the ray bounce limit, no more light is gathered
        return Color(0, 0, 0);

    Hit_Record rec;
    if (world->hit(world, ray, Interval(0.0001, INFINITY), &rec)) {
        Ray_t scattered;
        Color_t attenuation;
        Material_t* mat = (Material_t*)rec.mat;
        if (mat->scatter(mat, &ray, &rec, &attenuation, &scattered)) {
            return vector_mult(attenuation, ray_color(scattered, depth-1, world));
        }
        return Color(0, 0, 0);
    } 

    Vector_t unit_direction = unit_vector(ray.direction);
    double a = 0.5 * (unit_direction.y + 1.0);
    return vector_add(
                vector_scalar_mult(Color(1.0, 1.0, 1.0), (1.0 - a)),
                vector_scalar_mult(Color(0.5, 0.7, 1.0), a)
            );
}

static inline Vector_t sample_square() {
    // returns the vector to a random point in the [-.5, -.5]-[+.5, +.5] unit square 
    return Vector(rand_double() - 0.5, rand_double() - 0.5, 0);
}

static inline Ray_t get_ray(Camera_t* cam, int i, int j) {
    /* Construct a camera ray originating from the origin and directed at 
       randomly sampled point around the pixel location i, j */

    Vector_t offset = sample_square();
    Point_t pixel_sample = vector_add(cam->pixel00_loc, 
                                      vector_add(vector_scalar_mult(cam->pixel_delta_u, 
                                                                   (i + offset.x)),        
                                                 vector_scalar_mult(cam->pixel_delta_v, 
                                                                   (j + offset.y))));
    Point_t ray_origin = cam->center;
    Vector_t ray_direction = vector_sub(pixel_sample, ray_origin);

    return Ray(ray_origin, ray_direction);
}

static inline void camera_init(Camera_t* cam) {
    cam->image_height = (int)(cam->image_width / cam->aspect_ratio);
    cam->image_height = (cam->image_height < 1) ? 1 : cam->image_height;
    
    cam->pixel_samples_scale = 1.0 / cam->samples_per_pixel;
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
            Color_t pixel_color = Color(0, 0, 0);
            for (int sample = 0; sample < cam->samples_per_pixel; sample++) {
                Ray_t ray = get_ray(cam, i, j);
                // pixel_color += ray_color(ray, world)
                vector_add_(&pixel_color, (Vector_t)ray_color(ray, cam->max_depth, world));
            }
            write_color(vector_scalar_mult(pixel_color, cam->pixel_samples_scale));
        }
    }
}

#endif // CAMERA_H
