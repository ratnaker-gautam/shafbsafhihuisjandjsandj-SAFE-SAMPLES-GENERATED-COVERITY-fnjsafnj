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

struct Vector3D vector_add(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

struct Vector3D vector_scale(struct Vector3D v, double s) {
    struct Vector3D result;
    result.x = v.x * s;
    result.y = v.y * s;
    result.z = v.z * s;
    return result;
}

double vector_dot(struct Vector3D a, struct Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vector3D vector_cross(struct Vector3D a, struct Vector3D b) {
    struct Vector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

struct Matrix3x3 matrix_multiply(struct Matrix3x3 a, struct Matrix3x3 b) {
    struct Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.elements[i][j] = 0.0;
            for (int k = 0; k < 3; k++) {
                result.elements[i][j] += a.elements[i][k] * b.elements[k][j];
            }
        }
    }
    return result;
}

struct Vector3D matrix_vector_multiply(struct Matrix3x3 m, struct Vector3D v) {
    struct Vector3D result;
    result.x = m.elements[0][0] * v.x + m.elements[0][1] * v.y + m.elements[0][2] * v.z;
    result.y = m.elements[1][0] * v.x + m.elements[1][1] * v.y + m.elements[1][2] * v.z;
    result.z = m.elements[2][0] * v.x + m.elements[2][1] * v.y + m.elements[2][2] * v.z;
    return result;
}

struct Vector3D transform_point(struct Transform t, struct Vector3D point) {
    struct Vector3D rotated = matrix_vector_multiply(t.rotation, point);
    struct Vector3D scaled = vector_scale(rotated, t.scale);
    return vector_add(scaled, t.translation);
}

void update_bounds(struct BoundingBox* bounds, struct Vector3D point) {
    if (point.x < bounds->min.x) bounds->min.x = point.x;
    if (point.y < bounds->min.y) bounds->min.y = point.y;
    if (point.z < bounds->min.z) bounds->min.z = point.z;
    if (point.x > bounds->max.x) bounds->max.x = point.x;
    if (point.y > bounds->max.y) bounds->max.y = point.y;
    if (point.z > bounds->max.z) bounds->max.z = point.z;
}

int initialize_mesh(struct Mesh* mesh, uint32_t vertex_count) {
    if (vertex_count == 0 || vertex_count > 1000000) return 0;
    mesh->vertices = malloc(vertex_count * sizeof(struct Vector3D));
    if (mesh->vertices == NULL) return 0;
    mesh->vertex_count = vertex_count;
    mesh->bounds.min.x = INFINITY;
    mesh->bounds.min.y = INFINITY;
    mesh->bounds.min.z = INFINITY;
    mesh->bounds.max.x = -INFINITY;
    mesh->bounds.max.y = -INFINITY;
    mesh->bounds.max.z = -INFINITY;
    return 1;
}

void compute_mesh_bounds(struct Mesh* mesh) {
    if (mesh->vertices == NULL || mesh->vertex_count == 0) return;
    mesh->bounds.min.x = INFINITY;
    mesh->bounds.min.y = INFINITY;
    mesh->bounds.min.z = INFINITY;
    mesh->bounds.max.x = -INFINITY;
    mesh->bounds.max.y = -INFINITY;
    mesh->bounds.max.z = -INFINITY;
    for (uint32_t i = 0; i < mesh->vertex_count; i++) {
        update_bounds(&mesh->bounds, mesh->vertices[i]);
    }
}

void apply_transform_to_mesh(struct Mesh* mesh, struct Transform transform) {
    if (mesh->vertices == NULL || mesh->vertex_count == 0) return;
    for (uint32_t i = 0; i < mesh->vertex_count; i++) {
        mesh->vertices[i] = transform_point(transform