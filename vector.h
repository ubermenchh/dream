#ifndef VECTOR_H
#define VECTOR_H 

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double x, y, z;
} Vector;

enum OPS { ADD, SUB, MUL, DIV, DOT, CROSS };
typedef Vector Point;

static inline Vector* vector_init(void) {
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    if (vector == NULL) return NULL;
    vector->x = 0; vector->y = 0; vector->z = 0;
    return vector;
}

static inline Vector* vector_negate(Vector* vector) {
    Vector* out = vector_init();
    out->x = -1 * vector->x;
    out->y = -1 * vector->y;
    out->z = -1 * vector->z;
    return out;
}


static inline Vector* vector_scalar_op(enum OPS op, Vector* vector, double v) {
    Vector* out = vector_init();

    switch (op) {
        case ADD:
            out->x = vector->x + v;
            out->y = vector->y + v;
            out->z = vector->z + v;
            break;
        case SUB:
            out->x = vector->x - v;
            out->y = vector->y - v;
            out->z = vector->z - v;
            break;
        case MUL:
            out->x = vector->x * v;
            out->y = vector->y * v;
            out->z = vector->z * v;
            break;
        case DIV:
            out->x = vector->x / v;
            out->y = vector->y / v;
            out->z = vector->z / v;
            break;
        default:
            printf("Invalid operation.");
            return NULL;
    }
    return out;
}

static inline Vector* vector_op(enum OPS op, Vector* v, Vector* w) {
    Vector* out = vector_init();

    switch (op) {
        case ADD:
            out->x = v->x + w->x;
            out->y = v->y + w->y;
            out->z = v->z + w->z;
            break;
        case SUB:
            out->x = v->x - w->x;
            out->y = v->y - w->y;
            out->z = v->z - w->z;
            break;
        case MUL:
            out->x = v->x * w->x;
            out->y = v->y * w->y;
            out->z = v->z * w->z;
            break;
        case DIV:
            out->x = v->x / w->x;
            out->y = v->y / w->y;
            out->z = v->z / w->z;
            break;
        case CROSS:
            out->x = v->y * w->z - v->z * w->y;
            out->y = v->z * w->x - v->x * w->z;
            out->z = v->x * w->y - v->y * w->x;
            break;
        default:
            printf("Invalid operation.");
            return NULL;
    }
    return out;
}

static inline Vector* vector_add(Vector* v, Vector* w) { return vector_op(ADD, v, w); }
static inline Vector* vector_sub(Vector* v, Vector* w) { return vector_op(SUB, v, w); }
static inline Vector* vector_mul(Vector* v, Vector* w) { return vector_op(MUL, v, w); }
static inline Vector* vector_div(Vector* v, Vector* w) { return vector_op(DIV, v, w); }
static inline Vector* vector_cross(Vector* v, Vector* w) { return vector_op(CROSS, v, w); }
static inline double vector_dot(Vector* v, Vector* w) {
    return (v->x * w->x) + (v->y * w->y) + (v->z * w->z);
}
static inline Vector* unit_vector(void) {
    Vector* out = vector_init();
    out->x = 1; out->y = 1; out->z = 1;
    return out;
}

static inline Vector* vector_scalar_add(Vector* v, double x) { return vector_scalar_op(ADD, v, x); }
static inline Vector* vector_scalar_sub(Vector* v, double x) { return vector_scalar_op(SUB, v, x); }
static inline Vector* vector_scalar_mul(Vector* v, double x) { return vector_scalar_op(MUL, v, x); }
static inline Vector* vector_scalar_div(Vector* v, double x) { return vector_scalar_op(DIV, v, x); }

static inline double vector_length_sq(Vector* vector) {
    return (vector->x * vector->x) + (vector->y * vector->y) + (vector->z * vector->z); 
}
static inline double vector_length(Vector* vector) { return sqrt(vector_length_sq(vector)); }

static inline void print_vector(Vector* vector) {
    printf("(%f %f %f)\n", vector->x, vector->y, vector->z);
}

#endif // VECTOR_H
