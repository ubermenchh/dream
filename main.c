#include <stdio.h> 

#include "vector.h"
#include "color.h"
#include "ray.h"

Color ray_color(Ray* r) {
    Vector* unit_direction = unit_vector(r->direction);
    double a = 0.5 * (unit_direction->y + 1.0);
    Color white = (Color){1.0, 1.0, 1.0};
    Color blue = (Color){0.5, 0.7, 1.0};
    Color final_color = *vector_add(
            vector_scalar_mul(&white, (1.0 - a)), 
            vector_scalar_mul(&blue, a)
    );

    free_vector(unit_direction);
    return final_color;
}


int main(void) {
    // Image 
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height and ensure that it's at least 1 
    int image_height = (int)(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera 
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)(image_width) / image_height);
    Point camera_center = (Point){0, 0, 0};
    
    // Calculatr the vectors across the horizontal and down the vertical viewport edges
    Vector viewport_u = (Vector){viewport_width, 0, 0};
    Vector viewport_v = (Vector){0, -viewport_height, 0};

    // Calculate the horizontal and vertical delta vectors from pixel to pixel 
    Vector* pixel_delta_u = vector_scalar_div(&viewport_u, image_width);
    Vector* pixel_delta_v = vector_scalar_div(&viewport_v, image_height);
    Vector* pixel_delta = vector_add(pixel_delta_u, pixel_delta_v);

    // Calculate the location of the upper left pixel 
    Vector* pixel_delta_u_half = vector_scalar_div(pixel_delta_u, 2.0); // pixel_delta_u / 2
    Vector* pixel_delta_v_half = vector_scalar_div(pixel_delta_v, 2.0); // pixel_delta_v / 2 
    Vector* pixel_delta_half = vector_add(pixel_delta_u_half, pixel_delta_v_half);

    Point viewport_upper_left = *vector_sub(&camera_center, &(Vector){0, 0, focal_length});
    viewport_upper_left = *vector_sub(&viewport_upper_left, pixel_delta_half);

    // pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v)
    Point pixel00_loc = *vector_add(&viewport_upper_left, vector_scalar_mul(pixel_delta, 0.5));

    // Render 
    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (int j = 0; j < image_height; j++) {
        fprintf(stderr, "\rScanlines remaining: %d\n", (image_height - j));
        fflush(stderr);
        for (int i = 0; i < image_width; i++) {
            Point pixel_center = *vector_add(&pixel00_loc, 
                    vector_add(vector_scalar_mul(pixel_delta_u, i),
                               vector_scalar_mul(pixel_delta_v, j))
            );
            Vector* ray_direction = vector_sub(&pixel_center, &camera_center);
            Ray* ray = ray_init(&camera_center, ray_direction);

            Color pixel_color = ray_color(ray);
            color_init(stdout, &pixel_color);
        }
    }
    fprintf(stderr, "\rDone.\n");
}
