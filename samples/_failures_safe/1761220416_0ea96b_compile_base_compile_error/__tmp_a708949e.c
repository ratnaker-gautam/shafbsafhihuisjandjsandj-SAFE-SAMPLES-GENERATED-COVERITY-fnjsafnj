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
    struct GeometricObject* objects;
    size_t count;
    double time_step;
    uint32_t step_count;
};

int validate_vector(const struct Vector3D* vec) {
    if (vec == NULL) return 0;
    if (!isfinite(vec->x) || !isfinite(vec->y) || !isfinite(vec->z)) return 0;
    return 1;
}

int validate_matrix(const struct Matrix3x3* mat) {
    if (mat == NULL) return 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!isfinite(mat->elements[i][j])) return 0;
        }
    }
    return 1;
}

int validate_object(const struct GeometricObject* obj) {
    if (obj == NULL) return 0;
    if (!validate_vector(&obj->position)) return 0;
    if (!validate_vector(&obj->velocity)) return 0;
    if (!isfinite(obj->mass) || obj->mass <= 0.0) return 0;
    if (obj->id < 0) return 0;
    return 1;
}

struct Vector3D vector_add(const struct Vector3D* a, const struct Vector3D* b) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (a == NULL || b == NULL) return result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

struct Vector3D vector_scale(const struct Vector3D* vec, double scalar) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (vec == NULL || !isfinite(scalar)) return result;
    result.x = vec->x * scalar;
    result.y = vec->y * scalar;
    result.z = vec->z * scalar;
    return result;
}

struct Vector3D matrix_vector_multiply(const struct Matrix3x3* mat, const struct Vector3D* vec) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (mat == NULL || vec == NULL) return result;
    result.x = mat->elements[0][0] * vec->x + mat->elements[0][1] * vec->y + mat->elements[0][2] * vec->z;
    result.y = mat->elements[1][0] * vec->x + mat->elements[1][1] * vec->y + mat->elements[1][2] * vec->z;
    result.z = mat->elements[2][0] * vec->x + mat->elements[2][1] * vec->y + mat->elements[2][2] * vec->z;
    return result;
}

void update_simulation(struct SimulationState* state) {
    if (state == NULL || state->objects == NULL) return;
    if (state->count == 0) return;
    if (!isfinite(state->time_step) || state->time_step <= 0.0) return;
    
    struct Matrix3x3 rotation = {
        .elements = {
            {0.866, -0.5, 0.0},
            {0.5, 0.866, 0.0},
            {0.0, 0.0, 1.0}
        }
    };
    
    if (!validate_matrix(&rotation)) return;
    
    for (size_t i = 0; i < state->count; i++) {
        if (!validate_object(&state->objects[i])) continue;
        
        struct Vector3D acceleration = {0.0, -9.81, 0.0};
        struct Vector3D velocity_change = vector_scale(&acceleration, state->time_step);
        state->objects[i].velocity = vector_add(&state->objects[i].velocity, &velocity_change);
        
        struct Vector3D position_change = vector_scale(&state->objects[i].velocity, state->time_step);
        state->objects[i].position = vector_add(&state->objects[i].position, &position_change);
        
        struct Vector3D rotated_velocity = matrix_vector_multiply(&rotation, &state->objects[i].velocity);
        if (validate_vector(&rotated_velocity)) {
            state->objects[i].velocity = rotated_velocity;
        }
    }
    
    state->step_count++;
}

void print_simulation_state(const struct SimulationState* state) {
    if (state == NULL) return;
    printf("Simulation Step: %u\n", state->step_count);
    printf("Time Step: %.6f\n", state->time_step);
    printf("Object Count: %zu\n", state->count);
    
    for (size_t i = 0; i < state->count; i++) {
        if (!validate_object(&state->objects[i])) continue;
        printf("Object %d: Pos(%.3f, %.