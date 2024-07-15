#include <stdio.h>

#include "vec3.h"
#include "color.h"
#include "ray.h"

Color ray_color(Ray* r) {
    return *vector_init(0, 0, 0);
}

int main() {
    // Image 
    float aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate img_height
    int image_height = (int)(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height; // ensure its atleast 1 

    // Camera
    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (double)(image_width) / image_height;
    Point3 camera_center = *vector_init(0, 0, 0);

    // calculate the vectors across the horizontal and down the vertical viewport edges 
    Vector3* viewport_u = vector_init(viewport_width, 0, 0);
    Vector3* viewport_v = vector_init(0, -viewport_height, 0);

    // calculate the horizontal and vertical delta vectors from pixel to pixel 
    Vector3* pixel_delta_u = vector_scalar_op(DIV, viewport_u, image_width);
    Vector3* pixel_delta_v = vector_scalar_op(DIV, viewport_v, image_height);

    // calculate the location of the upper left pixel 
    Vector3* viewport_u_half = vector_scalar_op(DIV, viewport_u, 2); // viewport_u / 2
    Vector3* viewport_v_half = vector_scalar_op(DIV, viewport_v, 2); // viewport_v / 2
    // viewport_upper_left = camera_center - vector3(0, 0, focal_length) - viewport_u/2 - viewport_v/2
    Point3 viewport_upper_left = *vector_op(SUB, &camera_center, vector_init(0, 0, focal_length));
    viewport_upper_left = *vector_op(SUB, &viewport_upper_left, viewport_u_half);
    viewport_upper_left = *vector_op(SUB, &viewport_upper_left, viewport_v_half);

    // pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v)
    Point3 pixel_delta = *vector_scalar_op(MULT, vector_op(ADD, pixel_delta_u, pixel_delta_v), 0.5);
    Point3 pixel00_loc = *vector_op(ADD, &viewport_upper_left, &pixel_delta);

    // Render
    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (int j = 0; j < image_width; j++) {
        fprintf(stderr, "\rScanlines remaining: %d ", image_height - j);
        fflush(stderr);
        for (int i = 0; i < image_height; i++) {
            // pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v) 
            Vector3* i_pixel_delta_u = vector_scalar_op(MULT, pixel_delta_u, i);
            Vector3* j_pixel_delta_v = vector_scalar_op(MULT, pixel_delta_v, j);

            Point3 pixel_center = *vector_op(ADD, i_pixel_delta_u, j_pixel_delta_v);
            pixel_center = *vector_op(ADD, &pixel_center, &pixel00_loc);

            // ray_direction = pixel_center - camera_center
            Point3 ray_direction = *vector_op(SUB, &pixel_center, &camera_center);
            Ray* r = ray_init(&camera_center, &ray_direction);

            Color pixel_color = ray_color(r);
            write_color(stdout, &pixel_color);
        }
    }
    fprintf(stderr, "\rDone.                  \n");
    return 0;
}
