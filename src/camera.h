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
    
    double vfov;                // vertical view angle (field of view)
    Point_t lookfrom;           // point camera is looking from 
    Point_t lookat;             // point camera is looking at 
    Vector_t vup;               // camera-relative "up" direction

    double defocus_angle;       // variation angle of rays through each pixel 
    double focus_dist;          // distance from camera lookfrom point to plane of perfect focus
        
    int image_height;           // rendered image height 
    double pixel_samples_scale; // color scale factor for a sum of pixel samples
    Point_t center;             // camera center 
    Point_t pixel00_loc;        // location of pixel (0, 0) 
    Vector_t pixel_delta_u;     // offset to pixel to the height 
    Vector_t pixel_delta_v;     // offset to pixel below
    Vector_t u, v, w;           // camera frame basis vectors
    Vector_t defocus_disk_u;    // defocus disk horizontal radius 
    Vector_t defocus_disk_v;    // defocus disk vertical radius
} Camera_t;

static inline Camera_t Camera(double aspect_ratio, int image_width,
                              int samples_per_pixel, int max_depth, double vfov,
                              Point_t lookfrom, Point_t lookat, Vector_t vup, 
                              double defocus_angle, double focus_dist) {
    return (Camera_t){
        .aspect_ratio = aspect_ratio,
        .image_width = image_width,
        .samples_per_pixel = samples_per_pixel,
        .max_depth = max_depth,
        .vfov = vfov,
        .lookfrom = lookfrom,
        .lookat = lookat,
        .vup = vup,
        .defocus_angle = defocus_angle,
        .focus_dist = focus_dist
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

static inline Point_t defocus_disk_sample(Camera_t* cam) {
    // Returns a random point in the camera defocus disk 
    Vector_t p = random_vector_in_unit_disk();
    return vector_add(cam->center, 
                      vector_add(vector_scalar_mult(cam->defocus_disk_u, p.x), 
                                 vector_scalar_mult(cam->defocus_disk_v, p.y)));
}

static inline Ray_t get_ray(Camera_t* cam, int i, int j) {
    /* Construct a camera ray originating from the origin from the defocus disk 
       and directed at randomly sampled point around the pixel location i, j */

    Vector_t offset = sample_square();
    Point_t pixel_sample = vector_add(cam->pixel00_loc, 
                                      vector_add(vector_scalar_mult(cam->pixel_delta_u, 
                                                                   (i + offset.x)),        
                                                 vector_scalar_mult(cam->pixel_delta_v, 
                                                                   (j + offset.y))));
    Point_t ray_origin = (cam->defocus_angle <= 0) ? cam->center : defocus_disk_sample(cam);
    Vector_t ray_direction = vector_sub(pixel_sample, ray_origin);

    return Ray(ray_origin, ray_direction);
}

static inline void camera_init(Camera_t* cam) {
    cam->image_height = (int)(cam->image_width / cam->aspect_ratio);
    cam->image_height = (cam->image_height < 1) ? 1 : cam->image_height;
    
    cam->pixel_samples_scale = 1.0 / cam->samples_per_pixel;
    cam->center = cam->lookfrom;

    // determine viewport dimensions 
    double theta = degrees_to_radians(cam->vfov);
    double h = tan(theta / 2);
    double viewport_height = 2 * h * cam->focus_dist;
    double viewport_width = viewport_height * ((double)(cam->image_width) / cam->image_height); 

    // calculate the u, v, w unit basis vectors for the camera coordinate frame 
    cam->w = unit_vector(vector_sub(cam->lookfrom, cam->lookat));
    cam->u = unit_vector(vector_cross(cam->vup, cam->w));
    cam->v = vector_cross(cam->w, cam->u);

    // calculate the vectors across the horizontal and down the vertical viewport edges 
    Vector_t viewport_u = vector_scalar_mult(cam->u, viewport_width);
    Vector_t viewport_v = vector_scalar_mult(vector_negate(cam->v), viewport_height);

    // calculate the horizontal and vertical delta vectors from pixel to pixel 
    cam->pixel_delta_u = vector_scalar_div(viewport_u, (double)cam->image_width);  // viewport_u / image_width
    cam->pixel_delta_v = vector_scalar_div(viewport_v, (double)cam->image_height); // viewport_v / image_height

    // calculate the location of the upper left pixel
    // (viewport_u / 2) + (viewport_v / 2)
    Vector_t viewport = vector_add(vector_scalar_div(viewport_u, 2), vector_scalar_div(viewport_v, 2));
    // viewport_upper_left = camera_center - vector(0, 0, focal_length) - viewport
    Vector_t viewport_upper_left = vector_sub(vector_sub(cam->center, vector_scalar_mult(cam->w, cam->focus_dist)), viewport);
    // pixel_delta = pixel_delta_u + pixel_delta_v
    Vector_t pixel_delta = vector_add(cam->pixel_delta_u, cam->pixel_delta_v);
    // pixel00_loc = viewport_upper_left + 0.5 * pixel_delta
    cam->pixel00_loc = vector_add(viewport_upper_left, vector_scalar_mult(pixel_delta, 0.5));

    // calculate the camera defocus disk basis vectors 
    double defocus_radius = cam->focus_dist * tan(degrees_to_radians(cam->defocus_angle / 2));
    cam->defocus_disk_u = vector_scalar_mult(cam->u, defocus_radius);
    cam->defocus_disk_v = vector_scalar_mult(cam->v, defocus_radius);
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
