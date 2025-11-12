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

struct Vector3D compute_force(const struct GeometricObject *obj1, const struct GeometricObject *obj2) {
    struct Vector3D force = {0.0, 0.0, 0.0};
    if (obj1 == NULL || obj2 == NULL) return force;
    if (!validate_object(obj1) || !validate_object(obj2)) return force;
    
    struct Vector3D diff = {
        obj2->position.x - obj1->position.x,
        obj2->position.y - obj1->position.y,
        obj2->position.z - obj1->position.z
    };
    
    double distance = vector_length(&diff);
    if (distance < 1e-10) return force;
    
    double force_magnitude = (obj1->mass * obj2->mass) / (distance * distance);
    if (!isfinite(force_magnitude)) return force;
    
    struct Vector3D direction = {
        diff.x / distance,
        diff.y / distance,
        diff.z / distance
    };
    
    force.x = direction.x * force_magnitude;
    force.y = direction.y * force_magnitude;
    force.z = direction.z * force_magnitude;
    
    return force;
}

int update_simulation(struct SimulationState *state) {
    if (state == NULL) return 0;
    if (state->object_count == 0 || state->object_count > 10) return 0;
    if (!isfinite(state->time_step) || state->time_step <= 0.0) return 0;
    if (!isfinite(state->total_time) || state->total_time < 0.0) return 0;
    
    for (size_t i = 0; i <