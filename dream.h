#ifndef DREAM_H
#define DREAM_H 

#include <math.h> 
#include <stdio.h> 
#include <stdbool.h> 

// Constants 
#define PI (3.1415926535897932385)

// Utility Functions 
static inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

// Common Headers 
#include "color.h"
#include "ray.h"
#include "vector.h"

#endif // DREAM_H
