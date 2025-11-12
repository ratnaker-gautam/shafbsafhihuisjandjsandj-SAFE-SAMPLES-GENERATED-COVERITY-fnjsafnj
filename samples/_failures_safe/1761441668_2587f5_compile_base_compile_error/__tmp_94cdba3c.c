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
    if (!validate_vector(a) || !validate_vector(b)) return result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

struct Vector3D vector_scale(const struct Vector3D *vec, double scalar) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!validate_vector(vec) || !isfinite(scalar)) return result;
    result.x = vec->x * scalar;
    result.y = vec->y * scalar;
    result.z = vec->z * scalar;
    return result;
}

double vector_dot(const struct Vector3D *a, const struct Vector3D *b) {
    if (!validate_vector(a) || !validate_vector(b)) return 0.0;
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

struct Vector3D matrix_vector_multiply(const struct Matrix3x3 *mat, const struct Vector3D *vec) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (!validate_matrix(mat) || !validate_vector(vec)) return result;
    for (int i = 0; i < 3; i++) {
        result.x += mat->elements[0][i] * vec->x;
        result.y += mat->elements[1][i] * vec->y;
        result.z += mat->elements[2][i] * vec->z;
    }
    return result;
}

struct Vector3D compute_force(const struct GeometricObject *a, const struct GeometricObject *b) {
    struct Vector3D force = {0.0, 0.0, 0.0};
    if (!validate_object(a) || !validate_object(b)) return force;
    if (a == b) return force;
    
    struct Vector3D diff = {
        b->position.x - a->position.x,
        b->position.y - a->position.y,
        b->position.z - a->position.z
    };
    
    double distance_sq = vector_dot(&diff, &diff);
    if (distance_sq < 1e-12) return force;
    
    double distance = sqrt(distance_sq);
    double force_magnitude = (a->mass * b->mass) / distance_sq;
    
    if (!isfinite(force_magnitude)) return force;
    
    force.x = (diff.x / distance) * force_magnitude;
    force.y = (diff.y / distance) * force_magnitude;
    force.z = (diff.z / distance) * force_magnitude;
    
    return force;
}

void update_simulation(struct SimulationState *state) {
    if (state == NULL) return;
    if (state->object_count == 0 || state->object_count > 10) return;
    if (!isfinite(state->time_step) || state->time_step <= 0.0) return;
    
    for (size_t i = 0; i < state->object_count; i++) {
        if (!validate_object(&state->objects[i])) continue;
        
        struct Vector3D total_force = {0.0, 0.0, 0.0};
        for (size_t j = 0; j < state->object_count; j++) {
            if (i == j) continue;
            if (!validate_object(&state->objects[j])) continue;
            
            struct Vector3D force = compute_force(&state->objects[i], &state->objects[j]);
            total_force