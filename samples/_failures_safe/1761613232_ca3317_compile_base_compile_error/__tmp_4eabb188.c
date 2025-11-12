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
    struct Vector3D scale;
};

struct BoundingBox {
    struct Vector3D min;
    struct Vector3D max;
};

struct Geometry {
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
    if (!validate_vector(&tf->scale)) return 0;
    return 1;
}

struct Vector3D vector_add(const struct Vector3D* a, const struct Vector3D* b) {
    struct Vector3D result = {0, 0, 0};
    if (a == NULL || b == NULL) return result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

struct Vector3D vector_multiply(const struct Vector3D* a, const struct Vector3D* b) {
    struct Vector3D result = {0, 0, 0};
    if (a == NULL || b == NULL) return result;
    result.x = a->x * b->x;
    result.y = a->y * b->y;
    result.z = a->z * b->z;
    return result;
}

struct Vector3D matrix_vector_multiply(const struct Matrix3x3* mat, const struct Vector3D* vec) {
    struct Vector3D result = {0, 0, 0};
    if (mat == NULL || vec == NULL) return result;
    result.x = mat->elements[0][0] * vec->x + mat->elements[0][1] * vec->y + mat->elements[0][2] * vec->z;
    result.y = mat->elements[1][0] * vec->x + mat->elements[1][1] * vec->y + mat->elements[1][2] * vec->z;
    result.z = mat->elements[2][0] * vec->x + mat->elements[2][1] * vec->y + mat->elements[2][2] * vec->z;
    return result;
}

struct Vector3D transform_point(const struct Transform* tf, const struct Vector3D* point) {
    struct Vector3D result = {0, 0, 0};
    if (tf == NULL || point == NULL) return result;
    if (!validate_transform(tf) || !validate_vector(point)) return result;
    
    struct Vector3D scaled = vector_multiply(point, &tf->scale);
    struct Vector3D rotated = matrix_vector_multiply(&tf->rotation, &scaled);
    struct Vector3D translated = vector_add(&rotated, &tf->translation);
    
    return translated;
}

int compute_bounding_box(struct Geometry* geom) {
    if (geom == NULL || geom->vertices == NULL || geom->vertex_count == 0) return 0;
    
    geom->bounds.min.x = geom->vertices[0].x;
    geom->bounds.min.y = geom->vertices[0].y;
    geom->bounds.min.z = geom->vertices[0].z;
    geom->bounds.max.x = geom->vertices[0].x;
    geom->bounds.max.y = geom->vertices[0].y;
    geom->bounds.max.z = geom->vertices[0].z;
    
    for (uint32_t i = 1; i < geom->vertex_count; i++) {
        if (!validate_vector(&geom->vertices[i])) return 0;
        
        if (geom->vertices[i].x < geom->bounds.min.x) geom->bounds.min.x = geom->vertices[i].x;
        if (geom->vertices[i].y < geom->bounds.min.y) geom->bounds.min.y = geom->vertices[i].y;
        if (geom->vertices[i].z < geom->bounds.min.z) geom->bounds.min.z = geom->vertices[i].z;
        
        if (geom->vertices[i].x > geom->bounds.max.x) geom->bounds.max.x = geom->vertices[i].x;
        if (geom->vertices[i].y > geom->bounds.max.y) geom->bounds.max.y = geom->vertices[i].y;
        if (geom->vertices[i].z > geom->bounds.max.z) geom->bounds.max.z = geom->vertices[i].z;
    }
    
    return 1