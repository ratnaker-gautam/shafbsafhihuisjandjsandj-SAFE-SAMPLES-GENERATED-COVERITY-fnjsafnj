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

struct Transform {
    struct Matrix3x3 rotation;
    struct Vector3D translation;
    double scale;
};

struct BoundingBox {
    struct Vector3D min;
    struct Vector3D max;
};

struct Mesh {
    struct Vector3D* vertices;
    uint32_t vertex_count;
    struct BoundingBox bounds;
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

int validate_transform(const struct Transform* tf) {
    if (tf == NULL) return 0;
    if (!validate_matrix(&tf->rotation)) return 0;
    if (!validate_vector(&tf->translation)) return 0;
    if (!isfinite(tf->scale) || tf->scale <= 0.0) return 0;
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

struct Vector3D transform_point(const struct Transform* tf, const struct Vector3D* point) {
    struct Vector3D result = {0.0, 0.0, 0.0};
    if (tf == NULL || point == NULL) return result;
    struct Vector3D rotated = matrix_vector_multiply(&tf->rotation, point);
    struct Vector3D scaled = vector_scale(&rotated, tf->scale);
    result = vector_add(&scaled, &tf->translation);
    return result;
}

void compute_bounds(struct Mesh* mesh) {
    if (mesh == NULL || mesh->vertices == NULL || mesh->vertex_count == 0) return;
    mesh->bounds.min = mesh->vertices[0];
    mesh->bounds.max = mesh->vertices[0];
    for (uint32_t i = 1; i < mesh->vertex_count; i++) {
        if (mesh->vertices[i].x < mesh->bounds.min.x) mesh->bounds.min.x = mesh->vertices[i].x;
        if (mesh->vertices[i].y < mesh->bounds.min.y) mesh->bounds.min.y = mesh->vertices[i].y;
        if (mesh->vertices[i].z < mesh->bounds.min.z) mesh->bounds.min.z = mesh->vertices[i].z;
        if (mesh->vertices[i].x > mesh->bounds.max.x) mesh->bounds.max.x = mesh->vertices[i].x;
        if (mesh->vertices[i].y > mesh->bounds.max.y) mesh->bounds.max.y = mesh->vertices[i].y;
        if (mesh->vertices[i].z > mesh->bounds.max.z) mesh->bounds.max.z = mesh->vertices[i].z;
    }
}

int transform_mesh(struct Mesh* result, const struct Mesh* input, const struct Transform* tf) {
    if (result == NULL || input == NULL || tf == NULL) return 0;
    if (!validate_transform(tf)) return 0;
    if (input->vertex_count == 0 || input->vertices == NULL) return 0;
    if (result->vertices != NULL) return 0;
    if (input->vertex_count