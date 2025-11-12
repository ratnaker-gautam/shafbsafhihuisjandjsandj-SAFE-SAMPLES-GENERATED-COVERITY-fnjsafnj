//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
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

void update_simulation(struct SimulationState *state) {
    if (state == NULL || state->objects == NULL || state->count == 0) return;
    if (!isfinite(state->time_step) || state->time_step <= 0.0) return;
    
    struct Matrix3x3 rotation = {
        .elements = {
            {0.9848, -0.1736, 0.0},
            {0.1736, 0.9848, 0.0},
            {0.0, 0.0, 1.0}
        }
    };
    
    if (!validate_matrix(&rotation)) return;
    
    for (size_t i = 0; i < state->count; i++) {
        if (!validate_object(&state->objects[i])) continue;
        
        struct Vector3D rotated_vel = matrix_vector_multiply(&rotation, &state->objects[i].velocity);
        if (!validate_vector(&rotated_vel)) continue;
        
        struct Vector3D displacement = vector_scale(&rotated_vel, state->time_step);
        if (!validate_vector(&displacement)) continue;
        
        struct Vector3D new_pos = vector_add(&state->objects[i].position, &displacement);
        if (!validate_vector(&new_pos)) continue;
        
        state->objects[i].position = new_pos;
        
        double energy = 0.5 * state->objects[i].mass * 
                       (state->objects[i].velocity.x * state->objects[i].velocity.x +
                        state->objects[i].velocity.y * state->objects[i].velocity.y +
                        state->objects[i].velocity.z * state->objects[i].velocity.z);
        if (isfinite(energy)) {
            printf("Object %d: Energy = %.6f, Position = (%.6f, %.6f, %.6f)\n",
                   state->objects[i].id, energy,
                   state->objects[i].position.x,
                   state->objects[i].position.y,
                   state->objects[i].position.z);
        }
    }
    state->step_count++;
}

int main(void) {
    struct SimulationState sim = {0};