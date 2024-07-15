#ifndef VEC3_H
#define VEC3_H 

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Vector3 { double x, y, z; } Vector3;
typedef Vector3 Point3;

enum UNARY_OPS { INIT, NEG, LEN, SQR_LEN };
enum BINARY_OPS { ADD, SUB, MULT, DIV, DOT, CROSS };
enum ASSIGN_OPS { ADD_AS, SUB_AS, MULT_AS, DIV_AS };

static inline Vector3* vector_init(double x, double y, double z) {
    Vector3* out = (Vector3*)malloc(sizeof(Vector3));
    if (out == NULL) return NULL;
    out->x = x; out->y = y; out->z=z;
    return out;
}

static inline Vector3* vector_negate(Vector3* vector) {
    Vector3* out;
    out->x *= -1; out->y *= -1; out->z *= -1;
    return out;
}

static inline double vector_len_sq(Vector3* vector) {
    return vector->x * vector->x + vector->y * vector->y + vector->z * vector->z;
}

static inline double vector_len(Vector3* vector) {
    return sqrt(vector_len_sq(vector));
}

static inline void vector_setall(Vector3* vector, double x, double y, double z) {
    vector->x = x; vector->y = y; vector->z = z;
}

static inline Vector3* vector_op(enum BINARY_OPS op, Vector3* v1, Vector3* v2) {
    Vector3* out;

    switch(op) {
        case ADD:
            out->x = v1->x + v2->x;
            out->y = v1->y + v2->y;
            out->z = v1->z + v2->z;
            break;
        case SUB:
            out->x = v1->x - v2->x;
            out->y = v1->y - v2->y;
            out->z = v1->z - v2->z;
            break;
        case MULT:
            out->x = v1->x * v2->x;
            out->y = v1->y * v2->y;
            out->z = v1->z * v2->z;
            break;
        case DIV:
            out->x = v1->x / v2->x;
            out->y = v1->y / v2->y;
            out->z = v1->z / v2->z;
            break;
        case CROSS:
            out->x = v1->y * v2->z - v1->z * v2->y;
            out->y = v1->z * v2->x - v1->x * v2->z;
            out->z = v1->x * v2->y - v1->y * v2->x;
            break;
        default:
            fprintf(stderr, "Invalid Operation!");
            break;
    }
    return out;
}

static inline void vector_assign_op(enum ASSIGN_OPS op, Vector3* vector, double value) {
    switch(op) {
        case ADD_AS:
            vector->x += value;
            vector->y += value;
            vector->z += value;
            break;
        case SUB_AS:
            vector->x -= value;
            vector->y -= value;
            vector->z -= value;
            break;
        case MULT_AS:
            vector->x *= value;
            vector->y *= value;
            vector->z *= value;
            break;
        case DIV_AS:
            vector->x /= value;
            vector->y /= value;
            vector->z /= value;
            break;
        default:
            fprintf(stderr, "Invalid Operation!");
            break;
    }
}

static inline Vector3* vector_scalar_op(enum BINARY_OPS op, Vector3* vector, double value) {
    Vector3* out;

    switch(op) {
        case ADD:
            out->x = vector->x * value;
            out->y = vector->y * value;
            out->z = vector->z * value;
            break;
        case SUB:
            out->x = vector->x - value;
            out->y = vector->y - value;
            out->z = vector->z - value;
            break;
        case MULT:
            out->x = vector->x * value;
            out->y = vector->y * value;
            out->z = vector->z * value;
            break;
        case DIV:
            out->x = vector->x / value;
            out->y = vector->y / value;
            out->z = vector->z / value;
            break; 
        default:
            fprintf(stderr, "Invalid Operation!");
            break;
    }
    return out;
}

static inline double vector_dot(Vector3* v1, Vector3* v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}
static inline Vector3* vector_cross(Vector3* v1, Vector3* v2) { return vector_op(CROSS, v1, v2); }
static inline Vector3* vector_add(Vector3* v1, Vector3* v2) { return vector_op(ADD, v1, v2); }
static inline Vector3* vector_sub(Vector3* v1, Vector3* v2) { return vector_op(SUB, v1, v2); }

static inline Vector3* unit_vector(Vector3* vector) { 
    return vector_scalar_op(DIV, vector, vector_len(vector)); 
}
static inline void print_vector(Vector3* vector) {
    printf("(%f %f %f)\n", vector->x, vector->y, vector->z);
}

#endif // VEC3_H
