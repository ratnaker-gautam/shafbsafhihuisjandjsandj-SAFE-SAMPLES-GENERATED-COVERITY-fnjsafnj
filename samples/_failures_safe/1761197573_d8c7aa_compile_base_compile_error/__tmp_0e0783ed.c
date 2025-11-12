//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

struct Vector3D {
    double x;
    double y;
    double z;
};

struct Matrix3x3 {
    double elements[3][3];
};

struct LinearSystem {
    struct Matrix3x3 coefficients;
    struct Vector3D constants;
    struct Vector3D solution;
};

struct GeometryData {
    struct Vector3D points[4];
    double distances[6];
    double areas[4];
    double volume;
};

int validate_double(double value) {
    return !isnan(value) && !isinf(value);
}

int validate_vector3d(const struct Vector3D *vec) {
    return validate_double(vec->x) && validate_double(vec->y) && validate_double(vec->z);
}

struct Vector3D vector_add(const struct Vector3D *a, const struct Vector3D *b) {
    struct Vector3D result = {a->x + b->x, a->y + b->y, a->z + b->z};
    return result;
}

struct Vector3D vector_subtract(const struct Vector3D *a, const struct Vector3D *b) {
    struct Vector3D result = {a->x - b->x, a->y - b->y, a->z - b->z};
    return result;
}

double vector_dot(const struct Vector3D *a, const struct Vector3D *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

struct Vector3D vector_cross(const struct Vector3D *a, const struct Vector3D *b) {
    struct Vector3D result = {
        a->y * b->z - a->z * b->y,
        a->z * b->x - a->x * b->z,
        a->x * b->y - a->y * b->x
    };
    return result;
}

double vector_length(const struct Vector3D *vec) {
    return sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

double calculate_distance(const struct Vector3D *a, const struct Vector3D *b) {
    struct Vector3D diff = vector_subtract(a, b);
    return vector_length(&diff);
}

double calculate_triangle_area(const struct Vector3D *a, const struct Vector3D *b, const struct Vector3D *c) {
    struct Vector3D ab = vector_subtract(b, a);
    struct Vector3D ac = vector_subtract(c, a);
    struct Vector3D cross_product = vector_cross(&ab, &ac);
    return vector_length(&cross_product) / 2.0;
}

double calculate_tetrahedron_volume(const struct Vector3D *a, const struct Vector3D *b, const struct Vector3D *c, const struct Vector3D *d) {
    struct Vector3D ab = vector_subtract(b, a);
    struct Vector3D ac = vector_subtract(c, a);
    struct Vector3D ad = vector_subtract(d, a);
    struct Vector3D cross_product = vector_cross(&ab, &ac);
    double volume = fabs(vector_dot(&cross_product, &ad)) / 6.0;
    return volume;
}

int solve_linear_system(struct LinearSystem *system) {
    struct Matrix3x3 *mat = &system->coefficients;
    double det = mat->elements[0][0] * (mat->elements[1][1] * mat->elements[2][2] - mat->elements[1][2] * mat->elements[2][1])
               - mat->elements[0][1] * (mat->elements[1][0] * mat->elements[2][2] - mat->elements[1][2] * mat->elements[2][0])
               + mat->elements[0][2] * (mat->elements[1][0] * mat->elements[2][1] - mat->elements[1][1] * mat->elements[2][0]);
    
    if (fabs(det) < 1e-12) {
        return 0;
    }
    
    double inv_det = 1.0 / det;
    struct Matrix3x3 inv_mat;
    
    inv_mat.elements[0][0] = (mat->elements[1][1] * mat->elements[2][2] - mat->elements[1][2] * mat->elements[2][1]) * inv_det;
    inv_mat.elements[0][1] = (mat->elements[0][2] * mat->elements[2][1] - mat->elements[0][1] * mat->elements[2][2]) * inv_det;
    inv_mat.elements[0][2] = (mat->elements[0][1] * mat->elements[1][2] - mat->elements[0][2] * mat->elements[1][1]) * inv_det;
    inv_mat.elements[1][0] = (mat->elements[1][2] * mat->elements[2][0] - mat->elements[1][0] * mat->elements[2][2]) * inv_det;
    inv_mat.elements[1][1] = (mat->elements[0][0] * mat->elements[2][2] - mat