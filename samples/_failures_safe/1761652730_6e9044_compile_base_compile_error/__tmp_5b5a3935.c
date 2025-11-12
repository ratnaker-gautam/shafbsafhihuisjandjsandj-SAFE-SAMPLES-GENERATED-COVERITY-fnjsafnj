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

struct GeometricObject {
    struct Vector3D position;
    struct Vector3D velocity;
    double mass;
    double radius;
};

struct SimulationState {
    struct GeometricObject objects[10];
    size_t object_count;
    double time_step;
    double total_time;
};

int validate_vector(const struct Vector3D *vec) {
    if (vec == NULL) return 0;
    if (!isfinite(vec->x) || !isfinite(vec->y) || !isfinite(vec->z)) return 0;
    return 1;
}

int validate_matrix(const struct Matrix3x3 *mat) {
    if (mat == NULL) return 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!isfinite(mat->elements[i][j])) return 0;
        }
    }
    return 1;
}

int validate_object(const struct GeometricObject *obj) {
    if (obj == NULL) return 0;
    if (!validate_vector(&obj->position)) return 0;
    if (!validate_vector(&obj->velocity)) return 0;
    if (!isfinite(obj->mass) || obj->mass <= 0.0) return 0;
    if (!isfinite(obj->radius) || obj->radius <= 0.0) return 0;
    return 1;
}

struct Vector3D vector_add(const struct Vector3D *a, const struct Vector3D *b) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (a == NULL || b == NULL) return result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

struct Vector3D vector_scale(const struct Vector3D *vec, double scalar) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (vec == NULL || !isfinite(scalar)) return result;
    result.x = vec->x * scalar;
    result.y = vec->y * scalar;
    result.z = vec->z * scalar;
    return result;
}

double vector_dot(const struct Vector3D *a, const struct Vector3D *b) {
    if (a == NULL || b == NULL) return 0.0;
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

struct Vector3D matrix_vector_multiply(const struct Matrix3x3 *mat, const struct Vector3D *vec) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (mat == NULL || vec == NULL) return result;
    result.x = mat->elements[0][0] * vec->x + mat->elements[0][1] * vec->y + mat->elements[0][2] * vec->z;
    result.y = mat->elements[1][0] * vec->x + mat->elements[1][1] * vec->y + mat->elements[1][2] * vec->z;
    result.z = mat->elements[2][0] * vec->x + mat->elements[2][1] * vec->y + mat->elements[2][2] * vec->z;
    return result;
}

struct Matrix3x3 create_rotation_matrix(double angle, const struct Vector3D *axis) {
    struct Matrix3x3 result = {{{0.0}}};
    if (axis == NULL || !isfinite(angle)) return result;
    if (!validate_vector(axis)) return result;
    double norm = sqrt(vector_dot(axis, axis));
    if (norm <= 0.0) return result;
    double ux = axis->x / norm;
    double uy = axis->y / norm;
    double uz = axis->z / norm;
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    double one_minus_cos = 1.0 - cos_a;
    result.elements[0][0] = cos_a + ux * ux * one_minus_cos;
    result.elements[0][1] = ux * uy * one_minus_cos - uz * sin_a;
    result.elements[0][2] = ux * uz * one_minus_cos + uy * sin_a;
    result.elements[1][0] = uy * ux * one_minus_cos + uz * sin_a;
    result.elements[1][1] = cos_a + uy * uy * one_minus_cos;
    result.elements[1][2] = uy * uz * one_minus_cos - ux * sin_a;
    result.elements[2][0] = uz * ux * one_minus_cos - uy * sin_a;
    result.elements[2][1] = uz * uy * one_minus_cos + ux * sin_a;
    result.elements[2][2] = cos_a + uz * uz * one_minus_cos