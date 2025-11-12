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

struct Vector3D matrix_vector_multiply(const struct Matrix3x3 *mat, const struct Vector3D *vec) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (mat == NULL || vec == NULL) return result;
    result.x = mat->elements[0][0] * vec->x + mat->elements[0][1] * vec->y + mat->elements[0][2] * vec->z;
    result.y = mat->elements[1][0] * vec->x + mat->elements[1][1] * vec->y + mat->elements[1][2] * vec->z;
    result.z = mat->elements[2][0] * vec->x + mat->elements[2][1] * vec->y + mat->elements[2][2] * vec->z;
    return result;
}

double vector_length(const struct Vector3D *vec) {
    if (vec == NULL) return 0.0;
    double sum = vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
    if (!isfinite(sum) || sum < 0.0) return 0.0;
    return sqrt(sum);
}

void simulate_step(struct SimulationState *state) {
    if (state == NULL || state->object_count == 0 || state->object_count > 10) return;
    if (!isfinite(state->time_step) || state->time_step <= 0.0) return;
    
    for (size_t i = 0; i < state->object_count; i++) {
        if (!validate_object(&state->objects[i])) continue;
        
        struct Vector3D acceleration = {0.0, -9.81, 0.0};
        struct Vector3D velocity_change = vector_scale(&acceleration, state->time_step);
        state->objects[i].velocity = vector_add(&state->objects[i].velocity, &velocity_change);
        
        struct Vector3D position_change = vector_scale(&state->objects[i].velocity, state->time_step);
        state->objects[i].position = vector_add(&state->objects[i].position, &position_change);
        
        if (state->objects[i].position.y < state->objects[i].radius) {
            state->objects[i].position.y = state->objects[i].radius;
            state->objects[i].velocity.y = -state->objects[i].velocity.y * 0.8;
        }
    }
}

void print_simulation_state(const struct SimulationState *state) {
    if (state == NULL) return;
    printf("Simulation State:\n");
    printf("Objects: %zu, Time Step: %.3f\n", state->object_count, state->time_step);
    for (size_t i = 0; i < state->object_count; i++) {
        printf("Object %zu: Pos(%.2f, %.2f, %.2f) Vel(%.2f, %.