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
    int32_t id;
};

struct SimulationState {
    struct GeometricObject *objects;
    size_t count;
    double time_step;
    uint32_t step_count;
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
    if (obj->id < 0) return 0;
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

struct Vector3D matrix_vector_multiply(const struct Matrix3x3 *mat, const struct Vector3D *vec) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (mat == NULL || vec == NULL) return result;
    result.x = mat->elements[0][0] * vec->x + mat->elements[0][1] * vec->y + mat->elements[0][2] * vec->z;
    result.y = mat->elements[1][0] * vec->x + mat->elements[1][1] * vec->y + mat->elements[1][2] * vec->z;
    result.z = mat->elements[2][0] * vec->x + mat->elements[2][1] * vec->y + mat->elements[2][2] * vec->z;
    return result;
}

void update_object(struct GeometricObject *obj, const struct Matrix3x3 *transform, double time_step) {
    if (obj == NULL || transform == NULL || !isfinite(time_step) || time_step <= 0.0) return;
    struct Vector3D acceleration = matrix_vector_multiply(transform, &obj->position);
    struct Vector3D velocity_delta = vector_scale(&acceleration, time_step);
    obj->velocity = vector_add(&obj->velocity, &velocity_delta);
    struct Vector3D position_delta = vector_scale(&obj->velocity, time_step);
    obj->position = vector_add(&obj->position, &position_delta);
}

int initialize_simulation(struct SimulationState *sim, size_t object_count, double time_step) {
    if (sim == NULL || object_count == 0 || object_count > 1000 || !isfinite(time_step) || time_step <= 0.0) return 0;
    sim->objects = malloc(object_count * sizeof(struct GeometricObject));
    if (sim->objects == NULL) return 0;
    sim->count = object_count;
    sim->time_step = time_step;
    sim->step_count = 0;
    for (size_t i = 0; i < object_count; i++) {
        sim->objects[i].position.x = (double)(i + 1) * 0.5;
        sim->objects[i].position.y = (double)(i + 1) * 0.3;
        sim->objects[i].position.z = (double)(i + 1) * 0.7;
        sim->objects[i].velocity.x = 0.1;
        sim->objects[i].velocity.y = 0.2;
        sim->objects[i].velocity.z = 0.3;
        sim->objects[i].mass = 1.0 + (double)i * 0.1;
        sim->objects[i].id = (int32_t)i;
    }
    return 1;
}

void run_simulation_step(struct SimulationState *sim, const struct Matrix3x3 *transform) {
    if (sim == NULL || transform == NULL