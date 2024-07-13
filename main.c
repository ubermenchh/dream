#include <stdio.h> 

int main() {
    // Image 
    int image_height = 256;
    int image_width = 256;

    // Render
    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (int j = 0; j < image_width; j++) {
        fprintf(stderr, "\rScanlines remaining: %d ", image_height - j);
        fflush(stderr);
        for (int i = 0; i < image_height; i++) {
            float r = (float)i / (image_height - 1);
            float g = (float)j / (image_width  - 1);
            float b = 0.0; 

            int ir = (int)(255.999 * r);
            int ig = (int)(255.999 * g);
            int ib = (int)(255.999 * b);

            printf("%d %d %d\n", ir, ig, ib);
        }
    }
    fprintf(stderr, "\rDone.                  \n");
    return 0;
}
