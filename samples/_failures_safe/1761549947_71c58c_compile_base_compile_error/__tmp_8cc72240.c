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

void update_object_position(struct GeometricObject *obj, double time_step) {
    if (obj == NULL || !isfinite(time_step) || time_step <= 0.0) return;
    struct Vector3D displacement = vector_scale(&obj->velocity, time_step);
    obj->position = vector_add(&obj->position, &displacement);
}

int initialize_simulation(struct SimulationState *sim) {
    if (sim == NULL) return 0;
    sim->object_count = 2;
    sim->time_step = 0.1;
    sim->total_time = 0.0;
    
    sim->objects[0].position.x = 0.0;
    sim->objects[0].position.y = 0.0;
    sim->objects[0].position.z = 0.0;
    sim->objects[0].velocity.x = 1.0;
    sim->objects[0].velocity.y = 0.0;
    sim->objects[0].velocity.z = 0.0;
    sim->objects[0].mass = 1.0;
    sim->objects[0].radius = 0.5;
    
    sim->objects[1].position.x = 5.0;
    sim->objects[1].position.y = 0.0;
    sim->objects[1].position.z = 0.0;
    sim->objects[1].velocity.x = -0.5;
    sim->objects[1].velocity.y = 0.0;
    sim->objects[1].velocity.z = 0.0;
    sim->objects[1].mass = 2.0;
    sim->objects[1].radius = 0.8;
    
    for (size_t i = 0; i < sim->object_count; i++) {
        if